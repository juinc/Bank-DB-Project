#pragma once
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>

class Database
{
public:
	struct Client
	{
		std::string name;
		std::string surname;
		int acc_nr;
		std::string currency;
		long double balance;
		std::string last_op;
	};
public:
	//Calls read_data further (aka, it reads the data from f_name). If auto_save is set to true, when the destructor is called, the data will be saved in the file from which it was read
	//Apeleaza mai departe read_data (adica citeste datele din f_name). Daca auto_save este setat sa fie adevarat, atunci cand destructorul se apeleaza datele se vor salva in fisierul de unde s-a produs citirea
	Database(const std::string& f_name, bool auto_save);

	//Calls save_data if auto_save is set to true, and saves in the same file from which the data was read
	//Apeleaza save_data daca autosave e setat sa fie adevarat, si se salveaza in acelasi fisier de unde s-au citit datele
	~Database();

	//Reads the data from a file with the given format
	//Citeste datele dintr-un fisier cu formatul dat
	bool read_data(const std::string& f_name = "");

	//Saves the data in a file with the given format
	//Salveaza datele intr-un fisier cu formatul dat
	bool save_data(const std::string& f_name = "") const;

	//Adds a client to the database
	//Adauga un client la data de baze
	bool add_client(const std::string& name, const std::string& surname, const std::string& currency, const long double& balance);

	//Deletes a client from the database
	//Sterge un client din data de baze
	bool remove_client(const std::string& name, const std::string& surname);

	//Returns the client list as a vector
	//Returneaza lista cu clienti sub forma de vector
	std::vector<Client> get_client_list() const
	{
		std::vector<Client> clnts;
		for (auto key_pair : Clients)
			clnts.push_back(key_pair.second);
		return clnts;
	}

	//Returns the client list sorted by currency and then by name as a two-dimensional vector (the rows represent the currency, and the columns the clients in sorted order)
	//Returneaza lista cu clienti sortata dupa moneda si apoi nume sub forma de vector bidimensional (liinile reprezinta moneda, iar coloanele clientul sub forma ordonata)
	std::vector<std::vector<Client>> get_client_list_sorted() const;

	//Returns the list of clients with the maximum balance for each currency as a vector
	//Returneaza lista de clienti cu soldul maxim pentru fiecare moneda sub forma de vector
	std::vector<Client> get_client_list_max_curr() const;

	//Returns the total balance of the bank
	//Returneaza soldul total al bancii
	const long double& get_total_balance() const { return total_balance; }

	//Retunrs true if the database was initialized correctly (aka, the data was read from a file)
	//Returneaza true daca citirea a fost facuta
	const bool& is_initialized() const { return initialized; }

	//We update the property that tells if the data should be saved automatically
	//Actualizam proprietatea de salvare automata a fisierelor
	void set_auto_save(bool val) { auto_save = val; }
	const bool& get_auto_save() { return auto_save; }

	//We obtain the file from which we read the data
	//Obtinem fisierul din care facem citirea
	const std::string& get_curr_file() { return read_dir; }
private:
	//Function that makes two strings to be completely lowercase and then combines them. It is used to save the bank clients in a hash_map, speeding up deletion
	//Functie care face ca doua stringuri sa aiba complet litere mici si apoi le combina. Se utilizeaza pentru a salva clientii bancii in hash_map, accelerand stergerea
	static std::string hash(std::string s1, std::string s2);

	//Function that makes a string to be completely lowercase, used for the hash function
	//Functie care face ca un string sa fie rescris cu majuscule, ajuta la functiile corelate cu lucrul cu moneda etc.
	static std::string capitalize(std::string s);
private:
	//List of clients
	//Lista de clienti
	std::unordered_map<std::string,Client> Clients;

	//The hash map that contains the currency and the hashes of the clients that own that currency
	//Hash mapul in care tinem minte moneda si hashul clientilor care detin acea moneda
	std::map<std::string, std::vector<std::string>> Currency_Map;

	//The number of the current account, used to generate new accounts
	//Numarul contului cel mai mic disponibil pentru utilizare
	int curr_acc_count = 1;

	//The total balance of the bank
	//Bilantul total al bancii
	long double total_balance = 0.0f;

	//The file from which the data was read
	//Fisierul din care s-a facut citirea
	std::string read_dir;

	//If the database was initialized correctly (aka, the data was read from a file)
	//Daca data de baze a fost contruita / initializata corect
	bool initialized = false;

	//If when the database is destroyed, the data should be saved
	//Daca atunci cand se distruge data de baze, datele sa fie salvate 
	bool auto_save = false;
};