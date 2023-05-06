#ifndef FISH
#define FISH

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "variable.h"
#include "lrandom.h"

class LFish
{
    public: 
        LFish();

        ~LFish();

        //Loads image at specified path
        bool loadFromFile( std::string path, SDL_Renderer *gRenderer);

        //render this fish
        void render(SDL_Renderer *gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        //setCoordinate for each part
        void setImagePart(int _numPart);

        //Set point for this fish
        void setPoint(int _point);

        //Get this fish's point
        int getCurPoint();

        //Set size for this fish
        void setSize(int _size);

        //Reset this fish
        void reset();

        //Get this fish's size
        int getCurSize();

        //Number of part
        int numPart, curPart;

        //Type of this fish
        int fishType;
        
        //Is this fish on screen
        bool isOnScreen;

        //Speed of this fish
        int curSpeed;

        //Direction of this fish
        bool left, right;

        //Cur coordinate of top left position
        SDL_Point curPosition;

        //Rectangle of each part
        SDL_Rect gSpriteClips[10];

        //Does this fish have AI
        bool haveAI;

        //Size of this fish
        int curSize;
        
        //Point of this fish
        int curPoint;

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        //Set blending
        void setBlendMode( SDL_BlendMode blending );

        //Set alpha modulation
        void setAlpha( Uint8 alpha );

        void setWidth(int w);

        void setHeight(int h);

        int getWidth();

        int getHeight();

        void free();

        void setNotFish();
        bool follow;
    private:
        int mWidth, mHeight;
        //The actual hardware texture
        SDL_Texture* mTexture;
        bool trueFish;
};

#endif