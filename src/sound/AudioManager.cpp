//
// Created by johan on 2016-01-16.
//


#include "AudioManager.h"
#include <sstream>
#include <Logger.h>


std::map<std::string, sf::Music*> AudioManager::musics;
std::map<std::string, sf::SoundBuffer> AudioManager::soundBuffers;


sf::Sound* AudioManager::loadAndFetchSound(const std::string &fileName){
    try {
        return getSoundBuffer(fileName);
    } catch (std::invalid_argument exception) {
        loadSoundBuffer(fileName);
        return getSoundBuffer(fileName);
    }
}

void AudioManager::loadSoundBuffer(const std::string &fileName) {
    sf::SoundBuffer soundBuffer;
    if(soundBuffer.loadFromFile(fileName) == -1) {
        Logger::logError("Unable to load soundbuffer " + fileName);
    }

    soundBuffers.insert(std::pair<std::string, sf::SoundBuffer>(fileName, soundBuffer));
}

sf::Sound* AudioManager::getSoundBuffer(std::string fileName){
    sf::SoundBuffer *soundBuffer = getItemFromMap(&soundBuffers, fileName);
    sf::Sound *sound = new sf::Sound();
    sound->setBuffer(*soundBuffer);
    return sound;
}


sf::Music* AudioManager::loadAndFetchMusic(const std::string &fileName) {
    try {
        return getMusic(fileName);
    } catch (std::invalid_argument exception) {
        loadMusic(fileName);
        return getMusic(fileName);
    }
}

void AudioManager::loadMusic(const std::string &fileName) {
    sf::Music *music = new sf::Music();
    if(music->openFromFile(fileName) == -1){
        Logger::logError("Unable to load music " + fileName);
    }

    musics.insert(std::pair<std::string, sf::Music*>(fileName, music));
}

sf::Music* AudioManager::getMusic(std::string fileName) {
    return *getItemFromMap(&musics, fileName);
}

template<typename Type>
Type* AudioManager::getItemFromMap(std::map<std::string, Type> *map, std::string id) {
    typename std::map<std::string, Type>::iterator it = map->find(id);
    if( it != map->end()) {
        return &it->second;
    } else {
        std::stringstream errorMessage;
        errorMessage << id << " hasn't been loaded into AudioManager before fetched";
        throw std::invalid_argument(errorMessage.str());
    }
}






