#ifndef _BACK_GROUND
#define _BACK_GROUND

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "variable.h"
#include "lrandom.h"

class LBackGround
{
    public: 

        LBackGround();

        ~LBackGround();

        //Loads image at specified path
        bool loadFromFile( std::string path, SDL_Renderer *gRenderer);

        //Gender texture
        void render(SDL_Renderer *gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

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
    private:
        int mWidth, mHeight;
        //The actual hardware texture
        SDL_Texture* mTexture;

};

#endif