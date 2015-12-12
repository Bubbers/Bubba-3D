//
// Created by johan on 2015-12-12.
//

#ifndef BUBBA_3D_SOUNDMANAGER_H
#define BUBBA_3D_SOUNDMANAGER_H

#include <stdlib.h>
#include <AL/al.h>
#include <AL/alc.h>

#include <AL/alut.h>

#include <vector>

using namespace std;


class SoundManager {
public:

    SoundManager();
    ~SoundManager();

    ALboolean LoadALData();
    void LoadOGG(char *fileName, vector<char> &buffer, ALenum &format, ALsizei &freq);

private:

    void checkError();
    ALuint buffer;
    ALuint source;
    ALuint state;
    ALenum format;              // The sound data format
    ALsizei freq;               // The frequency of the sound data
    vector < char > bufferData; // The sound buffer data from file

};


#endif //BUBBA_3D_SOUNDMANAGER_H
