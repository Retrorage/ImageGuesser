#ifndef SETUP_HPP_INCLUDED
#define SETUP_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <json/json.h>
#include <time.h>

constexpr inline sf::Vector2f windowSize = {1000.0, 500.0};
constexpr inline sf::Vector2f resolutionScalePos = {21.0f / 177.0f, 45.0f / 254.0f};
constexpr inline float resolutionScale = 137.0f / 177.0f;
constexpr inline int lineAmount = 45;

inline unsigned int level = 1; //For Image Level
inline unsigned int type = 1; //For Random Info
inline unsigned int cardValue = 1;
inline bool valid = false;
inline sf::Rect<int> rect;
inline sf::RectangleShape card;
sf::Font font;
sf::Texture tex;

inline sf::Vector2f maxRes;
inline sf::Vector2f maxPos;
inline sf::Vector2i levelValue[4];

void SetUp() {
    if(!tex.loadFromFile("pics/55144522.jpg")) {
        std::cout << "Failed To Load default card" << std::endl;
    }

    maxRes = {static_cast<float>(tex.getSize().x), static_cast<float>(tex.getSize().y)};
    maxPos = {maxRes.x * resolutionScalePos.x, maxRes.y * resolutionScalePos.y};
    float boxRes = maxRes.x * resolutionScale;
    levelValue[0] = {static_cast<int>(boxRes * 0.20), static_cast<int>(boxRes * 0.20)};
    levelValue[1] = {static_cast<int>(boxRes * 0.35), static_cast<int>(boxRes * 0.35)};
    levelValue[2] = {static_cast<int>(boxRes * 0.60), static_cast<int>(boxRes * 0.60)};
    levelValue[3] = {static_cast<int>(boxRes * 1.0f), static_cast<int>(boxRes * 1.0f)};
}

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
    if(!ygoData["data"].isValidIndex(cardValue)) {
        std::cout << "Failed to pull card." << std::endl;
        if(!tex.loadFromFile("pics/55144522.jpg"))
            std::cout << "Failed to load default card." << std::endl;
        return false;
    }
    std::string cardPath = "pics/" + ygoData["data"][cardValue]["id"].asString() + ".jpg";
    if(tex.loadFromFile(cardPath)) {
        std::cout << "Successful Load" << std::endl;
        sf::Vector2i rectPos;
        rectPos.x = static_cast<int>(maxPos.x) + (rand() % (static_cast<int>(maxRes.x * resolutionScale) + 1 - levelValue[level].x));
        rectPos.y = static_cast<int>(maxPos.y) + (rand() % (static_cast<int>(maxRes.x * resolutionScale) + 1 - levelValue[level].x));
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
        rectPos.x = static_cast<int>(maxPos.x) + (rand() % (static_cast<int>(maxRes.x * resolutionScale) + 1 - levelValue[level].x));
        rectPos.y = static_cast<int>(maxPos.y) + (rand() % (static_cast<int>(maxRes.x * resolutionScale) + 1 - levelValue[level].x));
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
                //Removing the name of the card from the effect
                for(std::size_t foundName = effectString.find(ygoData["data"][cardValue]["name"].asString()); foundName != std::string::npos;
                foundName = effectString.find(ygoData["data"][cardValue]["name"].asString())) {
                    effectString.erase(foundName, ygoData["data"][cardValue]["name"].asString().size());
                    effectString.insert(foundName, "name");
                }
                //Removing the archetype
                //std::cout << ygoData["data"][cardValue]["archetype"].asString() << std::endl;
                if(!effectString.find(ygoData["data"][cardValue]["archetype"].asString().empty())) {
                for(std::size_t foundName = effectString.find(ygoData["data"][cardValue]["archetype"].asString()); foundName != std::string::npos;
                foundName = effectString.find(ygoData["data"][cardValue]["archetype"].asString())) {
                    effectString.erase(foundName, ygoData["data"][cardValue]["archetype"].asString().size());
                    effectString.insert(foundName, "archetype");
                }
                }
                for(std::string::iterator itStr = effectString.begin(); itStr != effectString.end(); itStr++) {
                    stringRef.append(itStr, itStr+1);
                    spacing++;
                    if(spacing > lineAmount && *itStr == ' ') { //Letter Limit new line
                        stringRef.append("\n");
                        spacing = 0;
                    }
                    if(*itStr == '\n')
                        spacing = 0;
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
                //Removing the name of the card from the effect
                for(std::size_t foundName = effectString.find(ygoData["data"][cardValue]["name"].asString()); foundName != std::string::npos;
                foundName = effectString.find(ygoData["data"][cardValue]["name"].asString())) {
                    effectString.erase(foundName, ygoData["data"][cardValue]["name"].asString().size());
                    effectString.insert(foundName, "{name}");
                }
                //Removing the archetype
                if(!effectString.find(ygoData["data"][cardValue]["archetype"].asString().empty())) {
                for(std::size_t foundName = effectString.find(ygoData["data"][cardValue]["archetype"].asString()); foundName != std::string::npos;
                foundName = effectString.find(ygoData["data"][cardValue]["archetype"].asString())) {
                    effectString.erase(foundName, ygoData["data"][cardValue]["archetype"].asString().size());
                    effectString.insert(foundName, "archetype");
                }
                }
                for(std::string::iterator itStr = effectString.begin(); itStr != effectString.end(); itStr++) {
                    stringRef.append(itStr, itStr+1);
                    spacing++;
                    if(spacing > lineAmount && *itStr == ' ') { //Letter Limit new line
                        stringRef.append("\n");
                        spacing = 0;
                    }
                    if(*itStr == '\n')
                        spacing = 0;
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
    else {
        std::cout << "Random Number: " << cardValue << std::endl;
        if(ygoData["data"].isValidIndex(cardValue)) {
            std::cout << cardValue << ": " << ygoData["data"][cardValue]["name"].asString() << std::endl;
        }
        valid = false;
    }
}

#endif // SETUP_HPP_INCLUDED
