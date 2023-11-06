/*

                    
                                                  DDLOADER 2
                                        5TH GENERATION SUCCESSOR TO LOAD-R
                              -LOADS GAMES BY JSON FILES, NO MORE HARD CODING GAMES :)
                                        
                                                 

                            (can you believe this all started as a patcher for friends?)

                  NOTES:
                    -This is a complete rewrite of the original LOAD-R (again)
                    -Yes, all the classes are in the source file for now- I'm lazy
                    -Why does CountFilesInDir() run twice? Should look up quirks of cpp
                    -std::vector<> solves everything.
                    -Maybe I should add all the funky colours and switcher back?
                    -Will do what DDLoader-Updater did (since I never released it), and can easily
                    add a way to update the games list without having to redownload the
			         entire program, since it can access my website or github on program boot!
                    -Yes, this will make it ping as a virus for 'phoning home', but I don't care /shrug
                                    (what doesn't show as a virus nowadays?)

                    -and most importantly:

                 _____  __ __    __  __  _      ____   ____  ____  ___        ___ ___   ___   ___   _____
                |     ||  |  |  /  ]|  |/ ]    |    \ /    ||    ||   \      |   |   | /   \ |   \ / ___/
                |   __||  |  | /  / |  ' /     |  o  )  o  | |  | |    \     | _   _ ||     ||    (   \_ 
                |  |_  |  |  |/  /  |    \     |   _/|     | |  | |  D  |    |  \_/  ||  O  ||  D  \__  |
                |   _] |  :  /   \_ |     \    |  |  |  _  | |  | |     |    |   |   ||     ||     /  \ |
                |  |   |     \     ||  .  |    |  |  |  |  | |  | |     |    |   |   ||     ||     \    |
                |__|    \__,_|\____||__|\_|    |__|  |__|__||____||_____|    |___|___| \___/ |_____|\___|   <3 me
                                                                                         

                                       

*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <string>
#include "json.hpp"
#include <Windows.h>
#include <ShlObj.h>
using json = nlohmann::json;

#pragma region CLASSES/ENUM

enum ButtonState { HOVER, PRESSED, DEFAULT };


class Game
{
public:
    std::string name;
    std::string description;
    std::string genre;
    std::string platform;
    std::string installLocation;
    std::string bepinexVersion;
    sf::Texture image;

    Game(const std::string& name, const std::string& description, const std::string& genre, const std::string& platform, const std::string& installLocation, const std::string& bepinexVersion, const sf::Texture& image)
        : name(name), description(description), genre(genre), platform(platform), installLocation(installLocation), bepinexVersion(bepinexVersion), image(image) 
    {
        //screw you, textures. we'll just load you directly -_-
    }

    void print()
	{
    	std::cout << "Name               : " << name << std::endl;
    	std::cout << "Description        : " << description << std::endl;
    	std::cout << "Genre              : " << genre << std::endl;
    	std::cout << "Platform           : " << platform << std::endl;
    	std::cout << "Install Location   : " << installLocation << std::endl;
    	std::cout << "Bepinex Version    : " << bepinexVersion << std::endl;
    }
};

class Button
{
public:

    sf::RectangleShape rect;
    sf::Text text;
    sf::Font font;
    ButtonState state;
    int ticker = 30;

    Button() 
    {
        ticker = 30;
        this->rect = sf::RectangleShape();
        this->text = sf::Text();
        this->font.loadFromFile("resources/fonts/JetBrainsMono-Regular.ttf");
        this->text.setFont(font);
        this->text.setString("textToDisplay");
        this->text.setCharacterSize(15);
        this->text.setFillColor(sf::Color::Black);
        this->text.setStyle(sf::Text::Bold);
        this->rect.setSize(sf::Vector2f(text.getLocalBounds().width + 10, text.getLocalBounds().height + 10));
        this->rect.setFillColor(sf::Color(150,150,150));
        this->rect.setOutlineThickness(2);
        this->rect.setOutlineColor(sf::Color::Black);
        this->text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2.0f, text.getLocalBounds().top + text.getLocalBounds().height / 2.0f);
        this->text.setPosition(sf::Vector2f(0,0));
        this->rect.setOrigin(rect.getLocalBounds().left + rect.getLocalBounds().width / 2.0f, rect.getLocalBounds().top + rect.getLocalBounds().height / 2.0f);
        this->rect.setPosition(sf::Vector2f(0,0));
        this->state = DEFAULT;
    }


    Button(std::string textToDisplay, sf::Vector2f position)
    {
        ticker = 30;
        this->rect = sf::RectangleShape();
        this->text = sf::Text();
        font.loadFromFile("resources/fonts/JetBrainsMono-Regular.ttf");
        text.setFont(font);
        text.setString(textToDisplay);
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Black);
        text.setStyle(sf::Text::Bold);

        text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2.0f, text.getLocalBounds().top + text.getLocalBounds().height / 2.0f);
        text.setPosition(position);

        rect.setSize(sf::Vector2f(text.getLocalBounds().width + 10, text.getLocalBounds().height + 10));
        rect.setOrigin(rect.getLocalBounds().left + rect.getLocalBounds().width / 2.0f, rect.getLocalBounds().top + rect.getLocalBounds().height / 2.0f);
        rect.setPosition(position);

        rect.setFillColor(sf::Color(200,200,200));
        rect.setOutlineThickness(2);
        rect.setOutlineColor(sf::Color::Black);

        state = DEFAULT;
    }

    bool isClicked(sf::RenderWindow& window)
    {
        if (ticker == 0)
        {
            
            return state == PRESSED;
        }
        return false;
    }

    void update(sf::Vector2i mousePosition)
    {
        if (ticker > 0)
            ticker--;
        if (state == HOVER)
		{
        	rect.setFillColor(sf::Color(0, 241, 241));
        }
		else if (state == PRESSED)
		{
        	rect.setFillColor(sf::Color(0, 214, 214));
        }
		else
		{
        	rect.setFillColor(sf::Color(0, 180, 180));
        }

        if (rect.getGlobalBounds().contains(mousePosition.x,mousePosition.y))
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                state = PRESSED;
            else
                state = HOVER;
        }
		else
		{
        	state = DEFAULT;
        }
    }

    void draw(sf::RenderWindow& window)
	{
    	window.draw(rect);
    	window.draw(text);
    }

    void setSize(sf::Vector2f size)
    {
        this->rect.setSize(size);
        rect.setOrigin(rect.getLocalBounds().left + rect.getLocalBounds().width / 2.0f, rect.getLocalBounds().top + rect.getLocalBounds().height / 2.0f);
    }
};

class Mod
{
public:
    std::string name;
    bool isDisabled;
    sf::RectangleShape rect;
    sf::Text text;
    sf::Font font;
    std::vector<Button*> buttons;
    sf::Vector2f position;

    Mod(const std::string& name, bool isDisabled, sf::Vector2f position) : name(name), isDisabled(isDisabled), position(position)
    {
        this->rect = sf::RectangleShape();
		this->text = sf::Text();
		this->font.loadFromFile("resources/fonts/JetBrainsMono-Regular.ttf");
		this->text.setFont(font);
		this->text.setString(name);
		this->text.setCharacterSize(15);
		this->text.setFillColor(sf::Color::Black);
		this->text.setStyle(sf::Text::Bold);
		this->rect.setSize(sf::Vector2f(780, 40));
		this->rect.setFillColor(sf::Color(0,148,148));
		this->rect.setOutlineThickness(1);
		this->rect.setOutlineColor(sf::Color::Black);
		this->text.setOrigin(0, text.getLocalBounds().top + text.getLocalBounds().height / 2.0f);
		this->text.setPosition(sf::Vector2f(position.x-365,position.y));
		this->rect.setOrigin(rect.getLocalBounds().left + rect.getLocalBounds().width / 2.0f, rect.getLocalBounds().top + rect.getLocalBounds().height / 2.0f);
		this->rect.setPosition(sf::Vector2f(position.x,position.y));
        this->buttons.push_back(new Button("DISABLE", sf::Vector2f(position.x+260,position.y)));
		this->buttons.push_back(new Button("DELETE", sf::Vector2f(position.x+345,position.y)));
        this->buttons.push_back(new Button("ENABLE", sf::Vector2f(position.x+260,position.y)));
        
    }

    ~Mod()
    {
    	for (auto& button : buttons)
    		delete button;
    }

    void update(sf::Vector2i mousePosition)
	{
        if (isEnabled())
            rect.setFillColor(sf::Color(0,148,148));
        else
            rect.setFillColor(sf::Color(0,96,96));

    	buttons[1]->update(mousePosition);
        if (isEnabled())
			buttons[0]->update(mousePosition);
		else
			buttons[2]->update(mousePosition);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(rect);
		window.draw(text);
		buttons[1]->draw(window);
        if (isEnabled())
            buttons[0]->draw(window);
        else
            buttons[2]->draw(window);
    }

    bool isEnabled()
    {
        //return true if name ends in '.ddloader'
        return name.substr(name.length()-9,9) != ".ddloader";
    }
    
};

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
    
    UI(std::vector<Game> games)
    {
#pragma UIINIT
        noModsText.setString("No mods found!");
        noModsText.setFont(font);
        noModsText.setCharacterSize(60);
        noModsText.setFillColor(sf::Color(0,100,100));
        noModsText.setStyle(sf::Text::Bold);
        
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
    }

    void update(sf::Vector2i mousePosition, sf::RenderWindow& window, bool locked)
    {
        if (locked)
            return;
        if (!isModManagerOpen)
        {
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

        if (SUCCEEDED(hr)) {
            COMDLG_FILTERSPEC fileTypes[] = { L"ZIP Files", L"*.zip" };
            openFileDialog->SetFileTypes(1, fileTypes);

            hr = openFileDialog->Show(NULL);

            if (SUCCEEDED(hr)) {
                IShellItem* pItem;
                hr = openFileDialog->GetResult(&pItem);

                if (SUCCEEDED(hr)) {
                    PWSTR filePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

                    if (SUCCEEDED(hr)) {
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

    void draw(sf::RenderWindow& window)
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

class Titlebar
{
public:
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Color color;
    std::vector<Button*> buttons;
    bool isHeld = false;

    Titlebar(sf::Vector2f position, sf::Vector2f size, sf::Color)
    {
        this->position = position;
        this->size = size;
        this->color = color;
        this->buttons.push_back(new Button("x", sf::Vector2f(position.x+260,position.y)));
        this->buttons.push_back(new Button("#", sf::Vector2f(position.x-260,position.y)))
    }
    
};

#pragma endregion


#pragma region METHODS
int CountFilesInDir(const std::string& path) 
{
    //counts ALL files, not just .json files
    int count;
    count = std::count_if(std::filesystem::directory_iterator(path), std::filesystem::directory_iterator(), static_cast<bool(*)(const std::filesystem::path&)>(std::filesystem::is_regular_file));
    return count;
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

void LoadGames(std::vector<Game>& games) 
{
    std::string path = "resources/games/";
    std::string extension = ".json";
    std::string pathImage = "resources/images/";

    for (int i = 0; i < CountFilesInDir(path); i++) 
    {
        std::string filename = "game" + std::to_string(i) + extension; // Assuming file names are like game0.json or game1.json
        std::ifstream file;
        file.open(path + filename); // Open the file

        if (file.is_open()) 
        {
            json j;
            file >> j;

            std::string nameToUse = j["name"];
            std::string descriptionToUse = j["description"];
            std::string genreToUse = j["genre"];
            std::string platformToUse = j["platform"];
            std::string installLocation = j["installLocation"];
            sf::Texture image;

            std::string imagePath = pathImage + "game" + std::to_string(i) + ".png";
            image.loadFromFile(imagePath);

            std::string bepinexVersionToUse = j["bepinexVersion"];

            Game game(nameToUse, descriptionToUse, genreToUse, platformToUse, installLocation, bepinexVersionToUse, image);

            games.push_back(game);
            file.close();
        }
    }

    for (int i = 0; i < games.size(); i++) 
    {
        Log("Loaded assets for " + games[i].name);
    }
}

void WipeLog()
{
    std::ofstream file("log-ddloader.txt", std::ios_base::trunc);
	if (file.is_open())
	{
    	file << "";
    	file.close();
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


#pragma endregion


const int width = 800;
const int height = 800;

sf::Font font;
sf::Text programTitle;
sf::RectangleShape programTitlebar;
sf::Vector2i mousePosition;
std::vector<Game> games;
bool locked = false;

int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "DDLOADER <3", sf::Style::None);
    font.loadFromFile("resources/fonts/JetBrainsMono-Regular.ttf");
    programTitle.setFont(font);
    programTitle.setString("DDLOADER2");
    programTitle.setCharacterSize(24);
    programTitle.setFillColor(sf::Color::Black);
    programTitle.setOrigin((int)programTitle.getLocalBounds().width / 2, (int)programTitle.getLocalBounds().height / 2);
    programTitle.setPosition(width / 2, 15);
    programTitlebar.setSize(sf::Vector2f(width, 40));
    programTitlebar.setFillColor(sf::Color(0, 148, 148));
    programTitlebar.setPosition(0, 0);
    programTitlebar.setOutlineColor(sf::Color::Black);
    programTitlebar.setOutlineThickness(1);

    WipeLog();
    LoadGames(games);
    UI ui(games);
    Button buttonExit("x", sf::Vector2f(width-20,20)); //I hate the windows title bars man, they're ugly as hell. we will just use our own, thanks

    while (window.isOpen())
    {
        if (!window.hasFocus())
            locked = true;
		else
			locked = false;
        mousePosition = sf::Mouse::getPosition(window);
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }

        if (buttonExit.isClicked(window) && !locked)
            window.close();

        window.clear(sf::Color(0,116,116));

        ui.update(mousePosition, window, locked);
        ui.draw(window);

        window.draw(programTitlebar);
        window.draw(programTitle);

        buttonExit.update(mousePosition);
        buttonExit.draw(window);

        window.setFramerateLimit(30);
        window.display();
    }

    return 0;
}