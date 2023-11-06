#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Game.h"



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


