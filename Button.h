#pragma once
#include <SFML/Graphics.hpp>
#include "ButtonState.h"



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

    sf::Vector2f getSize()
    {
        return rect.getSize();
    }
};
