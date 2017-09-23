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

#include <memory>
#include "AudioManager.h"
#include <sstream>
#include <Logger.h>


std::map<std::string, std::shared_ptr<sf::Music>> AudioManager::musics;
std::map<std::string, std::shared_ptr<sf::SoundBuffer>> AudioManager::soundBuffers;


std::shared_ptr<sf::Sound> AudioManager::loadAndFetchSound(const std::string &fileName){
    try {
        return getSoundBuffer(fileName);
    } catch (std::invalid_argument exception) {
        loadSoundBuffer(fileName);
        return getSoundBuffer(fileName);
    }
}

void AudioManager::loadSoundBuffer(const std::string &fileName) {
    std::shared_ptr<sf::SoundBuffer> soundBuffer = std::make_shared<sf::SoundBuffer>();
    soundBuffer->loadFromFile(fileName);


    soundBuffers.insert(std::pair<std::string, std::shared_ptr<sf::SoundBuffer>>(fileName, soundBuffer));
}

std::shared_ptr<sf::Sound> AudioManager::getSoundBuffer(std::string fileName){
    std::shared_ptr<sf::SoundBuffer> soundBuffer = getItemFromMap(&soundBuffers, fileName);
    std::shared_ptr<sf::Sound> sound = std::make_shared<sf::Sound>();
    sound->setBuffer(*soundBuffer);
    return sound;
}


std::shared_ptr<sf::Music> AudioManager::loadAndFetchMusic(const std::string &fileName) {
    try {
        return getMusic(fileName);
    } catch (std::invalid_argument exception) {
        loadMusic(fileName);
        return getMusic(fileName);
    }
}

void AudioManager::loadMusic(const std::string &fileName) {
    std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
    music->openFromFile(fileName);


    musics.insert(std::pair<std::string, std::shared_ptr<sf::Music>>(fileName, music));
}

std::shared_ptr<sf::Music> AudioManager::getMusic(std::string fileName) {
    return getItemFromMap(&musics, fileName);
}

template<typename Type>
Type AudioManager::getItemFromMap(std::map<std::string, Type> *map, std::string id) {
    typename std::map<std::string, Type>::iterator it = map->find(id);
    if( it != map->end()) {
        return it->second;
    } else {
        std::stringstream errorMessage;
        errorMessage << id << " hasn't been loaded into AudioManager before fetched";
        throw std::invalid_argument(errorMessage.str());
    }
}






