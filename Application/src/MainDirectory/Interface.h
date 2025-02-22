#pragma once
#include <chrono>

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "ImguiExtra/imgui_stdlib.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"
#include "Database/DBOperations.h"

class OmniDBLayer : public Walnut::Layer
{
public:
	//Destructorul care se apeleaza la finalul programului
	~OmniDBLayer(){
		if (db)
			delete db;
	}

	//Functia de Render care se apeleaza pentru fiecare frame
	void OnUIRender() override
	{
		//Facem containerul de la ImGui sa fie de marimea ferestrei
		auto pos = ImGui::GetMainViewport()->Pos;
		pos.y += 56;
		auto size = ImGui::GetMainViewport()->Size;
		size.y -= 56;

		//Setam pozitia si marimea containerului principal
		ImGui::SetNextWindowPos(pos);
		ImGui::SetNextWindowSize(size);

		//Cream containerul principal, care nu poate fi mutat, nu are titlu, nu poate fi colapsat, nu poate fi dock-at, marimea nu ii se poate schimba de la utilizator, nu se salveaza setarile sale si marimea se adapteaza la fereastra
		if (ImGui::Begin("Main", (bool*)true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
		{
			//Cream textul de la inceput
			ImGui::Text("Bine ati venit la managerul de date de baza Omni Bank!");
			ImGui::Text("");
			ImGui::Text("Va rugam sa introduceti numele fisierului cu datele din data de baza.");
			ImGui::Text("Lasati gol si apasati pe citire daca doriti sa incepeti o noua data de baze.");
			ImGui::Text("Lasati gol si apasati pe salvati daca doriti sa salvati in fisierul din care ati citit initial (scrie mai jos).");
			ImGui::Text("Daca fisierul de citire este invalid (ex. nu exista), el nu se va lua in considerare si se va crea o data de baze noua:");
			//Cream zona in care inseram numele fisierului pentru citire si salvare
			ImGuiInputText::InputText("##NumeFisier", &tmp_fname, 120);
			//Cream butonul pentru citirea datelor
			if(ImGui::Button("Citeste datele")){
				delete db;

				db = new Database(tmp_fname, true);
			}
			//Scriem mai departe pe aceeasi linie
			ImGui::SameLine();
			//Creeam butonul pentru salvarea datelor
			if (ImGui::Button("Salveaza datele")){
				db->save_data(tmp_fname);
			}
			//Creeam textul care sa arate cu ce fisier lucram in prezent
			ImGui::Text(("Fisierul din care s-au citit datele si cu care lucram: '" + (db && db->is_initialized() ? db->get_curr_file() : "") + "'").c_str());
			//Lista de clienti
			DrawClientList();
			//Lista de clienti sortata dupa moneda si apoi nume
			DrawOrderedClientList();
			//Meniul de stergere a unui client
			DrawDelClientMenu();
			//Meniul de adaugare a unui client
			DrawAddClientMenu();
			//Lista de ccei mai bogati clienti pentru fiecare moneda
			DrawRichestClientList();
			//Bilantul total al bancii
			DrawTotalBankBalanceMenu();
			//Membrii echipei
			if (ImGui::CollapsingHeader("Membrii echipei"))
				ImGui::Text("Proiect realizat de juinc");
			//Setarile aplicatiei
			if(ImGui::CollapsingHeader("Setari"))
			{
				ImGui::Checkbox("Actualizare automata a listelor", &auto_update);
				ImGui::Checkbox("Salvare automata a datelor la iesirea din program", &auto_save);

				if (db && db->is_initialized())
					db->set_auto_save(auto_save);
			}
			//Butonul de iesire din program
			if (ImGui::Button("Iesire program"))
				Walnut::Application::Get().Close();
		}ImGui::End();

		//Creeam notificarea de cine este facut programul (daca este deschisa)
		UI_DrawAboutModal();
		//Creeam notificarea ca nu avem niciun fisier deschis (daca am incercat o operatie fara sa avem niciun fisier citit)
		DrawNotInitializedWarning();
	}

	void ShowAboutModal()
	{
		m_AboutModalOpen = true;
	}
private:
	//Deseneaza un tabel pe baza unei liste de clienti
	static void DrawTable(const std::vector<Database::Client>& clnts)
	{
		//Marimea exterioara a tabelului
		ImVec2 outer_size = ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 8);

		//Cream un tabel care are scrolling vertical
		if (ImGui::BeginTable("table_scrolly", 6, ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, outer_size))
		{
			ImGui::TableSetupScrollFreeze(0, 1); // Facem prima linie vizibila intotdeauna
			ImGui::TableSetupColumn("Nume", ImGuiTableColumnFlags_None); // Prima linie prima coloana
			ImGui::TableSetupColumn("Prenume", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("NrCont", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Moneda", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Sold", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("UltimaOperatie", ImGuiTableColumnFlags_None);
			//Selectam prima linie (cea cu "Nume", "Prenume" etc) ca sa trecem la a doua
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();

			//Pentru fiecare client din lista asezam datele sale in tabel
			for (const Database::Client& clnt : clnts)
			{
				//Trecem la prima coloana
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(clnt.name.c_str());

				//Trecem la urmatoarea
				ImGui::TableNextColumn();
				ImGui::Text(clnt.surname.c_str());

				ImGui::TableNextColumn();
				ImGui::Text(std::to_string(clnt.acc_nr).c_str());

				ImGui::TableNextColumn();
				ImGui::Text(clnt.currency.c_str());

				ImGui::TableNextColumn();
				ImGui::Text(std::to_string(clnt.balance).c_str());

				ImGui::TableNextColumn();
				ImGui::Text(clnt.last_op.c_str());

				ImGui::TableNextRow();
			}
			ImGui::EndTable();
		}
	}
	//Deseneaza zona cu lista normala de clienti
	void DrawClientList()
	{
		//Cream zona cu lista de clienti
		if (ImGui::CollapsingHeader("Lista Clienti"))
		{
			//Cream si verificam daca butonul pentru lista de clienti este apasat sau daca avem auto-actualizarea pornita
			if (ImGui::Button("Afiseaza Lista Clienti Normali") || auto_update)
			{
				//Daca nu am deschis niciun fisier, avertizam utilizatorul
				if (!db)
				{
					m_warning = true;
					return;
				}

				//Actualizam lista cu lista normala de clienti
				normal = db->get_client_list();
			}

			//Cream tabelul
			DrawTable(normal);
		}
	}
	//Deseneaza zona cu lista ordonata de clienti dupa moneda si apoi nume
	void DrawOrderedClientList()
	{
		//Cream zona cu clientii ordonati alfabetic dupa moneda si apoi nume
		if (ImGui::CollapsingHeader("Lista Clienti Ordonata Alfabetic Dupa Moneda + Nume"))
		{
			//Cream si verificam daca butonul pentru lista ordonata de clienti este apasat sau daca avem auto-actualizarea pornita
			if (ImGui::Button("Afiseaza Lista Clienti Ordonata") || auto_update) {
				//Daca nu am deschis niciun fisier, avertizam utilizatorul
				if (!db)
				{
					m_warning = true;
					return;
				}
				//Actualizam lista locala cu lista normala de clienti
				sorted = db->get_client_list_sorted();
			}

			//Cream un vector temporar care sa tina toti clientii in ordine, deoarece sorted este un tablou bidimensional
			std::vector<Database::Client> list;
			for (auto& tmp_list : sorted)
				for (auto& clnt : tmp_list)
					list.push_back(clnt);

			//Desenam tabelul pe baza vectorului temporar
			DrawTable(list);
		}
	}
	//Deseneaza zona cu meniul de stergere al unui client
	void DrawDelClientMenu()
	{
		//Cream zona cu meniul de stergere al unui client
		if (ImGui::CollapsingHeader("Sterge Client"))
		{
			//Cream zona in care sa introducem numele si prenumele clientului
			ImGuiInputText::InputText("Introduceti Numele Stergere", &tmp_name_rem, 120);
			ImGuiInputText::InputText("Introduceti Prenumele Stergere", &tmp_surname_rem, 120);

			//Cream si verificam daca butonul pentru stergerea clientului este apasat
			if (ImGui::Button("Stergeti Client"))
			{
				//Daca nu am deschis niciun fisier, avertizam utilizatorul
				if (!db)
				{
					m_warning = true;
					return;
				}

				//Ascundem textul de success sau fail
				show_clientrem_succ = false;
				show_clientrem_fail = false;

				//Salvam timpul cand am apasat butonul
				pressed_rem = std::chrono::system_clock::now();

				//Stergem clientul din data de baze, si verificam daca s-a facut cu succes
				if (db->remove_client(tmp_name_rem, tmp_surname_rem))
					show_clientrem_succ = true;
				else
					show_clientrem_fail = true;
					

				//Resetam datele locale
				tmp_name_rem.clear();
				tmp_surname_rem.clear();
				tmp_currency.clear();
				tmp_balance.clear();
			}

			//Daca scadem din timpul in prezent, timpul la care am apasat butonul, obtinem timpul trecut de cand am apasat butonul
			//Daca a trecut mai mult de o secunda, ascundem textul
			if(std::chrono::system_clock::now() - pressed_rem > std::chrono::duration<int>(1))
			{
				show_clientrem_succ = false;
				show_clientrem_fail = false;
			}

			//Afisam textul
			if (show_clientrem_succ)
				ImGui::Text("Client sters cu succes!");
			if (show_clientrem_fail)
				ImGui::Text("S-a produs o eroare!");
		}
	}
	//Deseneaza zona cu meniul de adaugare al unui client
	void DrawAddClientMenu()
	{
		//Cream zona cu meniul de creare al unui client
		if (ImGui::CollapsingHeader("Adauga Client"))
		{
			//Cream zona in care sa introducem numele, prenumele, moneda si bilantul clientului
			ImGuiInputText::InputText("Introduceti Numele", &tmp_name_add, 120);
			ImGuiInputText::InputText("Introduceti Prenumele", &tmp_surname_add, 120);
			ImGuiInputText::InputText("Introduceti Moneda", &tmp_currency, 120);
			ImGuiInputText::InputText("Introduceti Bilantul", &tmp_balance, 120);

			//Cream si verificam daca butonul pentru stergerea clientului este apasat
			if (ImGui::Button("Adaugati Client"))
			{
				pressed_add = std::chrono::system_clock::now();

				//Verificam daca datele citite sunt goale
				if(tmp_name_add.empty() || tmp_surname_add.empty() || tmp_currency.empty() || tmp_balance.empty() || tmp_balance.find_first_not_of("-.1234567890") != std::string::npos)
				{
					show_clientadd_invalid = true;
					return;
				}
				//Daca nu am deschis niciun fisier, avertizam utilizatorul
				if (!db)
				{
					m_warning = true;
					return;
				}

				//Ascundem textul de success sau fail
				show_clientadd_succ = false;
				show_clientadd_fail = false;
				show_clientadd_invalid = false;

				//Salvam timpul cand am apasat butonul
				pressed_add = std::chrono::system_clock::now();

				//Adaugam clientul din data de baze, si verificam daca s-a facut cu succes
				if (db->add_client(tmp_name_add, tmp_surname_add, tmp_currency, std::stof(tmp_balance)))
					show_clientadd_succ = true;
				else
					show_clientadd_fail = true;
				

				//Resetam datele locale
				tmp_name_add.clear();
				tmp_surname_add.clear();
				tmp_currency.clear();
				tmp_balance.clear();
			}

			//Daca scadem din timpul in prezent, timpul la care am apasat butonul, obtinem timpul trecut de cand am apasat butonul
			//Daca a trecut mai mult de o secunda, ascundem textul
			if (std::chrono::system_clock::now() - pressed_add > std::chrono::duration<int>(1))
			{
				show_clientadd_succ = false;
				show_clientadd_fail = false;
				show_clientadd_invalid = false;
			}

			//Afisam textul
			if (show_clientadd_succ)
				ImGui::Text("Client adaugat cu succes!");
			if (show_clientadd_fail)
				ImGui::Text("S-a produs o eroare!");
			if (show_clientadd_invalid)
				ImGui::Text("Datele sunt invalide!");
		}
	}
	//Deseneaza zona cu lista de cei mai bogati clienti
	void DrawRichestClientList()
	{
		//Cream zona cu clientii care au cel mai mare sold pentru fiecare moneda
		if (ImGui::CollapsingHeader("Lista Clienti Cu Cel Mai Mare Sold Pt Fiecare Moneda"))
		{
			//Cream si verificam daca butonul pentru lista de clienti este apasat sau daca avem auto-actualizarea pornita
			if (ImGui::Button("Afiseaza Lista Clienti Bogati") || auto_update) {
				//Daca nu am deschis niciun fisier, avertizam utilizatorul
				if (!db)
				{
					m_warning = true;
					return;
				}

				//Actualizam lista locala cu lista de cei mai bogati clienti
				richest = db->get_client_list_max_curr();
			}

			//Desenam tabelul pe baza vectrolui cu cei mai bogati clienti
			DrawTable(richest);
		}
	}
	//Deseneaza bilantul total al bancii
	void DrawTotalBankBalanceMenu()
	{
		//Cream zona cu soldul total al bancii
		if (ImGui::CollapsingHeader("Soldul Total Al Bancii"))
		{
			//Cream si verificam daca butonul pentru soldul total este apasat sau daca avem auto-actualizarea pornita
			if (ImGui::Button("Actualizeaza") || auto_update) {
				//Daca nu am deschis niciun fisier, avertizam utilizatorul
				if (!db)
				{
					m_warning = true;
					return;
				}

				//Actualizam bilantul local al datei de baza
				balance_tot = db->get_total_balance();
			}

			//Actualizam bilantul total al datei de baza
			ImGui::Text(("Soldul total al bancii este: " + std::to_string(balance_tot)).c_str());
		}
	}
	//Avertismentul ca fisierul nu este deschis
	void DrawNotInitializedWarning()
	{
		//Daca nu trebuie sa avertizam utilizatorul
		if (!m_warning)
			return;

		//Cream popup-ul
		ImGui::OpenPopup("Avertisment");
		m_warning = ImGui::BeginPopupModal("Avertisment", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		if (m_warning)
		{
			//Preluam imaginea aplicatiei
			auto image = Walnut::Application::Get().GetApplicationIcon();
			ImGui::Image(image->GetDescriptorSet(), { 48, 48 });

			//Scriem pe aceeasi linie ca imaginea
			ImGui::SameLine();
			Walnut::UI::ShiftCursorX(20.0f);

			//Cream grupul principal de text
			ImGui::BeginGroup();
			ImGui::Text("Nu exista niciun fisier de date deschis / citit!");
			ImGui::EndGroup();

			//Cream butonul de inchidere
			if (Walnut::UI::ButtonCentered("Close"))
			{
				//Setam avertismentul la fals pentru a-l putea inchide
				m_warning = false;
				//steam auto-actualizarea la fals pentru a evita o bucla infinita si pentru ca nu are sens daca nu avem niciun fisier deschis
				auto_update = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	//Meniul de informatii al aplicatiei
	void UI_DrawAboutModal()
	{
		//Daca nu avem meniul de informatii deschis, returnam
		if (!m_AboutModalOpen)
			return;

		//Cream butonul de "Info", sus langa iconita
		ImGui::OpenPopup("Info");
		m_AboutModalOpen = ImGui::BeginPopupModal("Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		//Verificam daca meniul este deschis
		if (m_AboutModalOpen)
		{
			//Obtinem imaginea programului si o afisam
			auto image = Walnut::Application::Get().GetApplicationIcon();
			ImGui::Image(image->GetDescriptorSet(), { 48, 48 });

			//Pe aceeasi linie, afisam textul
			ImGui::SameLine();
			Walnut::UI::ShiftCursorX(20.0f);

			ImGui::BeginGroup();
			ImGui::Text("Managerul de date de baze Omni Bank");
			ImGui::Text("Creat de Omni Inc.");
			ImGui::EndGroup();

			//Cream butonul de inchidere
			if (Walnut::UI::ButtonCentered("Close"))
			{
				m_AboutModalOpen = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
private:
	bool m_AboutModalOpen = false; // Meniul de informatii despre program
	bool m_warning = false; // Avertismentul ca nu avem niciun fisier deschis
	bool auto_update = false; // Auto actualizarea tabelelor

	//Timpul cand afisam textul, il folosim ca dupa o secunda sa nu se mai afiseze
	std::chrono::time_point<std::chrono::system_clock> pressed_rem;
	std::chrono::time_point<std::chrono::system_clock> pressed_add;

	//Variable pentru cazul in care afisam textul daca clientul a putut fi adaugat sau nu
	bool show_clientadd_succ = false;
	bool show_clientadd_fail = false;
	bool show_clientadd_invalid = false;
	//Variable pentru cazul in care afisam textul daca clientul a putut fi sters sau nu
	bool show_clientrem_succ = false;
	bool show_clientrem_fail = false;

	//Daca sa se salveze datele automat in fisiere la inchiderea lor / iesirea din program
	bool auto_save = true;

	//Pointerul la data de baze
	Database* db = nullptr;

	//Datele bancii care se actualizeaza local
	std::vector<Database::Client> normal;
	std::vector<std::vector<Database::Client>> sorted;
	std::vector<Database::Client> RON_Clients;
	std::vector<Database::Client> richest;
	long double balance_tot = 0.0f;

	//Datele locale pentru crearea si stergerea unui cont
	std::string tmp_name_add, tmp_surname_add, tmp_name_rem, tmp_surname_rem, tmp_currency, tmp_balance;

	//Numele fisierului din care s-a citit ultima oara
	std::string tmp_fname;
};

inline Walnut::Application* Walnut::CreateApplication(int argc, char** argv){
	//Cream specificul aplicatiei
	Walnut::ApplicationSpecification spec;
	spec.Name = "Omni Bank Manager";
	spec.CustomTitlebar = true;

	//Declaram aplicatia in sine, pe baza specificului
	Walnut::Application* app = new Walnut::Application(spec);

	//Cream si adaugam layer-ul / container-ul principal
	std::shared_ptr<OmniDBLayer> exampleLayer = std::make_shared<OmniDBLayer>();
	app->PushLayer(exampleLayer);

	//Adaugam un callback pe bara de sus pentru meniul de informatii
	app->SetMenubarCallback([app, exampleLayer]()
		{
		//Adaugam butonul de "Info" langa iconita de sus
			if (ImGui::BeginMenu("Info"))
			{
				//Adaugam obtiunea de About pentru informatii
				if (ImGui::MenuItem("Despre"))
					//Afisam informatiile despre program
					exampleLayer->ShowAboutModal();
				
				ImGui::EndMenu();
			}
		});

	return app;
}

//Functia main a interfetei
inline int UInterfaceMain(int argc, char** argv){
	//Apelam functia main a frameworkului, care subecvent apeleaza functia CreateApplication implementata de noi
	return WalnutMain(argc, argv);
}