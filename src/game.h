#ifndef _GAME_H
#define _GAME_H

#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include "lrandom.h"
#include "variable.h"
#include "lsound.h"
#include "background.h"
#include "fish.h"
#include "score.h"
#include "bomb.h"

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
const int perm[] = {LANTERNSHARK, EEL, JELLYFISH, TURTLE, SHARK, OCTOPUS};
//size
const int sz[] = {120, 72, 96, 48, 48, 144};
//point
const int pt[] = {8000, 20, 400, 1, 1, 160000};
//speed
const int sp[] = {3, 2, 1, 1, 2, 3};
//numPart
const int np[] = {4, 4, 6, 4, 6, 6};

const int limitPoint[] = {1, 20, 400, 8000, 160000, 2000000};

const int limitSize[] = {48, 72, 96, 120, 144, 168};

const int cntFish[] = {30, 20, 15, 10, 5};

const pair<int, int> backgroundPosition[] = {{161, 150}, {439, 150}, {724, 150}, {161, 420}, {436, 420}, {723, 420}};

class LGame
{
    public:

        LGame();

        ~LGame();

        bool init();

        void loadGame();

        void playGame();

        //Load fish
        bool loadFishImage(LFish &curFish, const std::string &path);

        // Load background
        bool loadBackGroundImage(LBackGround &curBackGround, const std::string &path);

        void setUpBackGround();

        // find which map is chosed
        bool choseMapBackGround(SDL_Event* e);

        // Load bomb
        bool loadBombImage(LBomb &curBomb, const std::string &path);

        //Load string texture
        bool loadString(LScore &curTexture, const std::string &path, const std::string &str);

        //Add a new fish to your screen
        void addNewFish();

        //Load a new texture for a fish
        void loadFish(LFish &curFish, int fishType);

        //Render a fish
        void render(LFish &curFish);

        //Result if your fish attack a fish
        int attackResult(LFish &attackFish, LFish &targetFish);

        //Init fish
        void setUpFish();

        //set AI for fish
        void fishAI();

        //Check if start new game
        bool startNewGame(SDL_Event* e);

        string toString(int score);
        
        //Load score on screen
        void renderScore();

        //Load combo on screen
        void renderCombo();

        //Enough combo kill, eat all fish you can on screen
        void hungryMode();

        SDL_Point lastestMousePosition(SDL_Event* e);

        //Update other fish's position
        void updateFishPosition();

        //Eat other fish
        void eatOtherFish();

        //Init Bomb
        void setUpBomb();

        //Add a new bomb on screen
        void addNewBomb();

        //Render bomb on screen
        void renderBomb();

        //Check if your fish is killed by bomb
        void checkBombExplosion();

        //Check if bomb kill other fish
        void bombKillFish();

        //Render a new explosion on screen
        void addNewExplosion(int x, int y);

        //Check if you are watching best score
        bool checkWatchingHighScore(SDL_Event *e);

        //Render high score on screen
        void renderHighScore();

        void reset();

        void free();

        LFish gAnimal[TOTAL_ANIMAL];
        LFish yourFish;
        LFish fishOnScreen[TOTAL_ANIMAL * 50];
        LBackGround gameLose, newGame, waitScreen, highScore;
        LBackGround curBackGround;
        LBackGround choseBackGround;
        LBackGround gBackGround[6], gSlotGame[6];
        LScore textScore[TOTAL_ANIMAL];
        LScore textCombo, textChoseBackGround;
        LScore highScoreTexture[10];
        LSound myMusic;
        Mix_Chunk *explosionSound = NULL, *mouseClick = NULL, *eatingSound = NULL;
        LBomb bombOnScreen[10], explosion[10];
        int totalFish;
        bool isLose, isStart, haveBackGround, watchingHighScore;
        int curTime, lastTime, curCombo;
        vector<int> highScoreVector;
    private:
        SDL_Window *gWindow;
        SDL_Renderer *gRenderer;
};
#endif