//
// Created by simon on 2016-01-14.
//

#include <JoystickTranslator.h>
#include <stdexcept>
#include <filereadstream.h>
#include <document.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <SFML/Window.hpp>
#include <IJoystickTranslation.h>
#include <JoystickTranslation.h>
#include <Logger.h>

using namespace rapidjson;
using namespace std;

JoystickTranslator::JoystickTranslator() {

}

JoystickTranslator* JoystickTranslator::getInstance() {
    static JoystickTranslator jt;
    return &jt;
}

void JoystickTranslator::init(string filePath) {

    struct stat filestatus;
    stat(filePath.c_str(), &filestatus);
    const unsigned long long size = filestatus.st_size;

    std::FILE* file = fopen(filePath.c_str(), "r");
    char readBuffer[size];
    FileReadStream rs(file,readBuffer,size);

    jsonConfig = new Document();
    jsonConfig->ParseStream<kParseCommentsFlag>(rs);
    try{
        readDocument(jsonConfig);
    }catch(std::string error){
        Logger::logError("Failed reading controls: " + error);
    }
}

void JoystickTranslator::updateMapping(){
    try{
        readDocument(jsonConfig);
    }catch(std::string error){
        Logger::logError("Failed reading controls: " + error);
    }
}

JoystickTranslator::~JoystickTranslator() {

}

void JoystickTranslator::readDocument(Document* doc) {
    check(doc->IsArray(),"Document was not array.");
    bool joysticksFound[8] = {false,false,false,false,false,false,false,false};
    Value* def;
    bool defFound = false;
    for(SizeType i = 0; i < doc->Size() ; i++){
        int devId  = (*doc)[i]["deviceID"].GetInt();
        int vendId  = (*doc)[i]["vendorID"].GetInt();
        int js = -1;
        while((js = getJoystick(devId,vendId,js+1)) != -1) {
            readMappings(&((*doc)[i]["mappings"]), js, false);
            joysticksFound[js] = true;
        }
        if((*doc)[i].HasMember("default") && (*doc)[i]["default"].GetBool()){
            defFound = true;
            def = &(*doc)[i]["mappings"];
        }
    }
    if(!defFound)
        throw "There was no default mapping.";
    for(unsigned int found = 0; found < 8 ; found++){
        if(sf::Joystick::isConnected(found) && !joysticksFound[found])
            readMappings(def,found, true);
    }
}

void JoystickTranslator::readMappings(Value* mappings, unsigned int joystick, bool defaultMapping) {
    check(mappings->IsArray(),"The mappings wasn't an array.");
    translations.push_back(JoystickTranslation(joystick, defaultMapping));
    for(SizeType i = 0; i<mappings->Size() ; i++){
        string type = (*mappings)[i]["type"].GetString();
        string button = (*mappings)[i]["xbox"].GetString();
        if(type == "button"){
            translations.back().addButton(buttonFromString(button),
                         decideOnButtonRetriever(&(*mappings)[i]["generic"]));
        }else if(type == "axis"){
            translations.back().addAxis(axisFromString(button),
                         decideOnAxisRetriever(&(*mappings)[i]["generic"]));
        }else{
            throw "Invalid type in button " + button;
        }
    }
}

JoystickTranslation::valueRetriever JoystickTranslator::decideOnButtonRetriever(Value* mapData) {
    check(mapData->IsObject(), "value wasn't object when checking button generic.");
    string type = (*mapData)["type"].GetString();
    if(type == "button")
        return JoystickTranslation::buttonValueRetriever((*mapData)["buttonID"].GetUint());
    else if(type == "axis"){
        return JoystickTranslation::buttonFromAxisRetriever(
                SFMLAxisFromString(string((*mapData)["name"].GetString())));
    } else if(type == "halfAxis") {
        return JoystickTranslation::buttonFromHalfAxisRetriever(
                SFMLAxisFromString(string((*mapData)["name"].GetString())),
                (*mapData)["positiveElseNegative"].GetBool());
    }else if(type == "none"){
        return JoystickTranslation::valRetriever(0.0f);
    }else
        throw "Unknown type: " + type + " when deciding button retriever.";
}

JoystickTranslation::valueRetriever JoystickTranslator::decideOnAxisRetriever(Value *mapData) {
    check(mapData->IsObject(), "value wasn't object when checking button generic.");
    string type = (*mapData)["type"].GetString();
    if(type == "button")
        return JoystickTranslation::axisFromButtonRetriever((*mapData)["buttonID"].GetUint());
    else if(type == "axis")
        return JoystickTranslation::axisValueRetriever(
                SFMLAxisFromString(string((*mapData)["name"].GetString())));
    else if(type == "buttons")
        return JoystickTranslation::axisFromButtonsRetriever((*mapData)["buttonNeg"].GetUint(),
                                                             (*mapData)["buttonPos"].GetUint());
    else if(type == "noneNegative")
        return JoystickTranslation::valRetriever(-100.0f);
    else if (type == "none")
        return JoystickTranslation::valRetriever(0.0f);
    else
        throw "Unknown type: " + type + " when deciding axis retriever.";
}

int JoystickTranslator::getJoystick(int devId, int vendId, unsigned int startAt = 0) {
    for(unsigned int i = startAt; i<sf::Joystick::Count ; i++){
        sf::Joystick::Identification id = sf::Joystick::getIdentification(i);
        if(id.productId == devId && id.vendorId == vendId)
            return i;
    }
    return -1;
}

void JoystickTranslator::check(bool ch, string message) {
    if(!ch)
        throw message;
}

IJoystickTranslation* JoystickTranslator::getTranslation(unsigned int joystickID) {
    if(joystickID >= sf::Joystick::Count)
        throw std::invalid_argument("The parameter joystick was " + std::to_string(joystickID) +
                                    " but must be less than " + std::to_string(sf::Joystick::Count));
    for(auto it = translations.begin(); it != translations.end(); ++it){
        if((*it).getJoystickID() == joystickID)
            return &*it;
    }
    throw std::invalid_argument("The joystick " + std::to_string(joystickID) + " isn't connected.");
}

IJoystickTranslation::Button JoystickTranslator::buttonFromString(std::string name) {
    if(name == "X")
        return IJoystickTranslation::Button::X;
    if(name == "A")
        return IJoystickTranslation::Button::A;
    if(name == "B")
        return IJoystickTranslation::Button::B;
    if(name == "Y")
        return IJoystickTranslation::Button::Y;
    if(name == "START")
        return IJoystickTranslation::Button::START;
    if(name == "BACK")
        return IJoystickTranslation::Button::BACK;
    if(name == "GUIDE")
        return IJoystickTranslation::Button::GUIDE;
    if(name == "LB")
        return IJoystickTranslation::Button::LB;
    if(name == "RB")
        return IJoystickTranslation::Button::RB;
    if(name == "LEFT_THUMBSTICK_PUSH")
        return IJoystickTranslation::Button::LEFT_THUMBSTICK_PUSH;
    if(name == "RIGHT_THUMBSTICK_PUSH")
        return IJoystickTranslation::Button::RIGHT_THUMBSTICK_PUSH;
    throw std::invalid_argument(name + " is not a button.");
}

IJoystickTranslation::Axis JoystickTranslator::axisFromString(std::string name) {
    if(name == "DPAD_X")
        return IJoystickTranslation::Axis::DPAD_X;
    if(name == "DPAD_Y")
        return IJoystickTranslation::Axis::DPAD_Y;
    if(name == "LEFT_THUMBSTICK_X")
        return IJoystickTranslation::Axis::LEFT_THUMBSTICK_X;
    if(name == "LEFT_THUMBSTICK_Y")
        return IJoystickTranslation::Axis::LEFT_THUMBSTICK_Y;
    if(name == "RIGHT_THUMBSTICK_X")
        return IJoystickTranslation::Axis::RIGHT_THUMBSTICK_X;
    if(name == "RIGHT_THUMBSTICK_Y")
        return IJoystickTranslation::Axis::RIGHT_THUMBSTICK_Y;
    if(name == "LT")
        return IJoystickTranslation::Axis::LT;
    if(name == "RT")
        return IJoystickTranslation::Axis::RT;
    throw std::invalid_argument(name + " is not an axis.");
}

sf::Joystick::Axis JoystickTranslator::SFMLAxisFromString(std::string name) {
    if(name == "X")
        return sf::Joystick::Axis::X;
    if(name == "PovX")
        return sf::Joystick::Axis::PovX;
    if(name == "PovY")
        return sf::Joystick::Axis::PovY;
    if(name == "Y")
        return sf::Joystick::Axis::Y;
    if(name == "Z")
        return sf::Joystick::Axis::Z;
    if(name == "R")
        return sf::Joystick::Axis::R;
    if(name == "U")
        return sf::Joystick::Axis::U;
    if(name == "V")
        return sf::Joystick::Axis::V;
    throw std::invalid_argument(name + " is not an SFML axis.");
}