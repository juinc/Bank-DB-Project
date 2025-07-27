#include <windows.h>
#include "MainDirectory/Interface.h"
#include "MainDirectory/NoUI.h"

int main(int argc, char** argv)
{
	bool noui = false;

	//We read the arguments given for execution, and if we encounter "-noui" we enter no UI mode
	//Citim argumentele date pentru executie, si daca intalnim "-noui" intram in modul fara interfata
	for (int i = 0; i < argc; ++i) 
		if (std::strstr(argv[i],"-noui"))
			noui = true;

	//If we're not in no UI mode, we have the interface
	//Daca nu intalnim "-noui", avem interfata
	if (noui == false) {

		//If we're in debug mode, we show the console window
		//Daca nu suntem in modul debug si avem interfata, ascundem consola
		#ifndef WL_DEBUG
			ShowWindow(GetConsoleWindow(), SW_HIDE);
		#endif
			

		return UInterfaceMain(argc, argv);
	}

	return NoUIMain(argc, argv);
}