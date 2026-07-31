// SPDX-License-Identifier: GPL-3.0-or-later
#include <iostream>
#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <json/json.h>
#include <time.h>
#include <string_view>

#include "ImageGuesser/setup.hpp"

constexpr std::string_view download_string = "download";

int main(int argc, char * argv[]) {
    if(font.openFromFile("assets/font.ttf"))
        std::cout << "Successfully Loaded Font" << std::endl;
    else {
        std::cout << "Failed to Load Font" << std::endl;
        return 1;
    }
    std::string filePath = "assets/yugiohcardinfo.json";
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

    if(argc > 1) {
        if(strlen(argv[1]) == download_string.size()) {
           if(memcmp(download_string.data(), argv[1], download_string.size()) == 0) {
               Download(data);
               return 0;
           }
        }
    }
    //std::cout << "Parsed JSON data:" << std::endl;
    //std::cout << data << std::endl;
    SetUp();
    sf::RenderWindow window;
    sf::Text text(font);
    sf::Text user(font);
    sf::Text directions(font);
    sf::Text number(font);
    number.setFillColor(sf::Color::White);
    number.setOutlineColor(sf::Color::Black);
    number.setOutlineThickness(2);
    number.setCharacterSize(36);
    number.setLineSpacing(1);
    number.setPosition({0, 0});
    number.setString(std::to_string(numberOfCardsGuessed));
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(18);
    text.setLineSpacing(1);
    text.setPosition({windowSize.x / 2, 0});
    directions.setFillColor(sf::Color::Blue);
    directions.setCharacterSize(18);
    directions.setLineSpacing(1);
    directions.setPosition({0, windowSize.y * 5 / 6});
    user.setFillColor(sf::Color::Red);
    user.setCharacterSize(18);
    user.setLineSpacing(1);
    user.setPosition({0, (windowSize.y * 5 / 6) + 75});
    directions.setString(
    "Press D for more Info (level 4,3,2,1,Answer)\nPress Space for next card\nPress Enter to type a guess, Enter again to submit.");
    std::string info;
    std::string guess;
    std::string finalString = "";
    guess = "";
    user.setString(guess);
    card.setPosition({0.0, 0.0});
    card.setSize({maxRes.x * resolutionScale, maxRes.x * resolutionScale});
    card.setScale({(windowSize.y * 5 / 6) / (maxRes.x * resolutionScale), (windowSize.y * 5 / 6) / (maxRes.x * resolutionScale)});
    card.setTexture(&tex);
    std::optional<sf::Event> event;
    window.create(sf::VideoMode(sf::Vector2u(windowSize.x, windowSize.y)), "Yugioh Image Guesser");
    window.setKeyRepeatEnabled(false);

    while(window.isOpen()) {
        while((event = window.pollEvent())) {
            if(event->is<sf::Event::Closed>())
                window.close();
            if(event->is<sf::Event::KeyPressed>() && !typing) {
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Space) {
                    //Next Card
                    valid = returnRandomCard(data, info);
                    if(!valid) {
                        guess = "Not a valid Card (Character Skill / Failed Load etc.), Press Space to get a new card";
                        user.setString(guess);
                    }
                    else {
                        text.setString(info);
                        score = true;
                        guess = "";
                        user.setString(guess);
                        number.setString(std::to_string(numberOfCardsGuessed) +
                                         " | Score: " + std::to_string(teamScore) +
                                         " | " + std::to_string(numberOfCardsGuessed * 4));
                    }
                }
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::D && valid) {
                    //Next Image / Info
                    nextLevel(data, info);
                    text.setString(info);
                    if(level == 4) {
                        guess = "Answer is " + data["data"][cardValue]["name"].asString();
                        user.setString(guess);
                        score = false;
                    }
                }
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter && valid && score) {
                    typing = true;
                    guess = "Type Your Answer(Case Sensitive!!!): ";
                    user.setString(guess);
                    break;
                }
            }
            if(event->is<sf::Event::KeyPressed>() && typing) {
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter) {
                    std::cout << "Checking" << std::endl;
                    finalString = std::string(guess.begin()+37, guess.end());
                    if(finalString == data["data"][cardValue]["name"].asString()) {
                        std::cout << "Correct!!!" << std::endl;
                        guess = "Correct!!!";
                        teamScore =+ teamScore + (5 - (level + 1));
                        score = false;
                    }
                    else {
                        //std::cout << "Incorrect, PepoSadge or Impossible to Type LMAO" << std::endl;
                        guess = "Incorrect, PepoSadge or Impossible to Type LMAO, Try Again!";
                    }
                    user.setString(guess);
                    number.setString(std::to_string(numberOfCardsGuessed) +
                                         " | Score: " + std::to_string(teamScore) +
                                         " | " + std::to_string(numberOfCardsGuessed * 4));
                    typing = false;
                    break;
                }
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Backspace) {
                    if(guess.end() != guess.begin()+37) {
                        guess.pop_back();
                        user.setString(guess);
                    }
                    break;
                }
            }
            if(event->is<sf::Event::TextEntered>() && typing) {
                if(event->getIf<sf::Event::TextEntered>()) {
                switch(event->getIf<sf::Event::TextEntered>()->unicode) {
                case '\r':
                    break;
                case '\b':
                    break;
                default:
                    guess.push_back(event->getIf<sf::Event::TextEntered>()->unicode);
                    user.setString(guess);
                    break;
                }
                }
            }
        }
        window.clear(sf::Color::Black);
        window.draw(card);
        window.draw(text);
        window.draw(directions);
        window.draw(user);
        window.draw(number);
        window.display();
    }
    return 0;
}
