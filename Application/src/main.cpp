#include <windows.h>
#include "MainDirectory/Interface.h"
#include "MainDirectory/NoUI.h"

int main(int argc, char** argv)
{
	bool noui = false;

	//Citim argumentele date pentru executie, si daca intalnim "-noui" intram in modul fara interfata
	for (int i = 0; i < argc; ++i) 
		if (std::strstr(argv[i],"-noui"))
			noui = true;

	//Daca nu intalnim "-noui", avem interfata
	if (noui == false) {
		//Daca nu suntem in modul debug si avem interfata, ascundem consola
		#ifndef _DEBUG
			ShowWindow(GetConsoleWindow(), SW_HIDE);
		#endif
			

		return UInterfaceMain(argc, argv);
	}

	return NoUIMain(argc, argv);
}
/*
	TODO
	- Add fail-cases for adding/removing clients and the other functions.
	- Increase robustivity and check if we're passed strings where we are supposed to have numbers or stuff to avoid errors (specially in no ui mode)
	Otherwise, program will crash and start acting out (try passing a number when you're asked to pass in an accounts' balance)
	- Add comments in english too
*/