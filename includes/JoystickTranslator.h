/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
#pragma once

#include <IJoystickTranslation.h>
#include <JoystickTranslation.h>
#include <rapidjson/document.h>

class ControlStatus;

class JoystickTranslator {

public:
    IJoystickTranslation* getTranslation(unsigned int joystickID);

    static JoystickTranslator* getInstance();

    void init(std::string filePath);
    void updateMapping();

    virtual ~JoystickTranslator();

private:
    JoystickTranslator();

    std::vector<JoystickTranslation> translations;


    void readDocument(rapidjson::Document *doc);
    void check(bool, std::string);
    int getJoystick(int devId, int vendId, unsigned int startAt);
    void readMappings(rapidjson::Value* mappings, unsigned int joystickID, bool defaultMapping);

    JoystickTranslation::valueRetriever decideOnButtonRetriever(rapidjson::Value* mapData);
    JoystickTranslation::valueRetriever decideOnAxisRetriever(rapidjson::Value* mapData, std::string axisName);

    rapidjson::Document *jsonConfig;

    static IJoystickTranslation::Button buttonFromString(std::string name);
    static IJoystickTranslation::Axis axisFromString(std::string name);
    static sf::Joystick::Axis SFMLAxisFromString(std::string name);
};
