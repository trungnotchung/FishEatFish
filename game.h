#ifndef _GAME_H
#define _GAME_H
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>
#include "ltexture.h"
#include "lbutton.h"
#include "lrandom.h"
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

//size
const int sz[] = {120, 72, 96, 48, 48, 144};
//point
const int pt[] = {2000, 20, 200, 1, 1, 20000};
//speed
const int sp[] = {3, 2, 1, 1, 2, 3};
//numPart
const int np[] = {4, 4, 6, 4, 6, 6};

class LGame
{
    public:

        LGame();

        ~LGame();

        bool init();

        void loadGame();

        void playGame();

        //Load image texture
        bool loadMedia(LTexture &curTexture, const std::string &path);

        //Load string texture
        bool loadString(LTexture &curTexture, const std::string &path, const std::string &str);

        //Add a new fish to your screen
        void addNewFish();

        //Load a new texture for a fish
        void loadFish(LTexture &curTexture, int fishType);

        //Render a fish
        void render(LTexture &curTexture);

        //Init fish
        void setUpFish();

        void close();

        void reset();

        void free();

        SDL_Point lastestMousePosition(SDL_Event* e);

        LTexture gAnimal[TOTAL_ANIMAL];
        LTexture curBackGround;
        LTexture yourFish;
        LTexture fishOnScreen[TOTAL_ANIMAL * 11];
        int totalFish;
    private:
        SDL_Window *gWindow;
        SDL_Renderer *gRenderer;
};
#endif