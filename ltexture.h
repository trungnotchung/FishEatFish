#ifndef _LTEXTURE_H
#define _LTEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

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

        //Number of part
        int numPart, curPart;

        //Rectangle of each part
        SDL_Rect gSpriteClips[10];
    private:
        //The actual hardware texture
        SDL_Texture* mTexture;
        TTF_Font *gFont;
        bool isBackGround;
        //Image dimensions
        int mWidth;
        int mHeight;
};  
#endif