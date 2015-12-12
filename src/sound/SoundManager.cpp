//
// Created by johan on 2015-12-12.
//

#include <stddef.h>
#include <Logger.h>
#include <iostream>
#include "SoundManager.h"
#include "vorbis/vorbisfile.h"

#define BUFFER_SIZE   32768


void SoundManager::LoadOGG(char *fileName, vector<char> &buffer, ALenum &format, ALsizei &freq)
{
    int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
    int bitStream;
    long bytes;
    char array[BUFFER_SIZE];                // Local fixed size array
    FILE *f;

    // Open for binary reading
    f = fopen(fileName, "rb");

    if (f == NULL)
    {
        cerr << "Cannot open " << fileName << " for reading..." << endl;
        exit(-1);
    }
    // end if

    vorbis_info *pInfo;
    OggVorbis_File oggFile;

    // Try opening the given file
    if (ov_open(f, &oggFile, NULL, 0) != 0)
    {
        cerr << "Error opening " << fileName << " for decoding..." << endl;
        exit(-1);
    }
    // end if

    // Get some information about the OGG file
    pInfo = ov_info(&oggFile, -1);

    // Check the number of channels... always use 16-bit samples
    if (pInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;
    // end if

    // The frequency of the sampling rate
    freq = pInfo->rate;

    // Keep reading until all is read
    do
    {
        // Read up to a buffer's worth of decoded sound data
        bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);

        if (bytes < 0)
        {
            ov_clear(&oggFile);
            cerr << "Error decoding " << fileName << "..." << endl;
            exit(-1);
        }
        // end if

        // Append to end of buffer
        buffer.insert(buffer.end(), array, array + bytes);
    }
    while (bytes > 0);

    // Clean up!
    ov_clear(&oggFile);
}
// end of LoadOGG


SoundManager::SoundManager() {
    Logger::logInfo("Generating OpenAL data.");

    ALint state;                            // The state of the sound source
    ALuint bufferID;                        // The OpenAL sound buffer ID
    ALuint sourceID;                        // The OpenAL sound source
    ALenum format;                          // The sound data format
    ALsizei freq;                           // The frequency of the sound data
    vector<char> bufferData;                // The sound buffer data from file

    ALfloat listenerPos[3]={0.0,0.0,4.0};
    ALfloat listenerVel[3]={0.0,0.0,0.0};
    ALfloat listenerOri[6]={0.0,0.0,1.0, 0.0,1.0,0.0};
    ALfloat source0Pos[3]={ 0.0, 0.0, 0.0};
    ALfloat source0Vel[3]={ 0.0, 0.0, 0.0};

    // Initialize the OpenAL library
    alutInit(0, NULL);

    alListenerfv(AL_POSITION,listenerPos);
    alListenerfv(AL_VELOCITY,listenerVel);
    alListenerfv(AL_ORIENTATION,listenerOri);

    // Create sound buffer and source
    alGenBuffers(1, &bufferID);
    alGenSources(1, &sourceID);



    // Load the OGG file into memory
    LoadOGG("../scenes/tommy-korberg-stad-i-ljus.ogg", bufferData, format, freq);

    // Upload sound data to buffer
    alBufferData(bufferID, format, &bufferData[0], static_cast<ALsizei>(bufferData.size()), freq);

    // Attach sound buffer to source
    alSourcei(sourceID, AL_BUFFER, bufferID);

    checkError();
    // Set the source and listener to the same location
    alSourcef(sourceID, AL_PITCH, 1.0f);
    alSourcef(sourceID, AL_GAIN, 1.0f);
    alSourcefv(sourceID, AL_POSITION, source0Pos);
    alSourcefv(sourceID, AL_VELOCITY, source0Vel);
    alSourcei(sourceID, AL_BUFFER,buffer);
    alSourcei(sourceID, AL_LOOPING, AL_TRUE);

    alSourcePlay(sourceID);

    Logger::logInfo("Generating OpenAL data completed.");
}

SoundManager::~SoundManager() {

}

void SoundManager::checkError() {
    ALCenum error;

    error = alGetError();
    if (error != AL_NO_ERROR) {
        Logger::logInfo("Error in OPENAL");
    }
}


