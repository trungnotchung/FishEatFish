#ifndef _GAME_H
#define _GAME_H
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <queue>
#include <iostream>
#include "ltexture.h"
#include "lbutton.h"
#include "variable.h"

enum Animal
{
    SHARK = 0,
    JELLYFISH = 1, 
    TURTLE = 2,
    LANTERNSHARK = 3,
    EEL = 4, 
    OCTOPUS = 5,
    TOTAL_ANIMAL = 6
};

class LGame
{
    public:

        LGame();

        ~LGame();

        bool init();

        void loadGame();

        void playGame();

        bool loadMedia(LTexture &curTexture, const std::string &path);

        void setImagePart();

        bool loadString(LTexture &curTexture, const std::string &path, const std::string &str);

        void close();

        void reset();

        SDL_Point lastestMousePosition(SDL_Event* e);

        LTexture gAnimal[TOTAL_ANIMAL];
        LTexture curBackGround;
        LTexture yourFish;
    private:
        SDL_Window *gWindow;
        SDL_Renderer *gRenderer;
};
#endif