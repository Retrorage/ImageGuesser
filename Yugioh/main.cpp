#include <iostream>
#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <json/json.h>
#include <time.h>

constexpr inline sf::Vector2i levelValue[4] = {{30, 30}, {60, 60}, {90, 90}, {136, 136}};
constexpr inline sf::Vector2f windowSize = {1000.0, 500.0};
constexpr inline int lineAmount = 45;

inline unsigned int level = 1; //For Image Level
inline unsigned int type = 1; //For Random Info
inline unsigned int cardValue = 1;
inline bool valid = false;
inline sf::Rect<int> rect;
inline sf::RectangleShape card;
sf::Font font;
sf::Texture tex;
//Monster Random Info
//type 1: Stars, Atk, Def
//type 2: Attribute, Race, type
//type 3: desc

//Spell / Trap Random Info
//type 1: type + race type
//type 2: Set
//type 3: desc


bool returnRandomCard(Json::Value& ygoData, std::string& stringRef) {
    int numberOfYugiohCards = ygoData["data"].size();
    stringRef.clear();
    level = 0;
    std::cout << "Number of Cards: " << numberOfYugiohCards << std::endl;
    srand(time(NULL));
    cardValue = rand() % numberOfYugiohCards;
    type = (rand() % 3);
    type++;
    std::cout << "Random Number: " << cardValue << std::endl;
    if(ygoData["data"].isValidIndex(cardValue)) {
        std::cout << cardValue << ": " << ygoData["data"][cardValue]["name"].asString() << std::endl;
    }
    else {
        std::cout << "Failed to pull card." << std::endl;
        return false;
    }
    std::string cardPath = "pics/" + ygoData["data"][cardValue]["id"].asString() + ".jpg";
    if(tex.loadFromFile(cardPath)) {
        std::cout << "Successful Load" << std::endl;
        sf::Vector2i rectPos;
        rectPos.x = 21 + (rand() % (136 + 1 - levelValue[level].x));
        rectPos.y = 45 + (rand() % (136 + 1 - levelValue[level].x));
        rect = sf::Rect<int>(rectPos, levelValue[level]);
        card.setTexture(&tex);
        card.setTextureRect(rect);
    }
    else {
        std::cout << "Failed to load" << std::endl;
        return false;
    }
    return true;
}

void nextLevel(Json::Value& ygoData, std::string& stringRef) { //Does not store history so going back is more complex than I am willing to put time in LMAO
    if(level < 3) {
        level++;
        sf::Vector2i rectPos;
        rectPos.x = 21 + (rand() % (136 + 1 - levelValue[level].x));
        rectPos.y = 45 + (rand() % (136 + 1 - levelValue[level].x));
        rect = sf::Rect<int>(rectPos, levelValue[level]);
        card.setTextureRect(rect);
        if(level >= 1) {
            //Do thing
            std::string tempString = ygoData["data"][cardValue]["type"].asString();
            std::size_t found = tempString.find("Monster");
            std::string effectString = ygoData["data"][cardValue]["desc"].asString();
            int spacing = 0;
            if(found != std::string::npos) { //Monster Cards
            switch(type) {
            case 1:
                stringRef.append("Level: ");
                stringRef.append(ygoData["data"][cardValue]["level"].asString());
                stringRef.append("\n");
                stringRef.append("Atk: ");
                stringRef.append(ygoData["data"][cardValue]["atk"].asString());
                stringRef.append("\n");
                stringRef.append("Def: ");
                stringRef.append(ygoData["data"][cardValue]["def"].asString());
                stringRef.append("\n");
                break;
            case 2:
                stringRef.append("Attribute: ");
                stringRef.append(ygoData["data"][cardValue]["attribute"].asString());
                stringRef.append("\n");
                stringRef.append("Type: ");
                stringRef.append(ygoData["data"][cardValue]["race"].asString());
                stringRef.append("\n");
                stringRef.append("Card Type: ");
                stringRef.append(ygoData["data"][cardValue]["type"].asString());
                stringRef.append("\n");
                break;
            case 3:
                for(std::string::iterator itStr = effectString.begin(); itStr != effectString.end(); itStr++) {
                    stringRef.append(itStr, itStr+1);
                    spacing++;
                    if(spacing > lineAmount && *itStr == ' ') { //Letter Limit new line
                        stringRef.append("\n");
                        spacing = 0;
                    }
                }
                stringRef.append("\n");
                break;
            default:
                std::cout << "Error Lmao" << std::endl;
                break;
            }
            }
            else { //Magics and Traps
            switch(type) {
            case 1:
                stringRef.append("Card Type: ");
                stringRef.append(ygoData["data"][cardValue]["race"].asString());
                stringRef.append(" ");
                stringRef.append(ygoData["data"][cardValue]["type"].asString());
                stringRef.append("\n");
                break;
            case 2:
                stringRef.append("Set Origin: ");
                stringRef.append(ygoData["data"][cardValue]["card_sets"][0]["set_name"].asString());
                stringRef.append("\n");
                break;
            case 3:
                for(std::string::iterator itStr = effectString.begin(); itStr != effectString.end(); itStr++) {
                    stringRef.append(itStr, itStr+1);
                    spacing++;
                    if(spacing > lineAmount && *itStr == ' ') { //Letter Limit new line
                        stringRef.append("\n");
                        spacing = 0;
                    }
                }
                stringRef.append("\n");
                break;
            default:
                std::cout << "Error Lmao" << std::endl;
                break;
            }
            }
            type++;
            if(type > 3) //Check OverFlow
                type = 1;
        }
    }
}

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

    sf::RenderWindow window;
    sf::Text text(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(18);
    text.setLineSpacing(2);
    text.setPosition({windowSize.x / 2, 0});
    std::string info;
    if(!tex.loadFromFile("pics/hidden.png")) {
        std::cout << "Failed To Load Texture" << std::endl;
        return 1;
    }
    card.setPosition({0.0, 0.0});
    card.setSize({136.0, 136.0});
    card.setScale({windowSize.y / 136.0, windowSize.y / 136.0});
    card.setTexture(&tex);
    std::optional<sf::Event> event;
    window.create(sf::VideoMode(sf::Vector2u(windowSize.x, windowSize.y)), "Yugioh Image Guessor");
    window.setKeyRepeatEnabled(false);

    while(window.isOpen()) {
        while((event = window.pollEvent())) {
            if(event->is<sf::Event::Closed>())
                window.close();
            if(event->is<sf::Event::KeyPressed>()) {
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
                    //Finish Current / Go all the way to the end
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
