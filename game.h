#ifndef _GAME_H
#define _GAME_H

#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include "ltexture.h"
#include "lbutton.h"
#include "lrandom.h"
#include "variable.h"
#include "lsound.h"

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

//permutation of fish
const int perm[] = {EEL, JELLYFISH, TURTLE, SHARK, OCTOPUS, LANTERNSHARK};
//size
const int sz[] = {120, 72, 96, 48, 48, 144};
//point
const int pt[] = {2000, 20, 200, 1, 1, 20000};
//speed
const int sp[] = {3, 2, 1, 1, 2, 3};
//numPart
const int np[] = {4, 4, 6, 4, 6, 6};

const int limitPoint[] = {1, 20, 200, 2000, 20000, 2000000};

const int limitSize[] = {48, 72, 96, 120, 144, 168};

const int cntFish[] = {30, 20, 15, 10, 5};

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

        //Result if your fish attack a fish
        int attackResult(LTexture &attackFish, LTexture &targetFish);

        //Init fish
        void setUpFish();

        //set AI for fish
        void fishAI();

        //Check if start new game
        bool startNewGame(SDL_Event* e);

        void reset();

        void free();

        string toString(int score);

        void renderScore();

        SDL_Point lastestMousePosition(SDL_Event* e);

        LTexture gAnimal[TOTAL_ANIMAL];
        LTexture curBackGround;
        LTexture yourFish;
        LTexture fishOnScreen[TOTAL_ANIMAL * 50];
        LTexture textScore[TOTAL_ANIMAL];
        LTexture gameLose, newGame, waitScreen;
        int totalFish;
        bool isLose, isStart;
        LSound myMusic;
    private:
        SDL_Window *gWindow;
        SDL_Renderer *gRenderer;
};
#endif