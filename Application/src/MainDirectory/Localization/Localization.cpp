#include "Localization.h"

// Placeholder for missing translations
// Model pentru traducerile lipsa
const std::string Localization::missingPlaceholder = "[missing]";

// English translations
// Traducerile in engleza
const std::unordered_map<std::string, std::string> Localization::en_translations = {
    {"welcome", "Welcome to the Omni Bank database manager!"},
    {"prompt.filename", "Please enter the file name with the database data."},
    {"prompt.newdb", "Leave empty and press read if you want to start a new database."},
    {"prompt.savefile", "Leave empty and press save if you want to save to the file you initially read from (shown below)."},
    {"prompt.invalidfile", "If the read file is invalid (e.g. does not exist), it will be ignored and a new database will be created:"},
    {"button.read", "Read Data"},
    {"button.save", "Save Data"},
    {"currentfile", "Current file in use:"},
    {"header.clientlist", "Client List"},
    {"button.showclients", "Show Normal Client List"},
    {"header.orderedlist", "Client List Ordered Alphabetically by Currency + Name"},
    {"button.showordered", "Show Ordered Client List"},
    {"header.deleteclient", "Delete Client"},
    {"input.delname", "Enter Name to Delete"},
    {"input.delsurname", "Enter Surname to Delete"},
    {"button.deleteclient", "Delete Client"},
    {"success.clientdeleted", "Client deleted successfully!"},
    {"error.deletefail", "An error occurred!"},
    {"error.invalidname", "Invalid name or surname!"},
    {"header.addclient", "Add Client"},
    {"input.addname", "Enter Name"},
    {"input.addsurname", "Enter Surname"},
    {"input.addcurrency", "Enter Currency (e.g. RON)"},
    {"input.addbalance", "Enter Balance"},
    {"button.addclient", "Add Client"},
    {"success.clientadded", "Client added successfully!"},
    {"error.addfail", "An error occurred!"},
    {"error.invalidinput", "Invalid data!"},
    {"header.richestclients", "List of Clients with the Highest Balance for Each Currency"},
    {"button.showrichest", "Show Richest Clients List"},
    {"header.totalbalance", "Total Bank Balance"},
    {"button.updatebalance", "Update"},
    {"totalbalance", "The total bank balance is:"},
    {"team.header", "Team Members"},
    {"team.members", "Project by juinc"},
    {"menu.settings", "Settings"},
    {"settings.autoupdate", "Auto-update lists"},
    {"settings.autosave", "Auto-save data on exit"},
    {"menu.language", "Language"},
    {"menu.quit", "Quit Program"},
    {"menu.info", "Info"},
    {"menu.about", "About"},
    {"about.title", "Info"},
    {"about.appname", "Omni Bank Database Manager"},
    {"about.author", "Created by Omni Inc."},
    {"button.close", "Close"},
    {"warning.title", "Warning"},
    {"warning.nofile", "No data file is open/read!"},
    {"error.invalidfilename", "Invalid file name!"},
    // Table headers
    {"table.name", "Name"},
    {"table.surname", "Surname"},
    {"table.accnr", "Account No."},
    {"table.currency", "Currency"},
    {"table.balance", "Balance"},
    {"table.lastop", "Last Operation"},

    //No UI
    {"menu.main", "MENU"},
	{"menu.fileops", "File Operations"},
	{"button.newdb", "Create Empty Database"},
	{"prompt.command", "Enter command:"},
	{"success.read", "Read successful!"},
	{"error.read", "An error occurred while reading (file may not exist)! An empty database was created instead."},
	{"success.save", "Save successful!"},
	{"error.save", "An error occurred while saving!"},
	{"success.newdb", "Empty database created successfully!"},
	{"header.clientlist.ron", "Bank Account List in RON"},
	{"button.showron", "Show RON Account List"},
	{"value.true", "true"},
	{"value.false", "false"},
	{"success.langchange", "Language changed successfully!"},
};

// Romanian translations
// Traducerile in romana
const std::unordered_map<std::string, std::string> Localization::ro_translations = {
    {"welcome", "Bine ati venit la managerul de date de baza Omni Bank!"},
    {"prompt.filename", "Va rugam sa introduceti numele fisierului cu datele din data de baza."},
    {"prompt.newdb", "Lasati gol si apasati pe citire daca doriti sa incepeti o noua data de baze."},
    {"prompt.savefile", "Lasati gol si apasati pe salvati daca doriti sa salvati in fisierul din care ati citit initial (scrie mai jos)."},
    {"prompt.invalidfile", "Daca fisierul de citire este invalid (ex. nu exista), el nu se va lua in considerare si se va crea o data de baze noua:"},
    {"button.read", "Citeste datele"},
    {"button.save", "Salveaza datele"},
    {"currentfile", "Fisierul din care s-au citit datele si cu care lucram:"},
    {"header.clientlist", "Lista Clienti"},
    {"button.showclients", "Afiseaza Lista Clienti Normali"},
    {"header.orderedlist", "Lista Clienti Ordonata Alfabetic Dupa Moneda + Nume"},
    {"button.showordered", "Afiseaza Lista Clienti Ordonata"},
    {"header.deleteclient", "Sterge Client"},
    {"input.delname", "Introduceti Numele Stergere"},
    {"input.delsurname", "Introduceti Prenumele Stergere"},
    {"button.deleteclient", "Stergeti Client"},
    {"success.clientdeleted", "Client sters cu succes!"},
    {"error.deletefail", "S-a produs o eroare!"},
    {"error.invalidname", "Nume sau prenume invalid!"},
    {"header.addclient", "Adauga Client"},
    {"input.addname", "Introduceti Numele"},
    {"input.addsurname", "Introduceti Prenumele"},
    {"input.addcurrency", "Introduceti Moneda (ex. RON)"},
    {"input.addbalance", "Introduceti Bilantul"},
    {"button.addclient", "Adaugati Client"},
    {"success.clientadded", "Client adaugat cu succes!"},
    {"error.addfail", "S-a produs o eroare!"},
    {"error.invalidinput", "Datele sunt invalide!"},
    {"header.richestclients", "Lista Clienti Cu Cel Mai Mare Sold Pt Fiecare Moneda"},
    {"button.showrichest", "Afiseaza Lista Clienti Bogati"},
    {"header.totalbalance", "Soldul Total Al Bancii"},
    {"button.updatebalance", "Actualizeaza"},
    {"totalbalance", "Soldul total al bancii este:"},
    {"team.header", "Membrii echipei"},
    {"team.members", "Proiect realizat de juinc"},
    {"menu.settings", "Setari"},
    {"settings.autoupdate", "Actualizare automata a listelor"},
    {"settings.autosave", "Salvare automata a datelor la iesirea din program"},
    {"menu.language", "Limba"},
    {"menu.quit", "Iesire program"},
    {"menu.info", "Info"},
    {"menu.about", "Despre"},
    {"about.title", "Info"},
    {"about.appname", "Managerul de date de baze Omni Bank"},
    {"about.author", "Creat de Omni Inc."},
    {"button.close", "Inchide"},
    {"warning.title", "Avertisment"},
    {"warning.nofile", "Nu exista niciun fisier de date deschis / citit!"},
    {"error.invalidfilename", "Nume de fisier invalid!"},
    // Table headers
    {"table.name", "Nume"},
    {"table.surname", "Prenume"},
    {"table.accnr", "NrCont"},
    {"table.currency", "Moneda"},
    {"table.balance", "Sold"},
    {"table.lastop", "UltimaOperatie"},

    //No UI
    {"menu.main", "MENIU"},
	{"menu.fileops", "Operatii Fisiere"},
	{"button.newdb", "Creare Data de Baze Goala"},
	{"prompt.command", "Introduceti comanda:"},
	{"success.read", "Citire facuta cu succes!"},
	{"error.read", "A aparut o eroare la citire (posibil ca fisierul sa nu existe)! S-a creat o data de baze goala in schimb."},
	{"success.save", "Salvare facuta cu succes!"},
	{"error.save", "A aparut o eroare la salvare!"},
	{"success.newdb", "Data de baze goala creata cu succes!"},
	{"header.clientlist.ron", "Lista Conturilor Bancii in lei / RON"},
	{"button.showron", "Afiseaza Lista Conturi RON"},
	{"value.true", "adevarata"},
	{"value.false", "falsa"},
	{"success.langchange", "Limba schimbata cu succes!"},
};

// Constructor: default to English
// Constructor: selecteaza engleza la baza
Localization::Localization() : translations(&en_translations) {}

// Sets the current language based on the language code
// Seteaza limba curenta pe baza codului de limba
bool Localization::SetLanguage(const std::string& langCode) {
    if (langCode == "en") {
        translations = &en_translations;
        return true;
    }
    if (langCode == "ro") {
        translations = &ro_translations;
        return true;
    }
    translations = &en_translations; // fallback
    return false;
}

// Gets the translation for a key, or a placeholder if missing
// Obtine traducerea pentru o cheie, sau un placeholder daca lipseste
const std::string& Localization::Get(const std::string& key) const {
    auto it = translations->find(key);
    if (it != translations->end()) {
        return it->second;
    }
    return missingPlaceholder;
}