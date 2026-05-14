#include <iostream>
#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <json/json.h>
#include <time.h>

#include "setup.hpp"

int main() {
    if(font.openFromFile("monoFonto.ttf"))
        std::cout << "Successfully Loaded Font" << std::endl;
    else {
        std::cout << "Failed to Load Font" << std::endl;
        return 1;
    }
    std::string filePath = "yugiohcardinfo.json";
    std::ifstream file(filePath, std::ifstream::binary);

    if(!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return 1;
    }

    Json::Value data;
    Json::CharReaderBuilder readerBuilder;
    std::string errs;

    if(!Json::parseFromStream(readerBuilder, file, &data, &errs)) {
        std::cerr << "Error Parsing JSON: " << errs << std::endl;
        return 1;
    }
    file.close();
    //std::cout << "Parsed JSON data:" << std::endl;
    //std::cout << data << std::endl;
    SetUp();
    sf::RenderWindow window;
    sf::Text text(font);
    bool typing = false;
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(18);
    text.setLineSpacing(2);
    text.setPosition({windowSize.x / 2, 0});
    std::string info;
    card.setPosition({0.0, 0.0});
    card.setSize({136.0, 136.0});
    card.setScale({windowSize.y / 136.0, windowSize.y / 136.0});
    card.setTexture(&tex);
    std::optional<sf::Event> event;
    window.create(sf::VideoMode(sf::Vector2u(windowSize.x, windowSize.y)), "Yugioh Image Guessor");
    window.setKeyRepeatEnabled(false);
    std::cout << "Press D for more Info (level 1 to Level 4)" << std::endl <<
    "Press Space for next card" << std::endl <<
    "Press Enter to submit a guess!" << std::endl;
    while(window.isOpen()) {
        while((event = window.pollEvent())) {
            if(event->is<sf::Event::Closed>())
                window.close();
            if(event->is<sf::Event::KeyPressed>() && !typing) {
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Space) {
                    //Next Card
                    valid = returnRandomCard(data, info);
                    text.setString(info);
                }
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::D && valid) {
                    //Next Image / Info
                    nextLevel(data, info);
                    text.setString(info);
                }
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter && valid) {
                    typing = true;
                    char guess[201];
                    std::strcpy(guess, "");
                    std::cout << "Type Your Answer(Case Sensitive!!!): ";
                    std::cin.getline(guess, 200);
                    std::cout << "Checking" << std::endl;
                    if(guess == data["data"][cardValue]["name"].asString())
                        std::cout << "Correct!!!" << std::endl;
                    else
                        std::cout << "Incorrect, PepoSadge or Impossible to Type LMAO" << std::endl;
                    typing = false;
                }
            }
        }
        window.clear(sf::Color::Black);
        window.draw(card);
        window.draw(text);
        window.display();
    }
    return 0;
}
