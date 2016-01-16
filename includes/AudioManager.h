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
    static Type* getItemFromMap(std::map<std::string, Type> *map, std::string id) ;

    static sf::Sound* loadAndFetchSound(const std::string &fileName);
    static sf::Music* loadAndFetchMusic(const std::string &fileName);

private:
    static std::map<std::string, sf::Music*> musics;
    static std::map<std::string, sf::SoundBuffer> soundBuffers;

    static void loadSoundBuffer(const std::string &fileName);
    static sf::Sound* getSoundBuffer(std::string fileName);

    static void loadMusic(const std::string &fileName);
    static sf::Music* getMusic(std::string fileName);
};

#endif //SUPER_BUBBA_AWESOME_SPACE_AUDIOMANAGER_H
