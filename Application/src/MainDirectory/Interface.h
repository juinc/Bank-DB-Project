#pragma once
#include <chrono>
#include <regex>
#include <filesystem>
#include <iostream>

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "ImguiExtra/imgui_stdlib.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"
#include "Database/DBOperations.h"
#include "Localization/Localization.h"

// Global localization instance
// Instanta globala de localizare
inline Localization g_Localization;

// Helper macro for localized text
// Macro pentru text localizat
#define L(key) g_Localization.Get(key).c_str()

class OmniDBLayer : public Walnut::Layer
{
public:
	//The destructor which is called at the end of the program
	//Destructorul care se apeleaza la finalul programului
	~OmniDBLayer() {
		if (db)
			delete db;
	}

	//Render function that is called at the start of the program
	//Functia de Render care se apeleaza pentru fiecare frame
	void OnUIRender() override
	{
		//We create the main ImGui container, which will be resized to the size of the window
		//Facem containerul de la ImGui sa fie de marimea ferestrei
		auto pos = ImGui::GetMainViewport()->Pos;
		pos.y += 56;
		auto size = ImGui::GetMainViewport()->Size;
		size.y -= 56;

		//We set the position and size of the main container
		//Setam pozitia si marimea containerului principal
		ImGui::SetNextWindowPos(pos);
		ImGui::SetNextWindowSize(size);

		//We create the main container, which cannot be moved, has no title, cannot be collapsed, cannot be docked, its size cannot be changed by the user, its settings are not saved and its size adapts to the window
		//Cream containerul principal, care nu poate fi mutat, nu are titlu, nu poate fi colapsat, nu poate fi dock-at, marimea nu ii se poate schimba de la utilizator, nu se salveaza setarile sale si marimea se adapteaza la fereastra
		if (ImGui::Begin("Main", (bool*)true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
		{
			//We create the text at the beginning of the window
			//Cream textul de la inceput
			ImGui::Text(L("welcome"));
			ImGui::Text("");
			ImGui::Text(L("prompt.filename"));
			ImGui::Text(L("prompt.newdb"));
			ImGui::Text(L("prompt.savefile"));
			ImGui::Text(L("prompt.invalidfile"));

			//We create the section where we insert the file name for reading and saving
			//Cream zona in care inseram numele fisierului pentru citire si salvare
			ImGuiInputText::InputText("##NumeFisier", &tmp_fname, 120);

			// File name validation
			// Validare nume fisier
			bool valid_fname = tmp_fname.empty() || IsValidFileName(tmp_fname);

			//We create the button for reading the data
			//Cream butonul pentru citirea datelor
			if (ImGui::Button(L("button.read"))) {
				//If the file name is invalid and not empty, show error
				//Daca numele fisierului este invalid si nu e gol, afisam eroare
				if (!valid_fname && !tmp_fname.empty()) {
					show_file_invalid = true;
				}
				else {
					delete db;
					db = new Database(tmp_fname, true);
					show_file_invalid = false;
				}
			}

			//We write further on the same line
			//Scriem mai departe pe aceeasi linie
			ImGui::SameLine();

			//We create the button for saving the data
			//Cream butonul pentru salvarea datelor
			if (ImGui::Button(L("button.save"))) {
				//If the file name is invalid and not empty, show error
				//Daca numele fisierului este invalid si nu e gol, afisam eroare
				if (!valid_fname && !tmp_fname.empty()) {
					show_file_invalid = true;
				}
				else if (db) {
					db->save_data(tmp_fname);
					show_file_invalid = false;
				}
			}

			//If the file name is invalid, show error message
			//Daca numele fisierului este invalid, afisam mesaj de eroare
			if (show_file_invalid) {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), L("error.invalidfilename"));
			}

			//We create the text that shows the current file in use
			//Cream textul care sa arate cu ce fisier lucram in prezent
			ImGui::Text((std::string(L("currentfile")) + " '" + (db && db->is_initialized() ? db->get_curr_file() : "") + "'").c_str());

			//The list of clients
			//Lista de clienti
			DrawClientList();

			//The list of clients sorted by currency and then by name
			//Lista de clienti sortata dupa moneda si apoi nume
			DrawOrderedClientList();

			//The menu for deleting a client
			//Meniul de stergere a unui client
			DrawDelClientMenu();

			//The menu for adding a client
			//Meniul de adaugare a unui client
			DrawAddClientMenu();

			//The list of the richest clients for each currency
			//Lista de cei mai bogati clienti pentru fiecare moneda
			DrawRichestClientList();

			//The total balance of the bank
			//Bilantul total al bancii
			DrawTotalBankBalanceMenu();

			//The members of the team
			//Membrii echipei
			if (ImGui::CollapsingHeader(L("team.header")))
				ImGui::Text(L("team.members"));

			//The application settings
			//Setarile aplicatiei
			if (ImGui::CollapsingHeader(L("menu.settings")))
			{
				//Checkbox for auto-update
				//Bifa pentru auto-actualizare
				ImGui::Checkbox(L("settings.autoupdate"), &auto_update);
				//Checkbox for auto-save
				//Bifa pentru auto-salvare
				ImGui::Checkbox(L("settings.autosave"), &auto_save);

				//If the database is initialized, set auto-save
				//Daca baza de date este initializata, setam auto-salvarea
				if (db && db->is_initialized())
					db->set_auto_save(auto_save);

				// Language dropdown
				// Selectare limba
				static int lang_idx = 0;
				const char* langs[] = { "English", "Romana" };
				// Language selection combo box
				// Combo box pentru selectarea limbii
				if (ImGui::Combo(L("menu.language"), &lang_idx, langs, IM_ARRAYSIZE(langs))) {
					// Load the selected language file
					// Incarcam fisierul de limba selectat
					if (lang_idx == 0)
						g_Localization.SetLanguage("en");
					else
						g_Localization.SetLanguage("ro");
				}
			}

			//The button to exit the program
			//Butonul de iesire din program
			if (ImGui::Button(L("menu.quit")))
				Walnut::Application::Get().Close();
		}ImGui::End();

		//We create the notification that the program is made by whom (if it is open)
		//Creeam notificarea de cine este facut programul (daca este deschisa)
		UI_DrawAboutModal();

		//We create the notification that we do not have any file open (if we tried an operation without having any file read)
		//Creeam notificarea ca nu avem niciun fisier deschis (daca am incercat o operatie fara sa avem niciun fisier citit)
		DrawNotInitializedWarning();
	}

	//Shows the about modal
	//Afiseaza fereastra de informatii despre program
	void ShowAboutModal()
	{
		m_AboutModalOpen = true;
	}
private:

	//Draws a table of the richest clients for each currency
	//Deseneaza un tabel pe baza unei liste de clienti
	static void DrawTable(const std::vector<Database::Client>& clnts)
	{
		//The external size of the table
		//Marimea exterioara a tabelului
		ImVec2 outer_size = ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 8);

		//We create a table that has vertical scrolling
		//Cream un tabel care are scrolling vertical
		if (ImGui::BeginTable("table_scrolly", 6, ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, outer_size))
		{
			ImGui::TableSetupScrollFreeze(0, 1); // Facem prima linie vizibila intotdeauna / We make the first line always visible
			ImGui::TableSetupColumn(L("table.name"), ImGuiTableColumnFlags_None); // Prima linie prima coloana / First line first column
			ImGui::TableSetupColumn(L("table.surname"), ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn(L("table.accnr"), ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn(L("table.currency"), ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn(L("table.balance"), ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn(L("table.lastop"), ImGuiTableColumnFlags_None);

			//We select the first line (the one with "Nume", "Prenume" etc) to move to the second one
			//Selectam prima linie (cea cu "Nume", "Prenume" etc) ca sa trecem la a doua
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();

			//For each client in the list we set their data in the table
			//Pentru fiecare client din lista asezam datele sale in tabel
			for (const Database::Client& clnt : clnts)
			{
				//We go to the first column
				//Trecem la prima coloana
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(clnt.name.c_str());

				//We go to the next column
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

	//Draws the section with the normal client list
	//Deseneaza zona cu lista normala de clienti
	void DrawClientList()
	{
		//We create the section with the list of clients
		//Cream zona cu lista de clienti
		if (ImGui::CollapsingHeader(L("header.clientlist")))
		{
			//We create and check if the button for the normal client list is pressed or if we have auto-update enabled
			//Cream si verificam daca butonul pentru lista de clienti este apasat sau daca avem auto-actualizarea pornita
			if (ImGui::Button(L("button.showclients")) || auto_update)
			{
				//If we haven't opened any file, we warn the user
				//Daca nu am deschis niciun fisier, avertizam utilizatorul
				if (!db)
				{
					m_warning = true;
					return;
				}

				//We update the local list with the normal client list
				//Actualizam lista cu lista normala de clienti
				normal = db->get_client_list();
			}

			//We create the table
			//Cream tabelul
			DrawTable(normal);
		}
	}

	//Draws the section with the ordered client list by currency and then by name
	//Deseneaza zona cu lista ordonata de clienti dupa moneda si apoi nume
	void DrawOrderedClientList()
	{
		//We create the section with the clients ordered alphabetically by currency and then by name
		//Cream zona cu clientii ordonati alfabetic dupa moneda si apoi nume
		if (ImGui::CollapsingHeader(L("header.orderedlist")))
		{
			//We create and check if the button for the ordered client list is pressed or if we have auto-update enabled
			//Cream si verificam daca butonul pentru lista ordonata de clienti este apasat sau daca avem auto-actualizarea pornita
			if (ImGui::Button(L("button.showordered")) || auto_update) {

				//If we haven't opened any file, we warn the user
				//Daca nu am deschis niciun fisier, avertizam utilizatorul
				if (!db)
				{
					m_warning = true;
					return;
				}

				//Update the local list with the ordered client list
				//Actualizam lista locala cu lista normala de clienti
				sorted = db->get_client_list_sorted();
			}

			//We create a temporary vector that holds all the clients in order, since sorted is a two-dimensional array
			//Cream un vector temporar care sa tina toti clientii in ordine, deoarece sorted este un tablou bidimensional
			std::vector<Database::Client> list;
			for (auto& tmp_list : sorted)
				for (auto& clnt : tmp_list)
					list.push_back(clnt);

			//We draw the table based on the temporary vector
			//Desenam tabelul pe baza vectorului temporar
			DrawTable(list);
		}
	}

	//Draws the section with the menu for deleting a client
	//Deseneaza zona cu meniul de stergere al unui client
	void DrawDelClientMenu()
	{
		//We create the section with the menu for deleting a client
		//Cream zona cu meniul de stergere al unui client
		if (ImGui::CollapsingHeader(L("header.deleteclient")))
		{
			//We create the section where we insert the name and surname of the client to be deleted
			//Cream zona in care sa introducem numele si prenumele clientului
			ImGuiInputText::InputText(L("input.delname"), &tmp_name_rem, 120);
			ImGuiInputText::InputText(L("input.delsurname"), &tmp_surname_rem, 120);

			// Validate name and surname
			// Validam numele si prenumele
			bool valid_name = IsValidName(tmp_name_rem);
			bool valid_surname = IsValidName(tmp_surname_rem);

			//We create the section where we insert the currency and balance of the client to be deleted
			//Cream si verificam daca butonul pentru stergerea clientului este apasat
			if (ImGui::Button((std::string(L("button.deleteclient")) + std::string("##removeclient")).c_str()))
			{
				//If we did not open any file, we warn the user
				//Daca nu am deschis niciun fisier, avertizam utilizatorul
				if (!db)
				{
					m_warning = true;
					return;
				}

				//We hide the success or fail text
				//Ascundem textul de success sau fail
				show_clientrem_succ = false;
				show_clientrem_fail = false;
				show_clientrem_invalid = false;

				//We save the time when the button was pressed
				//Salvam timpul cand am apasat butonul
				pressed_rem = std::chrono::system_clock::now();

				// Validate input before attempting to remove
				// Validam inputul inainte de a incerca stergerea
				if (!valid_name || !valid_surname) {
					show_clientrem_invalid = true;
				}
				else if (db->remove_client(tmp_name_rem, tmp_surname_rem))
					show_clientrem_succ = true;
				else
					show_clientrem_fail = true;

				//We reset the local data
				//Resetam datele locale
				tmp_name_rem.clear();
				tmp_surname_rem.clear();
				tmp_currency.clear();
				tmp_balance.clear();
			}

			//If we subtract from the current time the time when we pressed the button, we get the time that has passed since we pressed the button
			//If more than one second has passed, we hide the text	
			//Daca scadem din timpul in prezent, timpul la care am apasat butonul, obtinem timpul trecut de cand am apasat butonul
			//Daca a trecut mai mult de o secunda, ascundem textul
			if (std::chrono::system_clock::now() - pressed_rem > std::chrono::duration<int>(1))
			{
				show_clientrem_succ = false;
				show_clientrem_fail = false;
				show_clientrem_invalid = false;
			}

			//We draw the text
			//Afisam textul
			if (show_clientrem_succ)
				ImGui::TextColored(ImVec4(0, 1, 0, 1), L("success.clientdeleted"));
			if (show_clientrem_fail)
				ImGui::TextColored(ImVec4(1, 0, 0, 1), L("error.deletefail"));
			if (show_clientrem_invalid)
				ImGui::TextColored(ImVec4(1, 0.5, 0, 1), L("error.invalidname"));
		}
	}

	//Draws the section with the menu for adding a client
	//Deseneaza zona cu meniul de adaugare al unui client
	void DrawAddClientMenu()
	{
		//We create the section with the menu for adding a client
		//Cream zona cu meniul de creare al unui client
		if (ImGui::CollapsingHeader(L("header.addclient")))
		{
			//We create the section where we insert the name, surname, currency and balance of the client to be added
			//Cream zona in care sa introducem numele, prenumele, moneda si bilantul clientului
			ImGuiInputText::InputText(L("input.addname"), &tmp_name_add, 120);
			ImGuiInputText::InputText(L("input.addsurname"), &tmp_surname_add, 120);
			ImGuiInputText::InputText(L("input.addcurrency"), &tmp_currency, 120);
			ImGuiInputText::InputText(L("input.addbalance"), &tmp_balance, 120);

			// Validate all input fields
			// Validam toate campurile de input
			bool valid_name = IsValidName(tmp_name_add);
			bool valid_surname = IsValidName(tmp_surname_add);
			bool valid_currency = IsValidCurrency(tmp_currency);
			bool valid_balance = IsValidBalance(tmp_balance);

			//We create and check if the button for adding a client is pressed
			//Cream si verificam daca butonul pentru adaugarea clientului este apasat
			if (ImGui::Button((std::string(L("button.addclient")) + std::string("##addclient")).c_str()))
			{
				std::cout << "adding client" << std::endl;
				pressed_add = std::chrono::system_clock::now();

				//We verify if the data read is empty or invalid
				//Verificam daca datele citite sunt goale sau invalide
				if (!valid_name || !valid_surname || !valid_currency || !valid_balance)
				{
					show_clientadd_invalid = true;
					show_clientadd_succ = false;
					show_clientadd_fail = false;
					return;
				}

				//If we did not open any file, we warn the user
				//Daca nu am deschis niciun fisier, avertizam utilizatorul
				if (!db)
				{
					m_warning = true;
					return;
				}

				//We hide the success or fail text
				//Ascundem textul de success sau fail
				show_clientadd_succ = false;
				show_clientadd_fail = false;
				show_clientadd_invalid = false;

				//We save the time when the button was pressed
				//Salvam timpul cand am apasat butonul
				pressed_add = std::chrono::system_clock::now();

				//We add the client to the database, and check if it was done successfully
				//Adaugam clientul in baza de date, si verificam daca s-a facut cu succes
				if (db->add_client(tmp_name_add, tmp_surname_add, tmp_currency, std::stof(tmp_balance)))
					show_clientadd_succ = true;
				else
					show_clientadd_fail = true;

				//We reset the local data
				//Resetam datele locale
				tmp_name_add.clear();
				tmp_surname_add.clear();
				tmp_currency.clear();
				tmp_balance.clear();
			}

			//If we subtract from the current time the time when we pressed the button, we get the time that has passed since we pressed the button
			//If more than one second has passed, we hide the text
			//Daca scadem din timpul in prezent, timpul la care am apasat butonul, obtinem timpul trecut de cand am apasat butonul
			//Daca a trecut mai mult de o secunda, ascundem textul
			if (std::chrono::system_clock::now() - pressed_add > std::chrono::duration<int>(1))
			{
				show_clientadd_succ = false;
				show_clientadd_fail = false;
				show_clientadd_invalid = false;
			}

			//We draw the text
			//Afisam textul
			if (show_clientadd_succ)
				ImGui::TextColored(ImVec4(0, 1, 0, 1), L("success.clientadded"));
			if (show_clientadd_fail)
				ImGui::TextColored(ImVec4(1, 0, 0, 1), L("error.addfail"));
			if (show_clientadd_invalid)
				ImGui::TextColored(ImVec4(1, 0.5, 0, 1), L("error.invalidinput"));
		}
	}

	//Draws the section with the list of the richest clients for each currency
	//Deseneaza zona cu lista de cei mai bogati clienti
	void DrawRichestClientList()
	{
		//We create the section with the list of clients with the highest balance for each currency
		//Cream zona cu clientii care au cel mai mare sold pentru fiecare moneda
		if (ImGui::CollapsingHeader(L("header.richestclients")))
		{
			//We create and check if the button for the richest client list is pressed or if we have auto-update enabled
			//Cream si verificam daca butonul pentru lista de clienti este apasat sau daca avem auto-actualizarea pornita
			if (ImGui::Button(L("button.showrichest")) || auto_update) {
				//If we haven't opened any file, we warn the user
				//Daca nu am deschis niciun fisier, avertizam utilizatorul
				if (!db)
				{
					m_warning = true;
					return;
				}

				//We update the local list with the richest client list
				//Actualizam lista locala cu lista de cei mai bogati clienti
				richest = db->get_client_list_max_curr();
			}

			//If the list is empty, we write that there are no clients
			//Desenam tabelul pe baza vectorului cu cei mai bogati clienti
			DrawTable(richest);
		}
	}

	//Draws the section with the total balance of the bank
	//Deseneaza bilantul total al bancii
	void DrawTotalBankBalanceMenu()
	{
		//We create the section with the total balance of the bank
		//Cream zona cu soldul total al bancii
		if (ImGui::CollapsingHeader(L("header.totalbalance")))
		{
			//We create and check if the button for the total balance is pressed or if we have auto-update enabled
			//Cream si verificam daca butonul pentru soldul total este apasat sau daca avem auto-actualizarea pornita
			if (ImGui::Button(L("button.updatebalance")) || auto_update) {
				//If we haven't opened any file, we warn the user
				//Daca nu am deschis niciun fisier, avertizam utilizatorul
				if (!db)
				{
					m_warning = true;
					return;
				}

				//We update the local list with the total balance of the database
				//Actualizam bilantul local al datei de baza
				balance_tot = db->get_total_balance();
			}

			//We update the total balance of the database
			//Actualizam bilantul total al datei de baza
			ImGui::Text((std::string(L("totalbalance")) + " " + std::to_string(balance_tot)).c_str());
		}
	}

	//The warning that the file is not open
	//Avertismentul ca fisierul nu este deschis
	void DrawNotInitializedWarning()
	{
		//If we don't have a warning, we return
		//Daca nu trebuie sa avertizam utilizatorul
		if (!m_warning)
			return;

		//Create the warning popup
		//Cream popup-ul
		ImGui::OpenPopup(L("warning.title"));
		m_warning = ImGui::BeginPopupModal(L("warning.title"), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		if (m_warning)
		{
			//Get the application icon and display it
			//Preluam imaginea aplicatiei
			auto image = Walnut::Application::Get().GetApplicationIcon();
			ImGui::Image(image->GetDescriptorSet(), { 48, 48 });

			//On the same line as the image, we shift the cursor to the right
			//Scriem pe aceeasi linie ca imaginea
			ImGui::SameLine();
			Walnut::UI::ShiftCursorX(20.0f);

			//We create the main text group
			//Cream grupul principal de text
			ImGui::BeginGroup();
			ImGui::Text(L("warning.nofile"));
			ImGui::EndGroup();

			//We create the text that tells the user to open a file
			//Cream butonul de inchidere
			if (Walnut::UI::ButtonCentered(L("button.close")))
			{
				//We set the warning to false so we can close it
				//Setam avertismentul la fals pentru a-l putea inchide
				m_warning = false;

				//If we have auto-update enabled, we set it to false so we don't enter an infinite loop and because it doesn't make sense if we don't have any file open
				//Setam auto-actualizarea la fals pentru a evita o bucla infinita si pentru ca nu are sens daca nu avem niciun fisier deschis
				auto_update = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	//The menu for the application information
	//Meniul de informatii al aplicatiei
	void UI_DrawAboutModal()
	{
		//If we don't have the about menu open, we return
		//Daca nu avem meniul de informatii deschis, returnam
		if (!m_AboutModalOpen)
			return;

		//We create the "Info" popup button, which is next to the application icon
		//Cream butonul de "Info", sus langa iconita
		ImGui::OpenPopup(L("about.title"));
		m_AboutModalOpen = ImGui::BeginPopupModal(L("about.title"), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		//We check if the menu is open
		//Verificam daca meniul este deschis
		if (m_AboutModalOpen)
		{
			//Get the image of the application and display it
			//Obtinem imaginea programului si o afisam
			auto image = Walnut::Application::Get().GetApplicationIcon();
			ImGui::Image(image->GetDescriptorSet(), { 48, 48 });

			//On the same line as the image, we shift the cursor to the right and display the text
			//Pe aceeasi linie, afisam textul
			ImGui::SameLine();
			Walnut::UI::ShiftCursorX(20.0f);

			ImGui::BeginGroup();
			ImGui::Text(L("about.appname"));
			ImGui::Text(L("about.author"));
			ImGui::EndGroup();

			//We create the close button
			//Cream butonul de inchidere
			if (Walnut::UI::ButtonCentered(L("button.close")))
			{
				m_AboutModalOpen = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	// Input validation helpers
	// Functii ajutatoare pentru validarea inputului

	// Validates a name (letters, spaces, hyphens, 2-64 chars)
	// Valideaza un nume (litere, spatii, cratime, 2-64 caractere)
	bool IsValidName(const std::string& name) const {
		return !name.empty() && name.length() <= 64 && std::regex_match(name, std::regex("^[A-Za-zăâîșțĂÂÎȘȚ -]{2,64}$"));
	}

	// Validates a currency (3 uppercase letters)
	// Valideaza o moneda (3 litere mari)
	bool IsValidCurrency(const std::string& curr) const {
		//return curr.length() == 3 && std::regex_match(curr, std::regex("^[A-Z]{3}$"));
		return std::regex_match(curr, std::regex("^[A-Za-zăâîșțĂÂÎȘȚ]{2,64}$"));
	}

	// Validates a balance (valid float, not empty, not too large)
	// Valideaza un bilant (float valid, nu gol, nu prea mare)
	bool IsValidBalance(const std::string& bal) const {
		if (bal.empty() || bal.length() > 32) return false;
		char* endptr = nullptr;
		double val = strtod(bal.c_str(), &endptr);
		return endptr == bal.c_str() + bal.size() && std::isfinite(val);
	}

	// Validates a file name (no forbidden chars, not empty, not too long)
	// Valideaza un nume de fisier (fara caractere interzise, nu gol, nu prea lung)
	bool IsValidFileName(const std::string& fname) const {
		if (fname.empty() || fname.length() > 128) return false;
		if (fname.find_first_of("\\/:*?\"<>|") != std::string::npos) return false;
		if (fname == "." || fname == "..") return false;
		return true;
	}

private:
	bool m_AboutModalOpen = false; // Meniul de informatii despre program / About menu of the program
	bool m_warning = false; // Avertismentul ca nu avem niciun fisier deschis / Warning that we don't have any file open
	bool auto_update = false; // Auto actualizarea tabelelor / Auto update of the tables

	//Variable for the time when the buttons were pressed
	//Timpul cand afisam textul, il folosim ca dupa o secunda sa nu se mai afiseze
	std::chrono::time_point<std::chrono::system_clock> pressed_rem;
	std::chrono::time_point<std::chrono::system_clock> pressed_add;

	//Variable for the case when we show the text if the client was added or not
	//Variable pentru cazul in care afisam textul daca clientul a putut fi adaugat sau nu
	bool show_clientadd_succ = false;
	bool show_clientadd_fail = false;
	bool show_clientadd_invalid = false;

	//Varaibles for the case when we show the text if the client was removed or not
	//Variable pentru cazul in care afisam textul daca clientul a putut fi sters sau nu
	bool show_clientrem_succ = false;
	bool show_clientrem_fail = false;
	bool show_clientrem_invalid = false;

	//If the file name is invalid
	//Daca numele fisierului este invalid
	bool show_file_invalid = false;

	//If we should save the data automatically when closing the files or exiting the program
	//Daca sa se salveze datele automat in fisiere la inchiderea lor / iesirea din program
	bool auto_save = true;

	//Pointer to the database data
	//Pointerul la data de baze
	Database* db = nullptr;

	//The data of the bank that is updated locally
	//Datele bancii care se actualizeaza local
	std::vector<Database::Client> normal;
	std::vector<std::vector<Database::Client>> sorted;
	std::vector<Database::Client> RON_Clients;
	std::vector<Database::Client> richest;
	long double balance_tot = 0.0f;

	//The local data for the creation and deletion of a client
	//Datele locale pentru crearea si stergerea unui cont
	std::string tmp_name_add, tmp_surname_add, tmp_name_rem, tmp_surname_rem, tmp_currency, tmp_balance;

	//The name of the file from which we read the data last time
	//Numele fisierului din care s-a citit ultima oara
	std::string tmp_fname;
};

//Main function of the interface
//Functia main a interfetei
inline Walnut::Application* Walnut::CreateApplication(int argc, char** argv) {
	//We create the application specification
	//Cream specificul aplicatiei
	Walnut::ApplicationSpecification spec;
	spec.Name = "Omni Bank Manager";
	spec.CustomTitlebar = true;

	// Load default language (English)
	// Incarcam limba implicita (engleza)
	g_Localization.SetLanguage("en");

	//We declare the application based on the specification
	//Declaram aplicatia in sine, pe baza specificului
	Walnut::Application* app = new Walnut::Application(spec);

	//We create and and add the main layer / container of the application
	//Cream si adaugam layer-ul / container-ul principal
	std::shared_ptr<OmniDBLayer> exampleLayer = std::make_shared<OmniDBLayer>();
	app->PushLayer(exampleLayer);

	//We add a callback for the topbar information menu
	//Adaugam un callback pe bara de sus pentru meniul de informatii
	app->SetMenubarCallback([app, exampleLayer]()
		{
			//We add the "Info" button next to the icon at the top
			//Adaugam butonul de "Info" langa iconita de sus
			if (ImGui::BeginMenu(L("menu.info")))
			{
				//We add the option to open the "About" menu
				//Adaugam obtiunea de About pentru informatii
				if (ImGui::MenuItem(L("menu.about")))
					//We print information about the program
					//Afisam informatiile despre program
					exampleLayer->ShowAboutModal();

				ImGui::EndMenu();
			}
		});

	return app;
}

//Main function of the interface
//Functia main a interfetei
inline int UInterfaceMain(int argc, char** argv) {
	//We call the main function of the Walnut framework, which subsequently calls the CreateApplication function implemented by us
	//Apelam functia main a frameworkului, care subecvent apeleaza functia CreateApplication implementata de noi
	return WalnutMain(argc, argv);
}