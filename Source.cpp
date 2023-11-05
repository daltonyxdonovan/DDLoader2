/*

                    
                                                  DDLOADER 2
                                        5TH GENERATION SUCCESSOR TO LOAD-R
                              -LOADS GAMES BY JSON FILES, NO MORE HARD CODING GAMES :)
                                        That's the whole reason DDLoader got
                                                no more updates!
                                                 

                              (can you believe this all started as a patcher for friends?
                                  now I code, full-time, to mod video games. lol)

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
#include "json.hpp"
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

    Game(std::string name, std::string description, std::string genre, std::string platform, std::string installLocation, std::string bepinexVersion)
	{
    	this->name = name;
    	this->description = description;
    	this->genre = genre;
    	this->platform = platform;
    	this->installLocation = installLocation;
    	this->bepinexVersion = bepinexVersion;
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

    Button(std::string textToDisplay, sf::Vector2f position)
    {
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

        rect.setFillColor(sf::Color(150,150,150));
        rect.setOutlineThickness(2);
        rect.setOutlineColor(sf::Color::Black);

        state = DEFAULT;
    }

    bool isClicked()
    {
        return state == PRESSED;
    }

    void update(sf::Vector2i mousePosition)
    {
        //if mouse is hovering over button, set state to HOVER
        //if mouse is pressed on button, set state to PRESSED
        //else set state to DEFAULT

        if (state == HOVER)
		{
        	rect.setFillColor(sf::Color(150,150,150));
        }
		else if (state == PRESSED)
		{
        	rect.setFillColor(sf::Color(170,170,170));
        }
		else
		{
        	rect.setFillColor(sf::Color(50,50,50));
        }

        if (rect.getGlobalBounds().contains(mousePosition.x,mousePosition.y))
        {
            //if mouse is clicked
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
    
    UI(std::vector<Game> games)
    {
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
        this->bepinexVersion.setString(games[0].bepinexVersion);

        this->title.setCharacterSize(30);
        this->description.setCharacterSize(15);
        this->genre.setCharacterSize(15);
        this->platform.setCharacterSize(15);
        this->bepinexVersion.setCharacterSize(15);

        this->title.setFillColor(sf::Color::White);
        this->description.setFillColor(sf::Color::White);
        this->genre.setFillColor(sf::Color::White);
        this->platform.setFillColor(sf::Color::White);
        this->bepinexVersion.setFillColor(sf::Color::White);

        this->title.setStyle(sf::Text::Bold);

        this->title.setOrigin((int)this->title.getLocalBounds().width / 2, (int)this->title.getLocalBounds().height / 2);
        this->description.setOrigin((int)this->description.getLocalBounds().width / 2, (int)this->description.getLocalBounds().height / 2);
        this->genre.setOrigin((int)this->genre.getLocalBounds().width / 2, (int)this->genre.getLocalBounds().height / 2);
        this->platform.setOrigin((int)this->platform.getLocalBounds().width / 2, (int)this->platform.getLocalBounds().height / 2);
        this->bepinexVersion.setOrigin((int)this->bepinexVersion.getLocalBounds().width / 2, (int)this->bepinexVersion.getLocalBounds().height / 2);

        this->title.setPosition(100, 50);
        this->description.setPosition(100, 100);
        this->genre.setPosition(100, 150);
        this->platform.setPosition(100, 175);
        this->bepinexVersion.setPosition(100, 200);
    }

    void update(Game game)
    {
        this->title.setString(game.name);
        this->description.setString(game.description);
        this->genre.setString(game.genre);
        this->platform.setString(game.platform);
        this->bepinexVersion.setString(game.bepinexVersion);

        this->title.setOrigin((int)this->title.getLocalBounds().width / 2, (int)this->title.getLocalBounds().height / 2);
        this->description.setOrigin((int)this->description.getLocalBounds().width / 2, (int)this->description.getLocalBounds().height / 2);
        this->genre.setOrigin((int)this->genre.getLocalBounds().width / 2, (int)this->genre.getLocalBounds().height / 2);
        this->platform.setOrigin((int)this->platform.getLocalBounds().width / 2, (int)this->platform.getLocalBounds().height / 2);
        this->bepinexVersion.setOrigin((int)this->bepinexVersion.getLocalBounds().width / 2, (int)this->bepinexVersion.getLocalBounds().height / 2);
    }

    void draw(sf::RenderWindow& window)
	{
    	window.draw(title);
    	window.draw(description);
    	window.draw(genre);
    	window.draw(platform);
    	window.draw(bepinexVersion);
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
        std::cout << "[LOG] " << text << std::endl;
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
            std::string bepinexVersionToUse = j["bepinexVersion"];

            Game game(nameToUse, descriptionToUse, genreToUse, platformToUse, "", bepinexVersionToUse);

            games.push_back(game);
            file.close();
        }
    }

    for (int i = 0; i < games.size(); i++) 
    {
        Log("LOADED ASSET FOR: " + games[i].name);
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


#pragma endregion


const int width = 800;
const int height = 800;

sf::Font font;
sf::Text programTitle;
sf::RectangleShape programTitlebar;

int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "DDLOADER <3", sf::Style::None);
    sf::Vector2i mousePosition;
    std::vector<Game> games;
    font.loadFromFile("resources/fonts/JetBrainsMono-Regular.ttf");
    programTitle.setFont(font);
    programTitle.setString("DDLOADER");
    programTitle.setCharacterSize(24);
    programTitle.setFillColor(sf::Color::Black);
    programTitle.setOrigin((int)programTitle.getLocalBounds().width / 2, (int)programTitle.getLocalBounds().height / 2);
    programTitle.setPosition(width / 2, 15);
    programTitlebar.setSize(sf::Vector2f(width, 40));
    programTitlebar.setFillColor(sf::Color(50, 50, 50, 255));
    programTitlebar.setPosition(0, 0);
    programTitlebar.setOutlineColor(sf::Color::Black);
    programTitlebar.setOutlineThickness(1);




    WipeLog();
    LoadGames(games);
    UI ui(games);
    Button buttonExit("x", sf::Vector2f(width-20,20)); //I hate the windows title bars man, they're ugly as hell. we will just use our own, thanks

    while (window.isOpen())
    {
        mousePosition = sf::Mouse::getPosition(window);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (buttonExit.isClicked())
            window.close();


        window.clear(sf::Color(30,30,30));

        

        ui.draw(window);

        window.draw(programTitlebar);
        window.draw(programTitle);

        buttonExit.update(mousePosition);
        buttonExit.draw(window);

        window.display();
    }

    return 0;
}