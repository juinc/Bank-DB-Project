#pragma once

#include <string>
#include <unordered_map>

class Localization {
public:
	Localization(); // Add constructor - selecteaza engleza la baza / Adauga constructor - selects english by default

	
    // Supported languages: "en" (English), "ro" (Romanian)

    // Sets the current language based on the language code
	// Seteaza limba curenta pe baza codului de limba
    bool SetLanguage(const std::string& langCode);

    // Gets the translation for a key, or a placeholder if missing
	// Obtine traducerea pentru o cheie, sau un placeholder daca lipseste
    const std::string& Get(const std::string& key) const;

private:
    // Translation vector, gets set when a new language is selected
	// Vectorul de traduceri, se seteaza cand o noua limba este selectata
    const std::unordered_map<std::string, std::string>* translations = nullptr;

	// English and Romanian translations
	// Traducerile in engleza si romana
    static const std::unordered_map<std::string, std::string> en_translations;
    static const std::unordered_map<std::string, std::string> ro_translations;

	// Placeholder for missing translations
	// Model pentru traducerile lipsa
    static const std::string missingPlaceholder;
};