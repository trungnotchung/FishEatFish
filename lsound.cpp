#include "lsound.h"

LSound::LSound()
{
    sound = NULL;
}

LSound::~LSound()
{
    free();
}

void LSound::free()
{
    Mix_FreeMusic(sound);
    sound = NULL;
}

bool LSound::loadMusic(const std::string &path)
{
    sound = Mix_LoadMUS(path.c_str());
    bool success = true;
    if(sound == NULL)
    {
        printf( "Failed to load sound! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
    }
    return success;
}

void LSound::pauseMusic()
{
    if( Mix_PausedMusic() == 0)
        Mix_PauseMusic();
}

void LSound::resumeMusic()
{
    if( Mix_PausedMusic() == 1 )
        Mix_ResumeMusic();
}

void LSound::stopMusic()
{
    Mix_HaltMusic();
}

void LSound::playMusic()
{
    if(Mix_PlayingMusic() == 0)
    {
        Mix_PlayMusic(sound, -1);
    }
}