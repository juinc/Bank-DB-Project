#include "DBOperations.h"

/*
Constructorul si destructorul se ocupa cu citirea si respectiv salvarea datelor din data de baze

The constructor and destructor is tasked with reading and saving the data from the database
*/
Database::Database(const std::string& f_name, const bool auto_save) : auto_save(auto_save){
	read_data(f_name);
}
Database::~Database()
{
	if (auto_save)
		save_data();
}

/*
Ce este: Functia citeste datele dintr-un fisier si initializeaza variabilele datei de baze pe baza lor (nume de client, prenume etc)
Rostul ei: Ajuta in a pastra codul lizibil si face ca aceasta clasa sa fie reutilizabila (daca citim dintr-un nou fisier, cel trecut se salveaza)
Functia va returna "true" daca s-a reusit citirea si "false" altfel
Format: "nume prenume nr_cont moneda sold ultima_operatie"

What is it: This function reads the data from a file and initializes the varaibles of the database based on their value (name, surname etc) 
Its purpose: Helps in keeping the code readable and the reutilization of this class (if we read from a new file, the old one gets saved, and not overriden)
The function will return true it was able to read the data and false otherwise
Format: "name surname acc_number currency balance last_operation"
*/
bool Database::read_data(const std::string& f_name)
{
	// If we already have the saved values of another database, we save it, reset the data and open the new one
	// Daca deja aveam salvate valorile unei alte date de baze, o salvam, resetam datele si o deschidem pe cea noua
	if(initialized)
	{
		if(auto_save)
			save_data();
		initialized = false;

		read_dir.clear();

		Clients.clear();
		Currency_Map.clear();

		curr_acc_count = 1;
		total_balance = 0.0f;
	}

	// If no file name was given, we create a new empty database, so we set all the data to empty
	// Daca nu ni s-a dat un fisier inseamna ca facem o data de baze complet de la 0, asa ca setam toate datele goale
	if(f_name.empty())
	{
		Clients = {};
		Currency_Map = {};

		read_dir = "";

		initialized = true;

		return true;
	}

	//We open the file with the given name
	//Deschidem fisierul
	std::ifstream fin(f_name);

	//If an error occurred while opening the file, we return false (we could not read it), but we create a new empty database
	//This is to allow the user to work with files that do not exist, and they will be created after he saves the data
	//Daca s-a produs o eroare la deschidere, returnam false (nu s-a putut face citirea), dar cream o data de baze noua
	//Asta este ca sa ii permitem utilizatorului sa lucreze cu fisiere care nu exista, si sa fie create dupa ce el salveaza datele
	if (!fin.is_open())
	{
		Clients = {};
		Currency_Map = {};

		read_dir = "";

		initialized = true;

		return false;
	}

	//For every account in the file
	//Pentru fiecare cont din fisier
	while (!fin.eof())
	{
		//We read the client data
		//Citim datele clientului 
		Client client_curr;
		std::string temp;

		fin >> client_curr.name;

		//If the name is empty, we stop reading (we reached an empty line)
		//Daca numele este nul / am ajuns la o linie goala, ne oprim
		if (client_curr.name.empty())
			break;

		fin >> client_curr.surname;

		fin >> temp;
		client_curr.acc_nr = std::stoi(temp);

		fin >> client_curr.currency;

		fin >> temp;
		client_curr.balance = std::stof(temp);

		fin >> client_curr.last_op;

		//We update the total balance of the bank and the current account count
		//Actualizam soldul / bilantul total al bancii la citire, pentru optimizare
		total_balance += client_curr.balance;

		//We update the current account count to the maximum account number read
		//Actualizam la fel numarul de client current la citire pentru optimizare
		if (client_curr.acc_nr >= curr_acc_count)
			curr_acc_count = client_curr.acc_nr + 1;

		//We add a new client, associating the account with the hash of its name
		//Adaugam noul client, ii asociem contul cu hash-ul specific numelui sau
		Clients[hash(client_curr.name, client_curr.surname)] = client_curr;

		//To the currency's string in uppercase, we add the current account position
		//Adaugam, la stringul in majuscule al monedei, pozita contului curent
		Currency_Map[capitalize(client_curr.currency)].push_back(hash(client_curr.name,client_curr.surname));
	}

	//The database is ready to be used
	//Data de baza este gata de lucru
	initialized = true;

	//We set the file on which we are working on
	//Setam fisierul cu care lucram in prezent
	read_dir = f_name;

	//Close the file we read from
	//Inchidem fisierul din care am citit
	fin.close();

	//Return true because we were able to read the data
	//Returnam true pentru ca sa putu face citirea
	return true;
}

/*
Ce este: Functia returneaza lista de clienti sortata dupa moneda si apoi nume 
Rostul ei: Ceruta in enuntul proiectului, este aici pentru a usura utilizarea si abstractiza codul comun interfetei de consola cat si de Walnut.

What is it: Function returns the list of clients sorted by currency and afterwards name
Its purpose: It's necessary in the project's description, it is also here to ease the use and abstractization of the common code between the normal user interface and console interface
*/
std::vector<std::vector<Database::Client>> Database::get_client_list_sorted() const
{
	//If the database was not initialized, we return an empty vector to avoid errors
	//Daca datele din data de baze nu au fost citite, returnam imediat pentru a evita erori
	if (initialized == false)
		return {};

	//The vector that will contain all the clients sorted by currency and then name
	//Tabloul care va contine toti clientii sortati dupa moneda si nume
	std::vector<std::vector<Client>> sorted_clients;

	//Custom function for sorting the clients by name - could've been done in the sort function, but this way it is more readable
	//Functia customizata pentru sortare. Puteam sa o fac mai scurta si sa o adaug direct in functie, dar asa este mai lizibila.
	constexpr struct
	{
		bool operator()(const Client& a, const Client& b) const
		{
			int i = 0;

			//Pentru fiecare client le parcurgem numele pana nu mai sunt egale
			while (a.name[i] == b.name[i])
				i++;

			//Apoi returnam care litera are un cod ASCII mai mica (este inaintea celeilalte)
			return a.name[i] < b.name[i];
		}
	}
	customCond;

	// For each client list of each currency
	// Pentru fiecare lista cu clienti ai fiecarei monede 
	for (const auto& list : Currency_Map)
	{
		//We create a vector in which we will store the list of clients that we will sort
		//Cream un vector in care stocam lista de clienti pe care o vom sorta
		std::vector<Client> tmp_clients;

		//For every client of the currency, we add it to the vector that we will sort
		//Pentru fiecare client al monedei, il adaugam in vectorul pe care il vom sorta
		for (auto hash_key : list.second)
		{
			tmp_clients.push_back(Clients.at(hash_key));
		}

		//We sort the vector of clients by name
		//Sortam vectorul
		std::sort(tmp_clients.begin(), tmp_clients.end(), customCond);

		//We add the vector to the table with clients that we will return
		//Adaugam vectorul la tabloul cu cliente pe care il returnam
		sorted_clients.push_back(tmp_clients);
	}

	return sorted_clients;
}

/*
Ce este: Functia pentru a adauga un client in lista. Returneaza "true" daca sa putut realiza adaugarea si "false" altfel
Rostul ei: Ceruta in enuntul proiectului, este aici pentru a usura utilizarea si abstractiza codul comun interfetei de consola cat si de Walnut. 
Functia returneaza "true" daca operatia s-a finalizat cu succes si "false" daca a aparut o eroare

What is it: Function to add a client ot the list
Its purpose: It's necessary in the project's description, it is also here to ease the use and abstractization of the common code between the normal user interface and console interface
The function returns true if the operation has been completed successfully or false if there was an error. Returns true if the operation was successful and false otherwise.
*/
bool Database::add_client(const std::string& name, const std::string& surname, const std::string& currency, const long double& balance)
{
	//We create a new account of the client with the given data
	//Cream un nou cont de client cu datele furnizate
	Client curr_client;
	curr_client.name = name;
	curr_client.surname = surname;
	curr_client.currency = currency;
	curr_client.balance = balance;
	curr_client.acc_nr = curr_acc_count; // Numarul contului il setam la cel mai mic numar de cont liber al datei de baze / The number of the account is set to the smallest free account number of the database
	curr_client.last_op = "creare"; // Ultima operatie pe cont este efectiv crearea acestuia / The last operation on the account is the creation of it

	//We increment the account number available in the database
	//Incrementam numarul de cont valabil al datei de baze
	curr_acc_count++;

	//We add the new client to the list of clients
	//Adaugam noul client in lista de clienti
	Clients[hash(curr_client.name,curr_client.surname)] = curr_client;

	//We add the client to the category of its currency
	//Adaugam clientul la categoria monedei lui respective
	Currency_Map[capitalize(curr_client.currency)].push_back(hash(curr_client.name, curr_client.surname));

	//We increase the total balance of the bank with the current client's balance
	//Crestem bilantul total al bancii
	total_balance += curr_client.balance;

	//We have successfully added the client
	//Am adaugat clientul cu succes
	return true;
}

/*
Ce este: Functia pentru a sterge un client din lista, pe baza numelui si pronumelui. Returneaza "true" daca sa putut realiza stergerea si "false" altfel
Rostul ei: Ceruta in enuntul proiectului, este aici pentru a usura utilizarea si abstractiza codul comun interfetei de consola cat si de Walnut.
Functia returneaza "true" daca operatia s-a finalizat cu succes si "false" daca a aparut o eroare

What is it: Function to delete a client from the list, based on the name and surname. Returns true if the operation was successful and false otherwise.
Its purpose: It's necessary in the project's description, it is also here to ease the use and abstractization of the common code between the normal user interface and console interface
The function returns true if the operation has been completed successfully or false if there was an error 
*/
bool Database::remove_client(const std::string& name, const std::string& surname)
{
	//We get the hash of the client
	//Obtinem hash-ul clientului
	const auto hash_key = hash(name, surname);

	//We verify if the client exists in the database, and if not, it means that the deletion could not be done
	//Verificam daca avem un client cu astfel de nume in banca, iar daca nu asta inseamna ca stergerea nu a putut fi facuta
	if (!Clients.contains(hash_key))
		return false;

	//We get the currency of the client to delete, so we can remove it from the vector of currencies
	//Obtinem moneda clientului respectiv pentru a sterge, din vectorul cu monede, indicele contului curent
	auto curr = capitalize((Clients[hash(name,surname)]).currency);

	//We delete the account from the currency vector 
	//Stergem contul din vectorul cu monede hash-ul respectiv
	Currency_Map[curr].erase(std::find(Currency_Map[curr].begin(), Currency_Map[curr].end(), hash(name,surname)));

	//We delete the client from the lisst
	//Stergem clientul din lista
	Clients.erase(hash(name, surname));

	return true;
}

/*
Ce este: Functia pentru a returna clientii cu soldul maxim pentru moneda lor respectiva
Rostul ei: Ceruta in enuntul proiectului, este aici pentru a usura utilizarea si abstractiza codul comun interfetei de consola cat si de Walnut.

What is it: Function to return the clients with the maximum sold for their respective currency
Its purpose: It's necessary in the project's description, it is also here to ease the use and abstractization of the common code between the normal user interface and console interface
*/
std::vector<Database::Client> Database::get_client_list_max_curr() const
{
	//We create the vector in which we will store the accounts with the maximum balance
	//Cream vectorul in care vom tine minte conturile cu sold maxim
	std::vector<Client> maxs;

	//We go through all the vectors that contain the hashes of the clients for each currency
	//Parcurgem toti vectorii care contin hasurile clientilor pentru fiecare moneda
	for (const auto& list : Currency_Map)
	{
		//We verify if we have accounts for the respective currency
		//Verificam daca nu avem conturi pentru moneda respectiva
		if (list.second.empty())
			continue;

		//The hash of the client with the maximum balance
		//Hash-ul clientului cu bilantul maxim
		std::string clnt_max_hash;

		//We set the maximum balance to the minimum possible value, so that we can compare it with the first client
		//Setam bal_max la -INT_MAX fiindca valoarea soldului poate fi negativa dar nu va ajunge nicioadata chiar atat de mare
		long double bal_max = -INT_MAX;

		//For all of the clients in the list, we compare their balance with the maximum balance until that moment, and if it is greater, we update the maximum balance
		//Pentru toti clientii din lista comparam soldul lor cu soldul maxim pana la acel moment, si daca este mai mare actualizam soldul maxim
		for (auto clnt : list.second)
			if (bal_max < Clients.at(clnt).balance)
			{
				clnt_max_hash = clnt;
				bal_max = Clients.at(clnt).balance;
			}

		//We save the client with the maximum balance in the vector of maximums
		//Salvam clientul in vectorul cu maximi
		maxs.push_back(Clients.at(clnt_max_hash));
	}

	return maxs;
}

/*
Ce este: Functia pentru a salva datele formatate conform modului de citire al datei de baza intr-un fisier. Returneaza "true" daca s-a putut salva, si "false" altfel
Rostul ei: Se utilieaza pentru a salva noile actualizari. Nu am pus-o intr-un destructor fiindca acesta nu admite parametri.
Daca nu ii se da fisierul, ea va scrie automat din fisierul in care a citit.
Format: "nume prenume nr_cont moneda sold ultima_operatie"

What is it: Function to save the read data to a file, based on the specific database format. Returns true if it was able to save the data, and false otherwise.
Why is it here: This function is used to save the new updates. It hasn't been placed in the destructor because it doesn't allow parameters.
If the file isn't passed as an argument, it will use the file it read the data from.
Format: "name surname acc_number currency balance last_operation"
*/
bool Database::save_data(const std::string& f_name) const
{
	//If the database was not initialized, we cannot save anything because there is nothing to save
	//Daca data de baze nu a fost initializata, nu putem salva nimic pentru ca nu avem nimic de salvat
	if (!initialized)
		return true;

	//If we don't have a file to save to (either not passed as a parameter or not read from a file before), we cannot save anything
	//Daca nu avem niciun fisier in care sa salvam (nici dat prin parametru si nici salva de dinainte)
	if (f_name.empty() and read_dir.empty())
		return false;

	//We open either the file passed as a parameter or the file we read from first
	//Deschidem fie fisierul dat prin parametru sau fisierul din care am citit prima oara
	std::ofstream fout(f_name.empty() ? read_dir : f_name);

	//If the file could not be opened or there was an error, we cannot save anything
	//Daca fisierul nu s-a deschis sau a avut o eroare, nu se poate face salvarea
	if (!fout.is_open())
		return false;

	//For every element in the clients list, we save it in the correct format
	//Pentru fiecare element il salvam in formatul corect
	for (const auto& key_pair : Clients)
		fout << key_pair.second.name << ' ' << key_pair.second.surname << ' ' << key_pair.second.acc_nr << ' ' << key_pair.second.currency << ' ' << key_pair.second.balance << ' ' << key_pair.second.last_op << '\n';

	//We completed the saving of the data, so we close the file
	//S-a facut salvarea
	return true;
}


/*
Ce este: Functie pentru a concatena doua stringuri ale caror caractere au fost transformate toate in minuscule. Returneaza stringurile concatenate
Rostul ei: Se utilieaza pentru a-i oferi fiecarui utilizator un contor unic care sa-l defineasca in hash map,
astfel incat stergerea acestuia sa fie rapida.

What is it: Function to concatena two strings whose characters have been transformed in lowercase letters. Returns the concatenated strings.
Why is it here: This function is used to provide a custom key to every client account index in the hash_map, so that
deleting an account is quicker. (O(n) time approximatively)
*/
std::string Database::hash(std::string s1, std::string s2){
	for (auto& chr : s1)
		chr = std::tolower(chr);
	for (auto& chr : s2)
		chr = std::tolower(chr);

	return s1 + s2;
}

/*
Ce este: Functie pentru a transforma un string in majuscule. Returneaza stringul in majuscule
Rostul ei: Se utilieaza pentru a capitaliza numele monedei unui utilizator si a memoriza-o asa, astfel incat toti utilizatorii
cu o moneda comuna sa aiba acelasi nume la ea.

What is it: Function to transform a string in uppercase. It returns the uppercase string.
Why is it here: This function is used to provide a custom key to every client account index in the hash_map, so that
deleting an account is quicker. (O(n) time approximatively - because it must sort through the vector for the std::erase function)
*/
std::string Database::capitalize(std::string s){
	for (auto& chr : s)
		chr = std::toupper(chr);

	return s;
}
