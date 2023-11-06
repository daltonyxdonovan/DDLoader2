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
#include <Windows.h>
#include <ShlObj.h>
#include "UI.h"
#include "Game.h"
#include "Titlebar.h"
#include "Button.h"
#include "Mod.h"
#include "Game.h"
#include "json.hpp"
using json = nlohmann::json;

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
bool isSettingsOpen = false;

int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "DDLOADER <3", sf::Style::None);
    WipeLog();
    LoadGames(games);
    UI ui(games);
    Titlebar titleBar(sf::Vector2f(400,20), sf::Vector2f(804,40), sf::Color(0,148,148));

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
        }

        window.clear(sf::Color(0,116,116));

        ui.update(mousePosition, window, locked);
        ui.draw(window);

        titleBar.update(mousePosition, window, isSettingsOpen);
        titleBar.draw(window);

        window.setFramerateLimit(30);
        window.display();
    }

    return 0;
}