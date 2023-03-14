#ifndef _SOUND_H
#define _SOUND_H
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>

class LSound
{
    public:
    Mix_Music *sound = NULL;

    LSound();

    ~LSound();

    void free();

    //Load music from file
    bool loadMusic(const std::string &path);

    //Playing music
    void playMusic();

    //Pause music
    void pauseMusic();

    //Resume music
    void resumeMusic();

    //Stop playing music
    void stopMusic();
};

#endif