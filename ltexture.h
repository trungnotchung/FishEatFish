#ifndef _LTEXTURE_H
#define _LTEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "variable.h"
#include "lrandom.h"

//Texture wrapper class
class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile( std::string path, SDL_Renderer *gRenderer);
        
        #if defined(SDL_TTF_MAJOR_VERSION)
        //Creates image from font string
        bool loadFromRenderedText(SDL_Renderer *gRender, std::string textureText, SDL_Color textColor );

        bool loadString(SDL_Renderer *gRender, std::string path, std::string str);
        #endif
        
        //Deallocates texture
        void free();

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        //Set blending
        void setBlendMode( SDL_BlendMode blending );

        //Set alpha modulation
        void setAlpha( Uint8 alpha );
        
        //Renders texture at given point
        void render(SDL_Renderer *gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        //setCoordinate for each part
        void setImagePart(int _numPart);

        //Gets image dimensions
        int getWidth();
        int getHeight();

        //Set image dimensions
        void setWidth(int w);
        void setHeight(int h);

        //Set this texture is a background
        void setBackGround();

        //Set this texture is a fish
        void setFish();

        //Set this texture is not a fish
        void unSetFish();

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
    private:
        //The actual hardware texture
        SDL_Texture* mTexture;
        TTF_Font *gFont;
        //Is this texture a background
        bool isBackGround;
        //Is this texture a fish
        bool isFish;
        //Size of this fish
        int curSize;
        //Point of this fish
        int curPoint;
        //Image dimensions
        int mWidth;
        int mHeight;
};

#endif