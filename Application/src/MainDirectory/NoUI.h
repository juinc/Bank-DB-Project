#pragma once

#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include "Database/DBOperations.h"
#include "Localization/Localization.h"

namespace NoUI_Helper
{
	// The file where we write the results of the operations
	// Fisierul in care se scriu rezultatele operatiilor
	std::ofstream fout("rezultate.out");

	// The global localization object (default: English)
	// Obiect global de localizare (implicit: engleza)
	static Localization loc;

	// The function that helps us format the text, it chooses how much space to leave between words
	// Functie care ne ajuta sa formatam textul, alege cat de mult spatiu sa lasam intre cuvinte
	inline std::string select_tab(const std::string& s)
	{
		return (s.size() >= 8) ? "\t" : "\t\t";
	}

	// The function that prints a string to the console and to the file
	// Functie care afiseaza un string la tastatura si in fisier (o avem pentru a restrange codul si a-l face mai mic)
	inline void print(const std::string& cnt) {
		fout << cnt;
		std::cout << cnt;
	}

	// The function that prints a client to the console and to the file
	// Functie care afiseaza un client la tastatura si in fisier (o avem pentru a restrange codul si a-l face mai mic / lizibil)
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

	// Helper function to clear input buffer
	// Functie ajutatoare pentru a curata bufferul de intrare
	inline void clear_input() {
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	}

	// Function that displays the menu for reading and writing data
	// Functia de citire / scriere a datelor
	static void afis_meniu_date(Database& db)
	{
		short opt;

		system("cls");
		std::cout << "\t\t ***" << loc.Get("menu.fileops") << "***\n"; // NEW: menu.fileops
		std::cout << "1. " << loc.Get("button.read") << "\n";
		std::cout << "2. " << loc.Get("button.save") << "\n";
		std::cout << "3. " << loc.Get("button.newdb") << "\n"; // NEW: button.newdb
		std::cout << "4. " << loc.Get("currentfile") << "\n";
		std::cout << "\n" << loc.Get("prompt.command") << " "; // NEW: prompt.command

		while (!(std::cin >> opt) || opt < 1 || opt > 4) {
			std::cout << loc.Get("error.invalidinput") << "\n";
			clear_input();
		}
		clear_input();

		std::string f_name;

		switch (opt)
		{
		case 1:
			std::cout << loc.Get("prompt.filename") << ": ";
			std::getline(std::cin, f_name);

			if (db.read_data(f_name))
				std::cout << loc.Get("success.read") << "\n"; // NEW: success.read
			else
				std::cout << loc.Get("error.read") << "\n"; // NEW: error.read

			break;
		case 2:
			std::cout << loc.Get("prompt.savefile") << ": ";
			std::getline(std::cin, f_name);

			if (db.is_initialized())
				if (db.save_data(f_name)) {
					std::cout << loc.Get("success.save") << "\n"; // NEW: success.save
				}
				else {
					std::cout << loc.Get("error.save") << "\n"; // NEW: error.save
				}
			break;
		case 3:
			db.read_data();
			std::cout << loc.Get("success.newdb") << "\n"; // NEW: success.newdb
			break;
		case 4:
			std::cout << loc.Get("currentfile") << " '" << db.get_curr_file() << "' \n";
			break;
		}

		system("Pause");
	}

	// Function that displays the list of clients
	// Functie care afiseaza lista de clienti
	static void Afis_Lista_Clienti(const Database& db)
	{
		print("\n");
		print("\t\t\t\t\t " + loc.Get("header.clientlist") + " \n");
		print(loc.Get("table.name") + "\t\t" + loc.Get("table.surname") + "\t\t" +
			loc.Get("table.accnr") + "\t\t" + loc.Get("table.currency") + "\t\t" +
			loc.Get("table.balance") + "\t\t" + loc.Get("table.lastop") + "\t\t\n");

		for (const auto& client : db.get_client_list())
			print(client);

		system("Pause");
	}

	// Function that displays the list of clients who have accounts in RON
	// Functie care afiseaza lista de clienti care au cont in lei
	static void Afis_Lista_Clienti_RON(const Database& db)
	{
		print("\n");
		print("\t\t\t\t " + loc.Get("header.clientlist.ron") + " \n"); // NEW: header.clientlist.ron
		print(loc.Get("table.name") + "\t\t" + loc.Get("table.surname") + "\t\t" +
			loc.Get("table.accnr") + "\t\t" + loc.Get("table.currency") + "\t\t" +
			loc.Get("table.balance") + "\t\t" + loc.Get("table.lastop") + "\t\t\n");

		// For each client we check if their currency is RON
		// Pentru fiecare client verificam daca moneda lui este in RON
		for (const auto& client : db.get_client_list())
		{
			if (client.currency != "RON")
				continue;
			print(client);
		}
		system("Pause");
	}

	// Function that displays the sorted list of clients, first by currency and then by name
	// Functie care afiseaza lista sortata de clienti, dupa moneda si apoi nume
	static void Afis_Lista_Clienti_Sorted(const Database& db)
	{
		print("\n");
		print("\t\t\t " + loc.Get("header.orderedlist") + " \n");
		print(loc.Get("table.name") + "\t\t" + loc.Get("table.surname") + "\t\t" +
			loc.Get("table.accnr") + "\t\t" + loc.Get("table.currency") + "\t\t" +
			loc.Get("table.balance") + "\t\t" + loc.Get("table.lastop") + "\t\t\n");

		// We print each client from the bidimensional vector
		// Afisam fiecare client din vectorul bidimensional
		for (const auto& list : db.get_client_list_sorted())
			for (const auto& client : list)
				print(client);
		system("Pause");
	}

	// Function that displays the menu for deleting and adding clients
	// Functie care sterge un client
	static void Sterge_Client(Database& db)
	{
		std::string name, surname;
		std::cout << loc.Get("input.delname") << ": ";
		std::getline(std::cin, name);

		std::cout << loc.Get("input.delsurname") << ": ";
		std::getline(std::cin, surname);

		if (name.empty() || surname.empty()) {
			print("\n" + loc.Get("error.invalidname") + "\n");
			system("Pause");
			return;
		}

		if (!db.remove_client(name, surname)) {
			print("\n" + loc.Get("error.deletefail") + "\n");
			system("Pause");
		}
		else {
			print("\n" + loc.Get("success.clientdeleted") + "\n");
			system("Pause");
		}
	}

	// Function that adds a client to the database
	// Functie care adauga un client
	static void Adauga_Client(Database& db)
	{
		std::string name, surname, currency, tmp;
		long double balance = 0.0;

		std::cout << loc.Get("input.addname") << ": ";
		std::getline(std::cin, name);

		std::cout << loc.Get("input.addsurname") << ": ";
		std::getline(std::cin, surname);

		std::cout << loc.Get("input.addcurrency") << ": ";
		std::getline(std::cin, currency);

		std::cout << loc.Get("input.addbalance") << ": ";
		std::getline(std::cin, tmp);

		try {
			balance = std::stold(tmp);
		}
		catch (...) {
			print("\n" + loc.Get("error.invalidinput") + "\n");
			system("Pause");
			return;
		}

		if (name.empty() || surname.empty() || currency.empty()) {
			print("\n" + loc.Get("error.invalidinput") + "\n");
			system("Pause");
			return;
		}

		if (db.add_client(name, surname, currency, balance))
		{
			print("\n" + loc.Get("success.clientadded") + "\n");
			system("Pause");
		}
		else
		{
			print("\n" + loc.Get("error.addfail") + "\n");
			system("Pause");
		}
	}

	// Function that displays the clients with the maximum balance for each currency
	// Functie care afiseaza persoanele cu soldul maxim pentru fiecare moneda
	static void Afis_Suma_Max_Mond(const Database& db)
	{
		print("\n");
		print("\t\t\t " + loc.Get("header.richestclients") + " \n");
		print(loc.Get("table.name") + "\t\t" + loc.Get("table.surname") + "\t\t" +
			loc.Get("table.accnr") + "\t\t" + loc.Get("table.currency") + "\t\t" +
			loc.Get("table.balance") + "\t\t" + loc.Get("table.lastop") + "\t\t\n");

		for (const auto& client : db.get_client_list_max_curr())
			print(client);
		system("Pause");
	}

	// Function that displays the settings menu
	// Functie care afiseaza meniul de setari
	static void Afis_Meniu_Setari(Database& db)
	{
		short opt;

		system("cls");
		std::cout << "\t\t ***" << loc.Get("menu.settings") << "***\n";
		std::cout << "1. [" << (db.is_initialized() && db.get_auto_save() ? "x" : "") << "]\t " << loc.Get("settings.autosave") << "\n";
		std::cout << "2. " << loc.Get("menu.language") << "\n";
		std::cout << "3. " << loc.Get("button.close") << "\n";
		std::cout << loc.Get("prompt.command") << " "; // NEW: prompt.command

		while (!(std::cin >> opt) || opt < 1 || opt > 3) {
			std::cout << loc.Get("error.invalidinput") << "\n";
			clear_input();
		}
		clear_input();

		switch (opt)
		{
		case 1:
			db.set_auto_save(!db.get_auto_save());
			std::cout << loc.Get("settings.autosave") << " " << (db.get_auto_save() ? loc.Get("value.true") : loc.Get("value.false")) << "\n"; // NEW: value.true, value.false
			break;
		case 2:
		{
			std::cout << loc.Get("menu.language") << " (en/ro): ";
			std::string lang;
			std::getline(std::cin, lang);
			if (lang != "en" && lang != "ro") {
				std::cout << loc.Get("error.invalidinput") << "\n";
			}
			else {
				loc.SetLanguage(lang);
				std::cout << loc.Get("success.langchange") << "\n"; // NEW: success.langchange
			}
			break;
		}
		case 3:
			break;
		}

		system("Pause");
	}

	// Function that displays the total balance of the bank
	// Functie care afiseaza soldul total al bancii
	inline void Afis_Sold_Total(const Database& db)
	{
		print("\n" + loc.Get("totalbalance") + " ");
		print(std::to_string(db.get_total_balance()));
		print("\n");
		system("Pause");
	}
}

// Main function for the NoUI console application
// Functia principala pentru aplicatia consola NoUI
static int NoUIMain(int argc, char** argv)
{
	// We set the precision to 20 to see the numbers as specific as possible
	// Setam precizia la 20 ca sa vedem numerle cat mai specifice
	std::cout << std::setprecision(20);

	short optiune;
	Database db("", true);

	do
	{
		system("cls");

		/// We print the menu on the screen
		/// afisare meniu pe ecran
		std::cout << '\n' << '\n' << "\t\t****" << NoUI_Helper::loc.Get("menu.main") << "****" << '\n'; // NEW: menu.main
		std::cout << '\n' << "1. " << NoUI_Helper::loc.Get("menu.fileops");
		std::cout << '\n' << "2. " << NoUI_Helper::loc.Get("button.showclients");
		std::cout << '\n' << "3. " << NoUI_Helper::loc.Get("button.showron"); // NEW: button.showron
		std::cout << '\n' << "4. " << NoUI_Helper::loc.Get("button.showordered");
		std::cout << '\n' << "5. " << NoUI_Helper::loc.Get("header.deleteclient");
		std::cout << '\n' << "6. " << NoUI_Helper::loc.Get("header.addclient");
		std::cout << '\n' << "7. " << NoUI_Helper::loc.Get("button.showrichest");
		std::cout << '\n' << "8. " << NoUI_Helper::loc.Get("header.totalbalance");
		std::cout << '\n' << "9. " << NoUI_Helper::loc.Get("menu.settings");
		std::cout << '\n' << "10. " << NoUI_Helper::loc.Get("team.header");
		std::cout << '\n' << "11. " << NoUI_Helper::loc.Get("menu.quit");

		/// We read the option from the user
		/// citire operatie dorita
		std::cout << '\n' << "\t\t" << NoUI_Helper::loc.Get("prompt.command") << " ";

		while (!(std::cin >> optiune) || optiune < 1 || optiune > 11) {
			std::cout << NoUI_Helper::loc.Get("error.invalidinput") << "\n";
			NoUI_Helper::clear_input();
		}
		NoUI_Helper::clear_input();

		/// We Execute the desired operation
		/// executie operatie dorita
		switch (optiune)
		{
		case 1: NoUI_Helper::afis_meniu_date(db); break;
		case 2: NoUI_Helper::Afis_Lista_Clienti(db); break;
		case 3: NoUI_Helper::Afis_Lista_Clienti_RON(db); break;
		case 4: NoUI_Helper::Afis_Lista_Clienti_Sorted(db); break;
		case 5: NoUI_Helper::Sterge_Client(db); break;
		case 6: NoUI_Helper::Adauga_Client(db); break;
		case 7: NoUI_Helper::Afis_Suma_Max_Mond(db); break;
		case 8: NoUI_Helper::Afis_Sold_Total(db); break;
		case 9: NoUI_Helper::Afis_Meniu_Setari(db); break;
		case 10: NoUI_Helper::print("\n" + NoUI_Helper::loc.Get("team.members") + "\n"); system("Pause"); break;
		case 11:
			// We close the file
			// Inchidem fisierul
			NoUI_Helper::fout.close();
			return 0;
		default: std::cout << NoUI_Helper::loc.Get("error.invalidinput");
		}
	} while (optiune != 11);

	// Inchidem fisierul
	// We close the file
	NoUI_Helper::fout.close();

	return 0;
}