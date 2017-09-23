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
//
// Created by johan on 2016-01-16.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_AUDIOMANAGER_H
#define SUPER_BUBBA_AWESOME_SPACE_AUDIOMANAGER_H

#include <map>
#include <sstream>
#include <SFML/Audio.hpp>

class AudioManager {
public:
    template<typename Type>
    static Type getItemFromMap(std::map<std::string, Type> *map, std::string id) ;

    static std::shared_ptr<sf::Sound>loadAndFetchSound(const std::string &fileName);
    static std::shared_ptr<sf::Music> loadAndFetchMusic(const std::string &fileName);


private:
    static std::map<std::string, std::shared_ptr<sf::Music>> musics;
    static std::map<std::string, std::shared_ptr<sf::SoundBuffer>> soundBuffers;


    static void loadSoundBuffer(const std::string &fileName);
    static std::shared_ptr<sf::Sound> getSoundBuffer(std::string fileName);

    static void loadMusic(const std::string &fileName);
    static std::shared_ptr<sf::Music> getMusic(std::string fileName);
};

#endif //SUPER_BUBBA_AWESOME_SPACE_AUDIOMANAGER_H
