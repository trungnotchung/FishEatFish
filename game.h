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

        //Set count part for each image
        void setImagePart();

        //Load string texture
        bool loadString(LTexture &curTexture, const std::string &path, const std::string &str);

        //Init size and fish type for each fish
        void initFish();

        //Add a new fish to your screen
        void addNewFish();

        //Render a fish
        void render(LTexture &curTexture);

        void close();

        void reset();

        void free();

        SDL_Point lastestMousePosition(SDL_Event* e);

        LTexture gAnimal[TOTAL_ANIMAL];
        LTexture curBackGround;
        LTexture yourFish;
        std::vector<LTexture> fishOnScreen;
    private:
        SDL_Window *gWindow;
        SDL_Renderer *gRenderer;
};
#endif