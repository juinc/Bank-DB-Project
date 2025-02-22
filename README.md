# OMNI BANK DATABASE MANAGER
The omni bank database manager represents a bleeding edge piece of software in bank technology. This application can function with and without an interface (console interface mode). To trigger the no interface mode just create a shortcut and add "-noui" at the end of the „target” section, with a space between the path string to the .exe file of the program, or run the application from a terminal adding the "-noui" argument.
<p align="center">
  <img src="[[https://github-production-user-asset-6210df.s3.amazonaws.com/81556297/415790911-e8b16a1d-674b-44c9-b320-a6fc8daee1e1.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAVCODYLSA53PQK4ZA%2F20250221%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20250221T191325Z&X-Amz-Expires=300&X-Amz-Signature=ae78762acea500ed1684bbc9b57c9694a17abbae01f495a4082cb2bbae3f6a38&X-Amz-SignedHeaders=host](https://github.com/user-attachments/assets/04f10e25-f40e-401e-8ec1-6891aad93e2a)](https://github.com/user-attachments/assets/04f10e25-f40e-401e-8ec1-6891aad93e2a)" />
</p>

This is a project that I made for school. There is a translated version of this readme in Romanian nearing the bottom of the page. 
## CREDITS
The whole project's is based around [TheCherno's Walnut Framework](https://github.com/StudioCherno/Walnut). I took the dev branch's code, tinkered with `ApplicationGUI.h/.cpp` and a few other headers a bit (to add titlebar icon loading from a file, application embed icon loading and among other things).

The aforementioned framework also uses libraries such as `imgui`, `glfw`, `vulkan`, `premake` and others. They are all located in the `vendor` folder of the project so credit goes to all of their respective owners, maintainers and contributors.

## GETTING STARTED
Simply just clone the code and then run the [Setup-ExampleProject.bat](https://github.com/juinc/Bank-DB-Project/blob/main/scripts/Setup-ExampleProject.bat) script. This will create the .sln files for all of the projects and will get you started.

You can also just download one of the provided zip files from the releases tab, that should have everything ready to go.

**WARNING! You need to have the Vulkan SDK installed in your system for the interface to work!**

**P.S.** It is highly recommended you use Visual Studio to develop the application, I have not tested with other code editors / IDEs.
## TO-DO
- [ ] Create appropriate comments for all of the functions in Romanian
- [ ] Create appropriate comments for all of the functions in English
- [ ] Add multi-language support
- [ ] Translate the interfaces into English after multi-language support is added
- [ ] Add fail-cases for adding/removing clients and the other functions.
- [ ] Increase robustivity and check if we're passed strings where we are supposed to have numbers or stuff to avoid errors (specially in no ui mode)
***
# MANAGERUL DE DATE DE BAZE OMNI BANK
Managerul de date de bază omni bank reprezintă o tehnologie de vârf în industria bancară. Această aplicație poate funcționa și cu și fără o interfață (în modul de interfață de consolă). Pentru a intra în modul fără interfață, creați un shortcut și adăugați „-noui” la finalul căsuței de target, cu un spațiu la directoriul din care se va încărca fișierul.exe sau rulați programul din terminal cu unul din argumente fiind „-noui”
<p align="center">
  <img src="[[https://github-production-user-asset-6210df.s3.amazonaws.com/81556297/415790911-e8b16a1d-674b-44c9-b320-a6fc8daee1e1.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAVCODYLSA53PQK4ZA%2F20250221%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20250221T191325Z&X-Amz-Expires=300&X-Amz-Signature=ae78762acea500ed1684bbc9b57c9694a17abbae01f495a4082cb2bbae3f6a38&X-Amz-SignedHeaders=host](https://github.com/user-attachments/assets/04f10e25-f40e-401e-8ec1-6891aad93e2a)](https://github.com/user-attachments/assets/04f10e25-f40e-401e-8ec1-6891aad93e2a)" />
</p>

Acesta este un proiect pe care l-am făcut pentru școală.
## MERITE
Totalitatea acestui proiect se bazează pe cutia de unelte a lui TheCherno, numită [Walnut](https://github.com/StudioCherno/Walnut). Am preluat codul din branch-ul „dev” și am modificat `ApplicationGUI.h/.cpp` și câteva alte header-e un pic (pentru a adăuga capacitatea de a încărca iconițe în meniul de sus al programului dintr-un fișier, încărcarea iconiței programului din taskbar dintr-un fișier embedded și printre altele.

Librăria menționată anterior folosește și ea altele precum `imgui`, `glfw`,`vulkan`, `premake` etc. Toate sunt localizate în folderul `vendor` al programului, așa că le mulțumesc creatorilor lor înzecit. 

## PUNEREA ÎN FUNCȚIUNE
Clonați codul, și apoi rulați scriptul [Setup-ExampleProject.bat](https://github.com/juinc/Bank-DB-Project/blob/main/scripts/Setup-ExampleProject.bat). Acesta va crea toate fișierele .sln pentru toate proiectele și te va pune pe picioare.

Puteți, de asemenea, să descărcați unul din fișierele .zip din releases, iar acolo totul este deja pre-executat și gata de utilizat.

**WARNING! Aveți nevoie de SDK-ul de la vulkan instalat în sistem pentru ca interfața să funcționeze**

**P.S.** Este foarte recomandat să folosiți Visual Studio pentru acest proiect. Nu am testat alte aplicații pentru programare în C++.
