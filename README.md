# OMNI BANK DATABASE MANAGER
The omni bank database manager represents a bleeding edge piece of software in bank technology. This application can function with and without an interface (console interface mode). To trigger the no interface mode just create a shortcut and add "-noui" at the end of the „target” section, with a space between the path string to the .exe file of the program, or run the application from a terminal adding the "-noui" argument.
<p align="center">
  <img src="https://github.com/user-attachments/assets/b8da1b87-2906-480e-b5f7-a04e3c8c2ee2" />
</p>

This is a project that I made for school. There is a translated version of this readme in Romanian nearing the bottom of the page.
## REQUIREMENTS
- [Visual Studio 2022](https://visualstudio.com) (not strictly required, however included setup scripts only support this)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows) (preferably a recent version)
## GETTING STARTED
Simply just clone the code and then run the [Setup-ExampleProject.bat](https://github.com/juinc/Bank-DB-Project/blob/main/scripts/Setup-ExampleProject.bat) script. This will create the .sln files for all of the projects and will get you started.
## CREDITS & LICENSES
The whole project's is based around [TheCherno's Walnut Framework](https://github.com/StudioCherno/Walnut). I took the dev branch's code, tinkered with `ApplicationGUI.h/.cpp` and a few other headers a bit (to add titlebar icon loading from a file, application embed icon loading and among other things).

The aforementioned framework also uses libraries such as `imgui`, `glfw`, `vulkan`, `premake` and others. They are all located in the `vendor` folder of the project so credit goes to all of their respective owners, maintainers and contributors.

**3rd party libaries**
- [Dear ImGui](https://github.com/ocornut/imgui)
- [GLFW](https://github.com/glfw/glfw)
- [stb_image](https://github.com/nothings/stb)
- [GLM](https://github.com/g-truc/glm)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)
- [spdlog](https://github.com/gabime/spdlog)
- [premake](https://premake.github.io/) (just the binaries)
  
**Additional**
- Walnut uses the [Roboto](https://fonts.google.com/specimen/Roboto) font ([Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0))
## TO-DO
- [x] Create appropriate comments for all of the functions in Romanian
- [x] Create appropriate comments for all of the functions in English
- [x] Add multi-language support
- [x] Translate the interfaces into English after multi-language support is added
- [x] Add fail-cases for adding/removing clients and the other functions.
- [x] Increase robustivity and check if we're passed strings where we are supposed to have numbers or stuff to avoid errors (specially in no ui mode)
***

Reuploaded for e-mail privacy concerns.
# MANAGERUL DE DATE DE BAZE OMNI BANK
Managerul de date de bază omni bank reprezintă o tehnologie de vârf în industria bancară. Această aplicație poate funcționa și cu și fără o interfață (în modul de interfață de consolă). Pentru a intra în modul fără interfață, creați un shortcut și adăugați „-noui” la finalul căsuței de target, cu un spațiu la directoriul din care se va încărca fișierul.exe sau rulați programul din terminal cu unul din argumente fiind „-noui”
<p align="center">
  <img src="https://github.com/user-attachments/assets/b8da1b87-2906-480e-b5f7-a04e3c8c2ee2" />
</p>

Acesta este un proiect pe care l-am făcut pentru școală.
## NECESITĂȚI
- [Visual Studio 2022](https://visualstudio.com) (nu în mod strict, dar script-urile incluse au suport doar pentru acest IDE)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows) (preferabil o versiune recentă)

## PUNEREA ÎN FUNCȚIUNE
Clonați codul, și apoi rulați scriptul [Setup-ExampleProject.bat](https://github.com/juinc/Bank-DB-Project/blob/main/scripts/Setup-ExampleProject.bat). Acesta va crea toate fișierele .sln pentru toate proiectele și te va pune pe picioare.
## MERITE
Totalitatea acestui proiect se bazează pe cutia de unelte a lui TheCherno, numită [Walnut](https://github.com/StudioCherno/Walnut). Am preluat codul din branch-ul „dev” și am modificat `ApplicationGUI.h/.cpp` și câteva alte header-e un pic (pentru a adăuga capacitatea de a încărca iconițe în meniul de sus al programului dintr-un fișier, încărcarea iconiței programului din taskbar dintr-un fișier embedded și printre altele.

Librăria menționată anterior folosește și ea altele precum `imgui`, `glfw`,`vulkan`, `premake` etc. Toate sunt localizate în folderul `vendor` al programului, așa că le mulțumesc creatorilor lor înzecit. 

**Librării 3rd-party**
- [Dear ImGui](https://github.com/ocornut/imgui)
- [GLFW](https://github.com/glfw/glfw)
- [stb_image](https://github.com/nothings/stb)
- [GLM](https://github.com/g-truc/glm)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)
- [spdlog](https://github.com/gabime/spdlog)
- [premake](https://premake.github.io/) (just the binaries)
  
**Extra**
- Walnut folosește fontul [Roboto](https://fonts.google.com/specimen/Roboto), ([Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0))

Încărcat din nou pentru griji privind confidențialitatea e-mailului meu personal.