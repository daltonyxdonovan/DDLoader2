/*

                    
                                                  DDLOADER 2
                                        5TH GENERATION SUCCESSOR TO LOAD-R
                              -LOADS GAMES BY JSON FILES, NO MORE HARD CODING GAMES :)
                                        
                                                 

                            (can you believe this all started as a patcher for friends?)

                  NOTES:
                    -This is a complete rewrite of the original LOAD-R (again)
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
#include <string>
#include "UI.h"
#include "Game.h"
#include "Titlebar.h"
#include "Button.h"
#include "Mod.h"
#include "Game.h"
#include "json.hpp"

//disable warnings
#define _CRT_SECURE_NO_WARNINGS

using json = nlohmann::json;

class Tester
{
public:
    sf::RectangleShape rect;
    sf::Text text;
    std::string name;
    sf::Vector2f position;
    sf::Font* font;

    Tester(std::string names, sf::Font* font, bool recognised)
    {
        this->name = names;
        this->font = font;
        this->position = sf::Vector2f((rand() % 200) + 300, -20);
        this->text.setString(name);
        this->text.setFillColor(sf::Color::Black);
        this->text.setCharacterSize(25);
        this->text.setFont(*this->font);
        this->text.setOrigin(this->text.getGlobalBounds().width/2,this->text.getGlobalBounds().height/2+5);
        this->text.setPosition(position);
        this->rect = sf::RectangleShape(sf::Vector2f(this->text.getGlobalBounds().width+10, this->text.getGlobalBounds().height+10));
        this->rect.setFillColor(sf::Color(0,148,148));
        this->rect.setOrigin(this->rect.getGlobalBounds().width/2,this->rect.getGlobalBounds().height/2);
        this->rect.setPosition(position);
        this->rect.setOutlineThickness(2);
        this->rect.setOutlineColor(sf::Color(0,100,100));

        if (recognised)
        {
            this->rect.setOutlineColor(sf::Color(0,240,240));
            this->rect.setFillColor(sf::Color(0,214,214));
        }
    }

    void Update()
	{
    	this->position.y += 1;
    	this->rect.setPosition(this->position);
    	this->text.setPosition(this->position);
    }

    void Draw(sf::RenderWindow& window)
	{
        window.draw(this->rect);
    	window.draw(this->text);
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

};

class Confetti
{
public:
    sf::Vector2f position;
    bool flip = false;
    sf::RectangleShape rect;
    sf::Color color;
    sf::Texture* texture1;
    sf::Texture* texture2;
    int ticker = 0;
    bool right;
    int rightTicker;
    float rotation = 0;

    Confetti(sf::Vector2f position, sf::Texture* texture, sf::Texture* texture2, sf::Color color)
	{
    	this->position = position;
    	this->color = sf::Color(rand()%255,rand()%255,rand()%255);
    	this->rect = sf::RectangleShape(sf::Vector2f(50,50));
    	this->rect.setFillColor(color);
    	this->rect.setPosition(this->position);
        this->texture1 = texture;
        this->texture2 = texture2;
        this->rect.setTexture(this->texture1);
        this->rect.setOrigin(this->rect.getGlobalBounds().width/2,this->rect.getGlobalBounds().height/2);
        this->right = rand()%2;
        this->rightTicker = 0;
    }

    void Update()
    {
        ticker++;
        if (ticker > 100)
        {
            flip = !flip;
            if (flip)
            {
                this->rect.setTexture(this->texture2);
            }
            else
            {
                this->rect.setTexture(this->texture1);
            }
            ticker = 0;
        }

        rightTicker++;
        if (rightTicker>100)
        {
            right = !right;
			rightTicker = 0;
        }

        if (right)
        {
            position.x += .1f;
            rotation+=3;

        }
        else
        {
            position.x -= .1f;
            rotation-=3;
        }



        this->position.y += 5;
        this->rect.setPosition(this->position);

        this->rect.setRotation(rotation);
    }

    void Draw(sf::RenderWindow& window)
    {
        window.draw(this->rect);
    }

};

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
std::vector<Tester> testers;
bool isSecretOpen = false;
int secretTicker = 0;
int names1Ticker = 0;
int names2Ticker = 0;
bool flip = true;
int count = 0;
std::vector<Confetti> confetti;
sf::Texture tex1;
sf::Texture tex2;
std::vector<std::string> recognised = { "CameraMan", "DHL Erzfeind", "latest_version", "Traube", "Wilde Pommes YT", "QuackAndCheese", "THANK YOU ALL SO MUCH <3", "FOR TESTING/BEING COOL/WHATEVER IT WAS!"};
std::vector<std::string> testersn = { "Azurii", "Bill", "Blammo", "Cookie", "Dimi", "DomPaul11", "Memepoleon", "nico", "Pixel Yoshi", "PlushEdits(Backup)", "Speedy612", "TarikGamer7" };
sf::Color color1 = sf::Color(217, 7, 24);
sf::Color color2 = sf::Color(15, 140, 65);
sf::Color color3 = sf::Color(43, 43, 128);
sf::Color color4 = sf::Color(230, 205, 18);
sf::RectangleShape shape1(sf::Vector2f(800,800));



int main()
{
    shape1.setFillColor(sf::Color(0,0,0, 200));
    shape1.setPosition(0,0);

    sf::RenderWindow window(sf::VideoMode(width, height), "DDLOADER <3", sf::Style::None);
    font.loadFromFile("resources/fonts/monospace.ttf");
    WipeLog();
    LoadGames(games);
    UI ui(games);
    Titlebar titleBar(sf::Vector2f(400,20), sf::Vector2f(804,40), sf::Color(0,148,148));
    srand(time(NULL));
    tex1.loadFromFile("resources/images/confetti2.png");
    tex2.loadFromFile("resources/images/confetti.png");

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

        if (isSecretOpen)
        {
            count++;


            if (count%2 == 0)
            {
                int choice = rand() % 4;


                if (choice == 0)
                    confetti.push_back(Confetti(sf::Vector2f(rand() % 800, -20), &tex1, &tex2, color1));
                else if (choice == 1)
                    confetti.push_back(Confetti(sf::Vector2f(rand() % 800, -20), &tex2, &tex1, color2));
                else if (choice == 2)
                    confetti.push_back(Confetti(sf::Vector2f(rand() % 800, -20), &tex2, &tex2, color4));
                else
                    confetti.push_back(Confetti(sf::Vector2f(rand() % 800, -20), &tex1, &tex1, color3));


            }

            

            
            if (count > 2400)
            {
            	isSecretOpen = false;
            	count = 0;
                confetti.clear();
                testers.clear();
            }
            if (secretTicker > 0)
                secretTicker--;
            if (secretTicker == 0)
            {
                secretTicker = 40;
                if (flip)
                {
                	testers.push_back(Tester(recognised[names1Ticker], &font, true));
                    names1Ticker++;
                    if (names1Ticker> recognised.size()-1)
						names1Ticker = 0;
                    flip = !flip;
                }
				else
				{
                	testers.push_back(Tester(testersn[names2Ticker], &font, true));
                    names2Ticker++;
					if (names2Ticker> testersn.size()-1)
                        names2Ticker = 0;
                    flip = !flip;
                }
            }
        }

        if (!isSettingsOpen)
        {
            ui.updateMain(mousePosition, window, locked);
            ui.drawMain(window);
        }
        else
        {
            ui.updateSettings(mousePosition,window,locked,isSecretOpen);
            ui.drawSettings(window);
        }

        if (isSecretOpen)
        {
            
            window.draw(shape1);
            for (int i = 0; i < confetti.size(); i++)
            {
                confetti[i].Draw(window);
				confetti[i].Update();
            }

            for (int i = 0; i < testers.size(); i++)
            {
                testers[i].Draw(window);
                testers[i].Update();
            }
        }

        for (int i = 0; i < testers.size(); i++)
        {
            if (testers[i].position.y > 820)
            {
            	testers.erase(testers.begin() + i);
            }
        }

        for (int i = 0; i < confetti.size(); i++)
        {
            if (confetti[i].position.y > 820)
			{
            	confetti.erase(confetti.begin() + i);
            }
        }

        titleBar.update(mousePosition, window, isSettingsOpen);
        titleBar.draw(window);

        window.setFramerateLimit(60);
        window.display();
    }

    return 0;
}