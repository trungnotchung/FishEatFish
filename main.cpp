#include "game.h"
#include <iostream>

int main(int argc, char **argv)
{
    srand(time(NULL));

    LGame newGame;
    newGame.loadGame();
    newGame.playGame();

    std::cout << "No error found!";
    return 0;
}