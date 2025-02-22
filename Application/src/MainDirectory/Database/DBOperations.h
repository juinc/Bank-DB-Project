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
	//Apeleaza mai departe read_data (adica citeste datele din f_name). Daca auto_save este setat sa fie adevarat, atunci cand destructorul se apeleaza datele se vor salva in fisierul de unde s-a produs citirea
	Database(const std::string& f_name, bool auto_save);
	//Apeleaza save_data daca autosave e setat sa fie adevarat, si se salveaza in acelasi fisier de unde s-au citit datele
	~Database();

	//Citeste datele dintr-un fisier cu formatul dat
	bool read_data(const std::string& f_name = "");
	//Salveaza datele intr-un fisier cu formatul dat
	bool save_data(const std::string& f_name = "") const;

	//Adauga un client la data de baze
	bool add_client(const std::string& name, const std::string& surname, const std::string& currency, const long double& balance);
	//Sterge un client din data de baze
	bool remove_client(const std::string& name, const std::string& surname);

	//Returneaza lista cu clienti sub forma de vector
	std::vector<Client> get_client_list() const
	{
		std::vector<Client> clnts;
		for (auto key_pair : Clients)
			clnts.push_back(key_pair.second);
		return clnts;
	}
	//Returneaza lista cu clienti sortata dupa moneda si apoi nume sub forma de vector bidimensional (liinile reprezinta moneda, iar coloanele clientul sub forma ordonata)
	std::vector<std::vector<Client>> get_client_list_sorted() const;
	//Returneaza lista de clienti cu soldul maxim pentru fiecare moneda sub forma de vector
	std::vector<Client> get_client_list_max_curr() const;
	//Returneaza soldul total al bancii
	const long double& get_total_balance() const { return total_balance; }

	//Returneaza true daca citirea a fost facuta
	const bool& is_initialized() const { return initialized; }

	//Actualizam proprietatea de salvare automata a fisierelor
	void set_auto_save(bool val) { auto_save = val; }
	const bool& get_auto_save() { return auto_save; }

	//Obtinem fisierul din care facem citirea
	const std::string& get_curr_file() { return read_dir; }
private:
	//Functie care face ca doua stringuri sa aiba complet litere mici si apoi le combina. Se utilizeaza pentru a salva clientii bancii in hash_map, accelerand stergerea
	static std::string hash(std::string s1, std::string s2);
	//Functie care face ca un string sa fie rescris cu majuscule, ajuta la functiile corelate cu lucrul cu moneda etc.
	static std::string capitalize(std::string s);
private:
	//Lista de clienti
	std::unordered_map<std::string,Client> Clients;

	//Hash mapul in care tinem minte moneda si hashul clientilor care detin acea moneda
	std::map<std::string, std::vector<std::string>> Currency_Map;

	//Numarul contului cel mai mic disponibil pentru utilizare
	int curr_acc_count = 1;
	//Bilantul total al bancii
	long double total_balance = 0.0f;
	//Fisierul din care s-a facut citirea
	std::string read_dir;

	//Daca data de baze a fost contruita / initializata corect
	bool initialized = false;
	//Daca atunci cand se distruge data de baze, datele sa fie salvate 
	bool auto_save = false;
};