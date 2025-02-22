#pragma once

#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "Database/DBOperations.h"

namespace NoUI_Helper
{
	//Fisierul in care se scriu rezultatele operatiilor
	std::ofstream fout("rezultate.out");

	//Functie car ene ajuta sa formatam textul, alege cat de mult spatiu sa lasam intre cuvinte
	inline std::string select_tab(std::string s)
	{
		return  (s.size() >= 8) ? "\t" : "\t\t";
	}
	//Functie care afiseaza un string la tastatura si in fisier (o avem pentru a restrange codul si a-l face mai mic)
	inline void print(std::string cnt) {
		fout << cnt;
		std::cout << cnt;
	}
	//Functie care afiseaza un client la tastatura si in fisier (o avem pentru a restrange codul si a-l face mai mic / lizibil)
	static void print(const Database::Client& clnt)
	{
		fout << clnt.name << select_tab(clnt.name);
		fout << clnt.surname << select_tab(clnt.surname);
		fout << clnt.acc_nr << select_tab(std::to_string(clnt.acc_nr));
		fout << clnt.currency << select_tab(clnt.currency);
		fout << clnt.balance << select_tab(std::to_string(clnt.balance));
		fout << clnt.last_op << '\n';

		std::cout << clnt.name << select_tab(clnt.name);
		std::cout << clnt.surname << select_tab(clnt.surname);
		std::cout << clnt.acc_nr << select_tab(std::to_string(clnt.acc_nr));
		std::cout << clnt.currency << select_tab(clnt.currency);
		std::cout << clnt.balance << select_tab(std::to_string(clnt.balance));
		std::cout << clnt.last_op << '\n';
	}
	//Functia de citire / scriere a datelor
	static void afis_meniu_date(Database& db)
	{
		short opt;

		system("cls");
		std::cout << "\t\t ***MENIU MANIPULARE DATE***\n";
		std::cout << "1. Citire date\n";
		std::cout << "2. Salvare date\n";
		std::cout << "3. Creare data de baze goala\n";
		std::cout << "4. Afisare fisier de lucru curent (gol daca este o data de baze noua)\n";
		std::cout << "\nIntroduceti comanda: ";

		std::cin >> opt;

		std::string f_name;

		switch(opt)
		{
		case 1:
			std::cout << "Introduceti numele fisierului din care se vor citi datele: ";
			std::cin >> f_name;

			if(db.read_data(f_name))
				std::cout << "Citire facuta cu succes!\n";
			else
				std::cout << "A aparut o eroare la citire (posibil ca fisierul sa nu existe)! S-a creat o data de baze goala in schimb.\n";

			break;
		case 2:
			std::cout << "Introduceti numele fisierului din care se vor salva datele: ";
			std::cin >> f_name;

			if(db.is_initialized())
				if (db.save_data(f_name)) {
					std::cout << "Salvare facuta cu succes!\n";
				}else{
					std::cout << "A aparut o eroare la salvare!\n";
				}
				
			break;
		case 3:
			db.read_data();
			std::cout << "Data de baze goala creata cu succes!\n";
			break;
		case 4:
			std::cout << "Fisierul de lucru curent este: " << " '" << db.get_curr_file() << "' \n";
			break;
		}

		system("Pause");
	}

	//Functie care afiseaza lista de clienti
	static void Afis_Lista_Clienti(const Database& db)
	{
		print("\n");
		print("\t\t\t\t\t Lista Conturilor Bancii \n");
		print("Nume\t\tPrenume\t\tNr Cont\t\tMoneda\t\tSold\t\tUltima Operatie\t\t\n");

		for (const auto& client : db.get_client_list())
			print(client);

		system("Pause");
	}

	//Functie care afiseaza lista de clienti care au cont in lei
	static void Afis_Lista_Clienti_RON(const Database& db)
	{
		print("\n");
		print("\t\t\t\t Lista Conturilor Bancii in lei / RON \n");
		print("Nume\t\tPrenume\t\tNr Cont\t\tMoneda\t\tSold\t\tUltima Operatie\t\t\n");

		//Pentru fiecare client verificam daca moneda lui este in RON
		for (const auto& client : db.get_client_list())
		{
			if (client.currency != "RON")
				continue;
			print(client);
		}
		system("Pause");
	}

	//Functie care afiseaza lista sortata de clienti, dupa moneda si apoi nume
	static void Afis_Lista_Clienti_Sorted(const Database& db)
	{
		print("\n");
		print("\t\t\t Lista Conturilor Bancii sortate dupa moneda si apoi nume \n");
		print("Nume\t\tPrenume\t\tNr Cont\t\tMoneda\t\tSold\t\tUltima Operatie\t\t\n");

		//Afisam fiecare client din vectorul bidimensional
		for (const auto& list : db.get_client_list_sorted())
			for (const auto& client : list)
				print(client);
		system("Pause");
	}

	//Functie care sterge un client
	static void Sterge_Client(Database& db)
	{
		std::string name, surname;
		std::cout << "Introduceti Numele Clientului: ";
		std::cin >> name;

		std::cout << "Introduceti Prenumele Clientului: ";
		std::cin >> surname;

		if (!db.remove_client(name, surname)) {
			print("\nClientul nu a fost gasit sau s-a produs o eroare!\n");
			system("Pause");
		}
		else {
			print("\nClientul a fost sters cu succes!\n");
			system("Pause");
		}
	}

	//Functie care adauga un client
	static void Adauga_Client(Database& db)
	{
		std::string name, surname, currency, tmp;
		long double balance;

		std::cout << "Introduceti Numele Clientului: ";
		std::cin >> name;

		std::cout << "Introduceti Prenumele Clientului: ";
		std::cin >> surname;

		std::cout << "Introduceti Moneda Clientului: ";
		std::cin >> currency;

		std::cout << "Introduceti Soldul Clientului: ";
		std::cin >> tmp;

		balance = std::stof(tmp);

		if (db.add_client(name, surname, currency, balance))
		{
			print("\nCont creat cu succes! \n");
			system("Pause");
		}
		else
		{
			print("\nS-a produs o eroare in crearea contului! \n");
			system("Pause");
		}


	}

	//Functie care afiseaza persoanele cu soldul maxim pentru fiecare moneda
	static void Afis_Suma_Max_Mond(const Database& db)
	{
		print("\n");
		print("\t\t\t Lista Conturilor Bancii cu sold maxim pentru fiecare moneda \n");
		print("Nume\t\tPrenume\t\tNr Cont\t\tMoneda\t\tSold\t\tUltima Operatie\t\t\n");

		for (const auto& client : db.get_client_list_max_curr())
			print(client);
		system("Pause");
	}

	static void Afis_Meniu_Setari(Database& db)
	{
		short opt;

		system("cls");
		std::cout << "\t\t ***MENIU SETARI***\n";
		std::cout << "1. [" << (db.is_initialized() && db.get_auto_save() ? "x" : "") << "]\t Salvarea automata a datelor dupa deschiderea unui nou fisier / inchiderea programului.\n";
		std::cout << "2. Iesire\n";
		std::cout << "Selectati setarea pe care vreti sa o modificati (sau 2 pentru iesire): ";

		std::cin >> opt;

		switch(opt)
		{
		case 1:
			db.set_auto_save(!db.get_auto_save());
			std::cout << "Optiune schimbata cu succes! Noua ei valoare este " << (db.get_auto_save() ? "adevarata" : "falsa") << "\n";
			break;
		case 2:
			break;
		}

		system("Pause");
	}

	//Functie care afiseaza soldul total al bancii
	inline void Afis_Sold_Total(const Database& db)
	{
		print("\nSoldul total al bancii este: ");
		print(std::to_string(db.get_total_balance()));
		print("\n");
		system("Pause");
	}
}



static int NoUIMain(int argc, char** argv)
{
	//Setam precizia la 20 ca sa vedem numerle cat mai specifice
	std::setprecision(20);

	short optiune;
	Database db("",true);

	do
	{
		system("cls");

		///afisare meniu pe ecran
		std::cout << '\n' << '\n' << "\t\t****MENIU****" << '\n';
		std::cout << '\n' << "1. Meniu manipulare fisiere (citire/salvare) ";
		std::cout << '\n' << "2. Afisare Lista clientilor din depozit";
		std::cout << '\n' << "3. Afisare Lista clientilor cu cont in lei ";
		std::cout << '\n' << "4. Afisare ordonarea clientilor alfabetic dupa moneda + alfabetic dupa numele clientului. ";
		std::cout << '\n' << "5. Stergerea unui client ";
		std::cout << '\n' << "6. Adaugarea unui noui client ";
		std::cout << '\n' << "7. Afisarea clientului cu cel mai mare sold pentru fiecare moneda ";
		std::cout << '\n' << "8. Afisarea valorii totala a stocului bancii ";
		std::cout << '\n' << "9. Setari ";
		std::cout << '\n' << "10. Membrii echipei ";
		std::cout << '\n' << "11. Iesire ";
		///citire operatie dorita
		std::cout << '\n' << "\t\tAlege operatia: ";
		std::cin >> optiune;
		///executie operatie dorita
		switch (optiune)
		{
			case 1: NoUI_Helper::afis_meniu_date(db); break; //Cand facem citirea, cream si data de baze
			case 2: NoUI_Helper::Afis_Lista_Clienti(db); break; //Afisam lista de clienti
			case 3: NoUI_Helper::Afis_Lista_Clienti_RON(db); break; //Afisam lista de clienti in lei
			case 4: NoUI_Helper::Afis_Lista_Clienti_Sorted(db); break; //Afisam lista de clienti sortata dupa moneda si apoi nume
			case 5: NoUI_Helper::Sterge_Client(db); break; //Stergem un client
			case 6: NoUI_Helper::Adauga_Client(db); break; //Adaugam un client
			case 7: NoUI_Helper::Afis_Suma_Max_Mond(db); break; //Afisam angatii cu soldul maxim pentru fiecare moneda
			case 8: NoUI_Helper::Afis_Sold_Total(db); break; //Afisam soldul total al bancii
			case 9: NoUI_Helper::Afis_Meniu_Setari(db); break;
			case 10: NoUI_Helper::print("\nProgram realizat de juinc\n"); system("Pause"); break;
			case 11:
				//Inchidem fisierul
				NoUI_Helper::fout.close();
				return 0;
			default: std::cout << "Optiune inexistenta!";
		}
	} while (optiune != 10);

	//Inchidem fisierul
	NoUI_Helper::fout.close();

	return 0;
}