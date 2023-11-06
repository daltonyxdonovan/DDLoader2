#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"


class Titlebar
{
public:
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Color color;
    std::vector<Button*> buttons;
    bool isHeld = false;
    sf::RectangleShape rect;
    sf::Font font;
    sf::Text title;


    Titlebar(sf::Vector2f position, sf::Vector2f size, sf::Color color)
    {
        this->position = position;
        this->size = size;
        this->color = color;
        buttons = std::vector<Button*>();
        this->isHeld = false;
        this->rect.setSize(size);
        this->rect.setFillColor(color);
        this->rect.setOutlineColor(sf::Color::Black);
        this->rect.setOutlineThickness(1);
        this->rect.setOrigin(this->rect.getGlobalBounds().width/2,this->rect.getGlobalBounds().height/2);
        this->font.loadFromFile("resources/fonts/JetBrainsMono-Regular.ttf");
        this->title.setString("DDLOADER2");
        this->title.setFillColor(sf::Color::Black);
        this->title.setCharacterSize(25);
        this->title.setFont(font);
        this->title.setOrigin(this->title.getGlobalBounds().width/2,this->title.getGlobalBounds().height/2+5);
        this->title.setPosition(position);
        this->rect.setPosition(position);
        this->buttons.push_back(new Button("x", sf::Vector2f(position.x+380,position.y)));
        this->buttons.push_back(new Button("+", sf::Vector2f(position.x-380,position.y)));
        this->buttons[1]->setSize(this->buttons[0]->getSize());
    }

    void LockButtons()
    {
        buttons[0]->ticker = 60;
        buttons[1]->ticker = 60;
        buttons[0]->state = DEFAULT;
        buttons[1]->state = DEFAULT;
    }

    void update(sf::Vector2i mousePosition, sf::RenderWindow& window, bool& isSettingsOpen)
    {
        buttons[0]->update(mousePosition);
        buttons[1]->update(mousePosition);

        if (buttons[0]->isClicked(window))
        {
            window.close();
        }

        if (buttons[1]->isClicked(window))
        {
            isSettingsOpen = !isSettingsOpen;
            LockButtons();
        }
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(rect);
        window.draw(title);
    
        buttons[0]->draw(window);
        buttons[1]->draw(window);
    }
    
};
