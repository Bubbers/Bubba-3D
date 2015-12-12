//
// Created by johan on 2015-12-12.
//

#ifndef BUBBA_3D_SOUNDMANAGER_H
#define BUBBA_3D_SOUNDMANAGER_H

#include "AL/al.h"
#include "AL/alc.h"


class SoundManager {
public:

    SoundManager();
    ~SoundManager();

private:

    void checkError();
    ALuint buffer;
    ALuint source;

};


#endif //BUBBA_3D_SOUNDMANAGER_H
