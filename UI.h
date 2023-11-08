#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <ShlObj.h>
#include "Button.h"
#include "Mod.h"
#include "Game.h"
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <string>
#include "json.hpp"
using json = nlohmann::json;

//disable warnings
#define _CRT_SECURE_NO_WARNINGS


class UI
{
public:
    sf::Font font;
    sf::Text title;
    sf::Text description;
    sf::Text genre;
    sf::Text platform;
    sf::Text bepinexVersion;
    sf::Text isBepinexInstalled;
    sf::RectangleShape displayArea;
    sf::RectangleShape darkenerArea;
    sf::RectangleShape buttonArea;
    sf::Text currentWD;
    bool isInstalled;
    std::string bepInstalled = "";
    std::vector<Button*> buttons;
    sf::Texture image;
    sf::Texture darkener;
    bool isModManagerOpen = false;
    std::vector<std::unique_ptr<Mod>> mods;
    int modUpdateTicker = 0;
    int gameIndex = 0;
    std::string pathChosen = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Wobbly Life";
    sf::Text noModsText;
    sf::Text settingsText;
    bool isSettingsOpen = false;
    
    UI(std::vector<Game> games)
    {
#pragma UIINIT
        noModsText.setString("No mods found!");
        noModsText.setFont(font);
        noModsText.setCharacterSize(60);
        noModsText.setFillColor(sf::Color(0,100,100));
        noModsText.setStyle(sf::Text::Bold);

        settingsText.setString("EXTRAS");
        settingsText.setFont(font);
        settingsText.setCharacterSize(60);
        settingsText.setFillColor(sf::Color(0,80,80));
        settingsText.setStyle(sf::Text::Bold);
        settingsText.setPosition(sf::Vector2f(400,60));
        
        buttons = std::vector<Button*>();
        mods = std::vector<std::unique_ptr<Mod>>();
        this->font.loadFromFile("resources/fonts/JetBrainsMono-Regular.ttf");
        this->title.setFont(font);
        this->description.setFont(font);
        this->genre.setFont(font);
        this->platform.setFont(font);
        this->bepinexVersion.setFont(font);
        this->title.setString(games[0].name);
        this->description.setString(games[0].description);
        this->genre.setString(games[0].genre);
        this->platform.setString(games[0].platform);
        this->bepinexVersion.setFillColor(sf::Color::Red);
        this->bepinexVersion.setStyle(sf::Text::Bold);
        bepInstalled = "FALSE";
        this->isInstalled = false;

        if (IsBepinexInstalled(games[0].installLocation))
        {
            bepInstalled = "TRUE";
            this->bepinexVersion.setFillColor(sf::Color::Green);
            this->isInstalled = true;
        }

        this->bepinexVersion.setString("BEPINEX INSTALLED: " + bepInstalled);
        this->currentWD.setString(games[0].installLocation);
        this->currentWD.setFillColor(sf::Color::White);
        this->currentWD.setCharacterSize(13);
        this->currentWD.setStyle(sf::Text::Bold);
        this->currentWD.setPosition(sf::Vector2f(400, 525));
        this->currentWD.setFont(font);
        this->currentWD.setOrigin((int)this->currentWD.getLocalBounds().width / 2, (int)this->currentWD.getLocalBounds().height / 2);
        this->title.setCharacterSize(40);
        this->description.setCharacterSize(10);
        this->genre.setCharacterSize(15);
        this->platform.setCharacterSize(15);
        this->bepinexVersion.setCharacterSize(13);
        this->title.setOutlineColor(sf::Color::Black);
        this->title.setOutlineThickness(1);
        this->title.setFillColor(sf::Color::White);
        this->description.setFillColor(sf::Color::White);
        this->genre.setFillColor(sf::Color::White);
        this->platform.setFillColor(sf::Color::White);
        
        this->title.setStyle(sf::Text::Bold);
        this->title.setOrigin((int)this->title.getLocalBounds().width / 2, (int)this->title.getLocalBounds().height / 2);
        this->bepinexVersion.setOrigin((int)this->bepinexVersion.getLocalBounds().width / 2, (int)this->bepinexVersion.getLocalBounds().height / 2);
        this->title.setPosition(400, 80);
        this->description.setPosition(400, 600);
        this->genre.setPosition(100, 150);
        this->platform.setPosition(100, 175);
        this->bepinexVersion.setPosition(400, 115);

        /*
                    0) install bepinex 5
                    1) install bepinex 6
                    2) wipe mods
                    3) play
                    4) change directory
                    5) open directory
                    6) mod manager
                    7) exit mod manager
                    8) install mod


                         -settings buttons vvvv
                    9) update ddloader
                    10) offline update ddloader


                        -game switcher buttons vvvv
                    11) +gameIndex
                    12) -gameIndex

                    13) thank yous
        */

        this->buttons.push_back(new Button("Install BepInEx 5", sf::Vector2f(206, 690)));
        buttons[0]->setSize(sf::Vector2f(370,180));
        this->buttons.push_back(new Button("Install BepInEx 6", sf::Vector2f(800-206, 690)));
        buttons[1]->setSize(sf::Vector2f(370,180));
        this->buttons.push_back(new Button("Wipe Mods", sf::Vector2f(206, 690)));
        buttons[2]->setSize(sf::Vector2f(370,180));
        this->buttons.push_back(new Button("Play", sf::Vector2f(100, 350)));
        this->buttons.push_back(new Button("Change Directory", sf::Vector2f(400, 555)));
        buttons[4]->setSize(sf::Vector2f(760,30));
        this->buttons.push_back(new Button("Open Directory", sf::Vector2f(400, 555)));
        buttons[5]->setSize(sf::Vector2f(760,30));
        this->buttons.push_back(new Button("Mod Manager", sf::Vector2f(800-206, 690)));
        buttons[6]->setSize(sf::Vector2f(370,180));
        this->buttons.push_back(new Button("Exit Mod Manager", sf::Vector2f(800-206, 690)));
        buttons[7]->setSize(sf::Vector2f(370,180));
        this->buttons.push_back(new Button("Install Mod", sf::Vector2f(206, 690)));
        buttons[8]->setSize(sf::Vector2f(370,180));

        this->buttons.push_back(new Button("Update DDLoader", sf::Vector2f(206,690)));
        buttons[9]->setSize(sf::Vector2f(370,180));
        this->buttons.push_back(new Button("Offline Update\n (.zip method)", sf::Vector2f(800-206,690)));
        buttons[10]->setSize(sf::Vector2f(370,180));

        this->buttons.push_back(new Button("Discord", sf::Vector2f(206,530)));
        buttons[11]->setSize(sf::Vector2f(370,90));
        this->buttons.push_back(new Button("GitHub", sf::Vector2f(800-206,530)));
        buttons[12]->setSize(sf::Vector2f(370,90));

        this->buttons.push_back(new Button("THANK YOU <3", sf::Vector2f(400, 455)));
        buttons[13]->setSize(sf::Vector2f(760,30));

        this->buttons.push_back(new Button(">>", sf::Vector2f(750, 400)));
        buttons[14]->setSize(sf::Vector2f(50,50));
        this->buttons.push_back(new Button("<<", sf::Vector2f(50, 400)));
        buttons[15]->setSize(sf::Vector2f(50,50));
        

        this->buttonArea.setFillColor(sf::Color(0, 148, 148));
        this->buttonArea.setSize(sf::Vector2f(780,200));
        this->buttonArea.setOrigin(390,this->buttonArea.getGlobalBounds().height/2);
        this->buttonArea.setPosition(400,690);
        this->buttonArea.setOutlineColor(sf::Color::Black);
        this->buttonArea.setOutlineThickness(1);
        this->displayArea.setFillColor(sf::Color::White);
        image.loadFromFile("resources/images/game0.png");
        this->displayArea.setTexture(&image);
        this->displayArea.setSize(sf::Vector2f(780,530));
        this->displayArea.setOrigin(390,this->buttonArea.getGlobalBounds().height/2);
        this->displayArea.setPosition(400,150);
        this->displayArea.setOutlineColor(sf::Color::Black);
        this->displayArea.setOutlineThickness(1);
        
        this->darkener.loadFromFile("resources/images/darker.png");
        this->darkenerArea.setTexture(&darkener);
        this->darkenerArea.setSize(sf::Vector2f(780,530));
        this->darkenerArea.setOrigin(390,this->buttonArea.getGlobalBounds().height/2);
        this->darkenerArea.setPosition(400,150);
        this->darkenerArea.setOutlineColor(sf::Color::Black);
        this->darkenerArea.setOutlineThickness(1);
        noModsText.setOrigin(noModsText.getGlobalBounds().width/2,noModsText.getGlobalBounds().height/2);
        noModsText.setPosition(sf::Vector2f(400,300));
        
        settingsText.setOrigin(settingsText.getGlobalBounds().width/2,settingsText.getGlobalBounds().height/2);

#pragma endregion
        UpdateModlist(currentWD.getString());
    }

    ~UI() 
    {
        for (Button* button : buttons) 
        {
            delete button;
        }

    }

    void set(Game game)
    {
        this->title.setString(game.name);
        this->description.setString(game.description);
        this->genre.setString(game.genre);
        this->platform.setString(game.platform);
        this->bepinexVersion.setString("BEPINEX " + game.bepinexVersion);
        this->currentWD.setString(game.installLocation);
        this->title.setOrigin((int)this->title.getLocalBounds().width / 2, (int)this->title.getLocalBounds().height / 2);
        this->bepinexVersion.setOrigin((int)this->bepinexVersion.getLocalBounds().width / 2, (int)this->bepinexVersion.getLocalBounds().height / 2);
        //set the picture of the game
        std::string gamePicName = "resources/images/game" + std::to_string(gameIndex) + ".png";
        image.loadFromFile(gamePicName);
        this->displayArea.setTexture(&image);
        if (IsBepinexInstalled(game.installLocation))
        {
            bepInstalled = "TRUE";
            this->bepinexVersion.setString("BEPINEX INSTALLED: " + bepInstalled);
            this->bepinexVersion.setOrigin((int)this->bepinexVersion.getLocalBounds().width / 2, (int)this->bepinexVersion.getLocalBounds().height / 2);
            this->bepinexVersion.setFillColor(sf::Color::Green);
            this->isInstalled = true;
        }
        else
		{
        	bepInstalled = "FALSE";
            this->bepinexVersion.setString("BEPINEX INSTALLED: " + bepInstalled);
            this->bepinexVersion.setOrigin((int)this->bepinexVersion.getLocalBounds().width / 2, (int)this->bepinexVersion.getLocalBounds().height / 2);
        	this->bepinexVersion.setFillColor(sf::Color::Red);
        	this->isInstalled = false;
        }
    }

    void updateMain(sf::Vector2i mousePosition, sf::RenderWindow& window, bool locked, std::vector<Game> games)
    {
        if (locked)
            return;
        if (!isModManagerOpen)
        {
            buttons[14]->update(mousePosition);
            buttons[15]->update(mousePosition);
            if (buttons[14]->isClicked(window))
            {
                gameIndex++;
                if (gameIndex > games.size() - 1)
					gameIndex = 0;
                set(games[gameIndex]);
				buttons[14]->state = DEFAULT;
                LockButtons();
            }
            if (buttons[15]->isClicked(window))
			{
            	gameIndex--;
            	if (gameIndex < 0)
                    gameIndex = games.size() - 1;
                set(games[gameIndex]);
                buttons[15]->state = DEFAULT;
                LockButtons();
            }
            if (!isInstalled)
            {
                buttons[0]->update(mousePosition);
                buttons[1]->update(mousePosition);
                buttons[4]->update(mousePosition);
            }
            else
            {
                buttons[2]->update(mousePosition);
                buttons[5]->update(mousePosition);
                buttons[6]->update(mousePosition);

            }

            if (buttons[0]->isClicked(window))
            {
                Unzip("resources/bepinex/bepinex5.zip", currentWD.getString());
                isInstalled = IsBepinexInstalled(currentWD.getString());
                buttons[0]->state = DEFAULT;
                buttons[1]->state = DEFAULT;

                LockButtons();
                
                if (IsBepinexInstalled(currentWD.getString()))
                {
                    bepInstalled = "TRUE";
                    this->bepinexVersion.setFillColor(sf::Color::Green);
                    this->isInstalled = true;
                    this->bepinexVersion.setString("BEPINEX INSTALLED: " + bepInstalled);
                    createPluginsFolder(currentWD.getString());
                }
            }
            else if (buttons[4]->isClicked(window))
            {
                ChangeDirectories(pathChosen);
                LockButtons();
            }
            else if (buttons[1]->isClicked(window))
            {
                Unzip("resources/bepinex/bepinex6.zip", currentWD.getString());
                isInstalled = IsBepinexInstalled(currentWD.getString());
                buttons[0]->state = DEFAULT;
                buttons[1]->state = DEFAULT;
                LockButtons();
                if (IsBepinexInstalled(currentWD.getString()))
                {
                    bepInstalled = "TRUE";
                    this->bepinexVersion.setFillColor(sf::Color::Green);
                    this->isInstalled = true;
                    this->bepinexVersion.setString("BEPINEX INSTALLED: " + bepInstalled);
                    createPluginsFolder(currentWD.getString());
                }
            }

            else if (buttons[2]->isClicked(window))
            {
                WipeMods(currentWD.getString());
                isInstalled = false;
                buttons[2]->state = DEFAULT;
                LockButtons();
                if (!IsBepinexInstalled(currentWD.getString()))
                {
                    bepInstalled = "FALSE";
                    this->bepinexVersion.setFillColor(sf::Color::Red);
                    this->isInstalled = false;
                    this->bepinexVersion.setString("BEPINEX INSTALLED: " + bepInstalled);
                }
            }

            else if (buttons[5]->isClicked(window))
            {
                OpenDirectory(currentWD.getString());
                buttons[5]->state = DEFAULT;
                LockButtons();
            }

            if (buttons[6]->isClicked(window))
            {
                isModManagerOpen = true;
                buttons[6]->state = DEFAULT;
                LockButtons();
            }
        }
        else
        {
            buttons[8]->update(mousePosition);
            buttons[7]->update(mousePosition);

            if (buttons[7]->isClicked(window))
			{
            	isModManagerOpen = false;
            	buttons[7]->state = DEFAULT;
            	LockButtons();
                UpdateModlist(currentWD.getString());
            }

            if (buttons[8]->isClicked(window))
			{
            	installMod(currentWD.getString());
            	buttons[8]->state = DEFAULT;
            	LockButtons();
            	UpdateModlist(currentWD.getString());
            }

            for (int i = 0; i < mods.size();i++)
            {
                mods[i]->update(mousePosition);
                if (mods[i]->buttons[0]->isClicked(window))
                {
                    DisableMod(currentWD.getString(),mods[i]->name);
                    mods[i]->rect.setFillColor(sf::Color(0,96,96));
                    UpdateModlist(currentWD.getString());
                }
                else if (mods[i]->buttons[1]->isClicked(window))
				{
                	DeleteMod(currentWD.getString(),mods[i]->name);
                    UpdateModlist(currentWD.getString());
                }
                else if (mods[i]->buttons[2]->isClicked(window))
                {
                	EnableMod(currentWD.getString(),mods[i]->name);
                    mods[i]->rect.setFillColor(sf::Color(0,148,148));
                    UpdateModlist(currentWD.getString());
                }
            }
        }
    }

    void installMod(std::string dir) 
    {
        Log("Opening File Dialog...");

        IFileOpenDialog* openFileDialog;
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&openFileDialog));

        if (SUCCEEDED(hr)) 
        {
            COMDLG_FILTERSPEC fileTypes[] = { L"ZIP Files", L"*.zip" };
            openFileDialog->SetFileTypes(1, fileTypes);

            hr = openFileDialog->Show(NULL);

            if (SUCCEEDED(hr)) 
            {
                IShellItem* pItem;
                hr = openFileDialog->GetResult(&pItem);

                if (SUCCEEDED(hr)) 
                {
                    PWSTR filePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

                    if (SUCCEEDED(hr)) 
                    {
                        // Convert LPWSTR to std::wstring
                        std::wstring wstr(filePath);
                        std::string zipFile(wstr.begin(), wstr.end());
                        std::string modDir = dir + "\\BepInEx\\plugins";

                        Log("Unzipping " + zipFile + " to " + modDir);
                        Unzip(zipFile, modDir);

                        CoTaskMemFree(filePath);
                    }

                    pItem->Release();
                }
            }

            openFileDialog->Release();
        }
    }

    void ChangeDirectories(std::string& varToChange) 
    {
        Log("Opening Directory Browser...");
        BROWSEINFOW browseInfo = { 0 };
        wchar_t pathW[MAX_PATH];
        browseInfo.hwndOwner = NULL;
        browseInfo.pidlRoot = NULL;
        browseInfo.pszDisplayName = pathW;
        browseInfo.lpszTitle = L"Select a Directory";
        browseInfo.ulFlags = BIF_RETURNONLYFSDIRS;

        LPITEMIDLIST item = SHBrowseForFolderW(&browseInfo);
        if (item != NULL) 
        {
            Log("Something was chosen, getting path...");
            SHGetPathFromIDListW(item, pathW);

            int length = WideCharToMultiByte(CP_UTF8, 0, pathW, -1, nullptr, 0, NULL, NULL);
            std::string path(length, 0);
            WideCharToMultiByte(CP_UTF8, 0, pathW, -1, &path[0], length, NULL, NULL);

            // Remove any null characters from the string
            path.erase(std::remove(path.begin(), path.end(), '\0'), path.end());

            varToChange = path;
            CoTaskMemFree(item);
        }
        else
            return;

        Log("Path chosen: " + varToChange);
        ChangeDirectoryInJson(varToChange);  // Use the corrected path
        currentWD.setString(varToChange);    // Use the corrected path

        if (IsBepinexInstalled(currentWD.getString()))
        {
            bepInstalled = "TRUE";
            this->bepinexVersion.setFillColor(sf::Color::Green);
            this->isInstalled = true;
            this->bepinexVersion.setString("BEPINEX INSTALLED: " + bepInstalled);
        }
        else
        {
            bepInstalled = "FALSE";
            this->bepinexVersion.setFillColor(sf::Color::Red);
            this->isInstalled = false;
            this->bepinexVersion.setString("BEPINEX INSTALLED: " + bepInstalled);
        }
    }

    void ChangeDirectoryInJson(std::string directoryChosen)
	{
        Log("Attempting to change directory in JSON to " + directoryChosen);

        //get the json at the index chosen from /bepinex/games/ and use that for the json path
        std::string jsonPath = "resources/games/game" + std::to_string(gameIndex) + ".json";

        Log("JSON to change: " + jsonPath);

    	std::ifstream file(jsonPath);
    	json j;
    	file >> j;
    	j["installLocation"] = directoryChosen;
    	std::ofstream o(jsonPath);
    	o << std::setw(4) << j << std::endl;
    }

    void LockButtons()
    {
        buttons[0]->ticker = 30;
        buttons[1]->ticker = 30;
        buttons[2]->ticker = 30;
        buttons[3]->ticker = 30;
        buttons[4]->ticker = 30;
        buttons[5]->ticker = 30;
        buttons[6]->ticker = 30;
        buttons[7]->ticker = 30;
        buttons[8]->ticker = 30;
        buttons[9]->ticker = 30;
        buttons[10]->ticker = 30;
        buttons[11]->ticker = 30;
        buttons[12]->ticker = 30;
        buttons[13]->ticker = 30;
        buttons[14]->ticker = 30;
        buttons[15]->ticker = 30;

    }

    bool HasMods(std::string dir)
    {
        std::string path = dir + "/BepInEx/plugins";
		if (std::filesystem::exists(path))
		{
            for (const auto& entry : std::filesystem::directory_iterator(path))
        	{
                std::string modName = entry.path().string();
                modName = modName.substr(modName.find_last_of("/\\") + 1);
                if (modName != "doorstop_config.ini")
                {
                    return true;
                }
            }
        }
        return false;
    }

    void UpdateModlist(std::string dir) 
    {
        mods.clear(); // Clear existing mods

        int i = 0;
        std::string path = dir + "/BepInEx/plugins";
        if (std::filesystem::exists(path)) 
        {
            for (const auto& entry : std::filesystem::directory_iterator(path)) 
            {
                std::string modName = entry.path().string();
                modName = modName.substr(modName.find_last_of("/\\") + 1);
                if (modName != "doorstop_config.ini") 
                {
                    mods.emplace_back(std::make_unique<Mod>(modName, false, sf::Vector2f(400, 70 + (i * 50))));
                    i++;
                }
            }
        }
    }

    void drawMain(sf::RenderWindow& window)
	{
        if (!isModManagerOpen)
        {
            
            window.draw(buttonArea);
            window.draw(displayArea);
            window.draw(darkenerArea);
            window.draw(title);
            window.draw(bepinexVersion);
            window.draw(currentWD);

            if (!isInstalled)
            {
                buttons[0]->draw(window);
                buttons[1]->draw(window);
                buttons[4]->draw(window);
            }
            else
            {
                buttons[2]->draw(window);
                buttons[5]->draw(window);
                buttons[6]->draw(window);
            }

            buttons[14]->draw(window);
            buttons[15]->draw(window);
        }
        else
        {
            

            window.draw(buttonArea);
            buttons[7]->draw(window);
            buttons[8]->draw(window);
            
            if (mods.size() == 0)
                window.draw(noModsText);
            else
            {
                for (int i = 0; i < mods.size();i++)
                {
                    mods[i]->draw(window);
                }
            }
        }
    }

    void updateSettings(sf::Vector2i mousePosition, sf::RenderWindow& window, bool locked, bool& isSecretOpen)
    {
        buttons[9]->update(mousePosition);
        buttons[10]->update(mousePosition);
        buttons[11]->update(mousePosition);
        buttons[12]->update(mousePosition);
        buttons[13]->update(mousePosition);

        if (buttons[9]->isClicked(window))
        {
            UpdateFromGithub();
            buttons[9]->state = DEFAULT;
            LockButtons();
        }

        else if (buttons[10]->isClicked(window))
        {
            std::string directoryProgramStartedIn = std::filesystem::current_path().string(); // Get current directory

            IFileOpenDialog* openFileDialog;
            HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&openFileDialog));

            if (SUCCEEDED(hr)) 
            {
                COMDLG_FILTERSPEC fileTypes[] = { L"ZIP Files", L"*.zip" };
                openFileDialog->SetFileTypes(1, fileTypes);

                hr = openFileDialog->Show(NULL);

                if (SUCCEEDED(hr)) 
                {
                    IShellItem* pItem;
                    hr = openFileDialog->GetResult(&pItem);

                    if (SUCCEEDED(hr)) 
                    {
                        PWSTR filePath;
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

                        if (SUCCEEDED(hr)) 
                        {
                            // Convert LPWSTR to std::wstring
                            std::wstring wstr(filePath);
                            std::string zipFile(wstr.begin(), wstr.end());

                            std::filesystem::path path(zipFile); // Convert to filesystem path
                            std::string directoryZipIsIn = path.parent_path().string(); // Get parent directory

                            

                            UpdateFromZip(directoryZipIsIn, directoryProgramStartedIn);

                            CoTaskMemFree(filePath);
                        }

                        pItem->Release();
                    }
                }

                openFileDialog->Release();
            }
            buttons[10]->state = DEFAULT;
            LockButtons();
        }

        else if (buttons[11]->isClicked(window))
        {
            OpenDiscord();
            buttons[11]->state = DEFAULT;
            LockButtons();
        }

        else if (buttons[12]->isClicked(window))
        {
            OpenGithub();
            buttons[12]->state = DEFAULT;
            LockButtons();
        }

        else if (buttons[13]->isClicked(window))
		{
        	isSecretOpen = true;
        	buttons[13]->state = DEFAULT;
        	LockButtons();
        }
    }

    void drawSettings(sf::RenderWindow& window)
    {
        window.draw(settingsText);
        window.draw(buttonArea);
        buttons[9]->draw(window);
        buttons[10]->draw(window);
        buttons[11]->draw(window);
        buttons[12]->draw(window);
        buttons[13]->draw(window);


    }

    void OpenGithub() 
    {
        const char* url = "https://github.com/daltonyxdonovan/DDLoader2";

        std::string command = "start ";
        command += url;
        int errorCode = system(command.c_str());
        if (errorCode == 0) {
            std::cout << "Opening GitHub page!" << std::endl;
        } else {
            std::cerr << "Failed to open GitHub page." << std::endl;
        }
    }

    void OpenDiscord() 
    {
        const char* url = "https://discord.com/invite/KR7xN3q9uR";

        std::string command = "start ";
        command += url;
        int errorCode = system(command.c_str());
        if (errorCode == 0) {
            std::cout << "Opening Discord server page!" << std::endl;
        } else {
            std::cerr << "Failed to open Discord server page." << std::endl;
        }
    }

    void UpdateFromZip(std::string dirOfZip, std::string currentwd) 
    {
        std::string updateZip = dirOfZip + "\\DDUpdate.zip"; // Path to DDUpdate.zip

        // Check if DDUpdate.zip exists
        if (std::filesystem::exists(updateZip)) {
            // Check if GamesJSON.zip and GamesPNG.zip exist within DDUpdate.zip
            std::string gamesJsonZip = dirOfZip + "\\GamesJSON.zip";
            std::string gamesPngZip = dirOfZip + "\\GamesPNG.zip";

            if (std::filesystem::exists(gamesJsonZip) && std::filesystem::exists(gamesPngZip)) {
                // Unzip GamesJSON.zip to 'resources/games' directory
                Unzip(gamesJsonZip, currentwd + "\\resources\\games");

                // Unzip GamesPNG.zip to 'resources/images' directory
                Unzip(gamesPngZip, currentwd + "\\resources\\images");
                Log("Extraction completed successfully.");
            } else {
                Log("GamesJSON.zip and/or GamesPNG.zip not found within DDUpdate.zip.");
            }
        } else {
            Log("DDUpdate.zip not found in the specified directory.");
        }

        Log("UpdateFromZip() has run");
    }

    bool DownloadFileFromURL(const std::string& url, const std::string& destination) 
    {
        // Use Windows-specific command to download a file from URL
        std::string command = "certutil -urlcache -split -f " + url + " " + destination;
    
        // Convert string command to const char* for system()
        const char* cmd = command.c_str();

        // Execute the command to download the file
        int result = system(cmd);

        return result == 0; // Check if the command was executed successfully
    }

    void UpdateFromGithub() 
    {
        std::string gamesJsonURL = "https://github.com/daltonyxdonovan/DDLoader2/raw/master/GamesJSON.zip";
        std::string gamesPngURL = "https://github.com/daltonyxdonovan/DDLoader2/raw/master/GamesPNG.zip";
        std::string gamesJsonZip = "GamesJSON.zip";
        std::string gamesPngZip = "GamesPNG.zip";

        std::string gamesJsonDestination = "resources/games/";
        std::string gamesPngDestination = "resources/images/";

        // Download JSON assets from GitHub
        if (DownloadFileFromURL(gamesJsonURL, gamesJsonZip)) {
            Log("Downloaded JSON assets successfully.");
        } else {
            Log("Failed to download JSON assets.");
        }

        // Download PNG assets from GitHub
        if (DownloadFileFromURL(gamesPngURL, gamesPngZip)) {
            Log("Downloaded PNG assets successfully.");
        } else {
            Log("Failed to download PNG assets.");
        }

        Log("UpdateFromGithub() has run");
    }


    

    void createPluginsFolder(std::string dir)
    {
        if (!std::filesystem::exists(dir + "/BepInEx"))
		{
            std::filesystem::create_directory(dir + "/BepInEx");
            Log("Created BepInEx folder (Backup method of creation!)");
        }
        if (!std::filesystem::exists(dir + "/BepInEx/plugins"))
        {
        	std::filesystem::create_directory(dir + "/BepInEx/plugins");
            Log("Created plugins folder");
        }

    }

    void Unzip(std::string filePath, std::string dirToUnzipTo) 
    {
        Log("Attempting unzipping of " + filePath + " to " + dirToUnzipTo);
        std::string command = "tar -xf \"" + filePath + "\" -C \"" + dirToUnzipTo + "\" 2>&1";


        FILE* pipe = _popen(command.c_str(), "r");
        if (!pipe) {
            Log("Error: Unable to open pipe.");
            return;
        }

        char buffer[128];
        Log("[PIPE]:\n\n");
        while (fgets(buffer, 128, pipe) != NULL) {
            Log(buffer);
        }

        _pclose(pipe);
    }

    void Log(std::string text) 
    {
    
        std::ofstream file("log-ddloader.txt", std::ios_base::app);
        if (file.is_open()) 
        {
            file << text << std::endl;
            file.close();
            std::cout << "[LOG]: " << text << std::endl;
        }
        else 
        {
            std::cerr << "Error opening or creating log file!" << std::endl;
        }
    }

    bool IsBepinexInstalled(std::string dir)
    {
        std::string path = dir + "/BepInEx";
	    if (std::filesystem::exists(path))
	    {
    	    Log("Passed Bepinex Check!");
            return true;
        }
	
        Log("BepInEx is not installed!");
        return false;
    }

    void WipeMods(std::string dir)
    {
        //in order to do this, we have to delete four things!
        //1. The BepInEx folder
        //2. The doorstop_config.ini file
        //3. The winhttp.dll file
        //4. The changelog.txt file
        int count = 0;
        std::string path = dir + "/BepInEx";
        std::string path2 = dir + "/doorstop_config.ini";
        std::string path3 = dir + "/winhttp.dll";
        std::string path4 = dir + "/changelog.txt";

        if (std::filesystem::exists(path))
	    {
    	    Log("Deleting " + path);
    	    std::filesystem::remove_all(path);
            count++;
        }

        if (std::filesystem::exists(path2))
        {
    	    Log("Deleting " + path2);
    	    std::filesystem::remove(path2);
            count++;
        }

        if (std::filesystem::exists(path3))
	    {
    	    Log("Deleting " + path3);
    	    std::filesystem::remove(path3);
		    count++;
        }

        if (std::filesystem::exists(path4))
        {
    	    Log("Deleting " + path4);
    	    std::filesystem::remove(path4);
            count++;
        }

        if (count > 0)
            Log("Wiped mods!");
        else
            Log("Nothing found to wipe! Was bepinex installed?");
    }

    void OpenDirectory(std::string dir) 
    {
        std::string command = "explorer \"" + dir + "\" 2>&1";

        FILE* pipe = _popen(command.c_str(), "r");
        if (!pipe) {
            Log("Error: Unable to open pipe.");
            return;
        }

        char buffer[128];
        while (fgets(buffer, 128, pipe) != NULL) {
            Log(buffer);
        }

        _pclose(pipe);
    }

    void DeleteMod(std::string dir, std::string modName) 
	{
    	std::string path = dir + "/BepInEx/plugins/" + modName;
    	if (std::filesystem::exists(path))
    	{
            Log("Deleting " + path);
            std::filesystem::remove(path);
        }
    }

    void DisableMod(std::string dir, std::string modName) 
    {
        std::string path = dir + "/BepInEx/plugins/" + modName;
        std::string disabledPath = path + ".ddloader";

        if (std::filesystem::exists(path)) {
            Log("Disabling " + modName);
            std::filesystem::rename(path, disabledPath);
        }
    }

    void EnableMod(std::string dir, std::string modName)
    {
        std::string path = dir + "/BepInEx/plugins/" + modName;
        std::string enabledPath = path.substr(0, path.size() - 9);

        if (std::filesystem::exists(path)) {
            Log("Enabling " + modName);
            std::filesystem::rename(path, enabledPath);
        }
    }

};

