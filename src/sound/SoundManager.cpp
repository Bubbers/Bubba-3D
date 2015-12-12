//
// Created by johan on 2015-12-12.
//

#include <stddef.h>
#include <Logger.h>
#include "SoundManager.h"


SoundManager::SoundManager() {
    Logger::logInfo("Generating OpenAL data.");


    Logger::logInfo("Generating OpenAL data completed.");
}

SoundManager::~SoundManager() {

}

void SoundManager::checkError() {
    ALCenum error;

    error = alGetError();
    if (error != AL_NO_ERROR) {
        Logger::logInfo("Error in OPENAL, useful message huh?");
    }
}
