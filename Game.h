#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>



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


