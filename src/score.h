#ifndef _SCORE
#define _SCORE_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

class LScore
{
    public:
        LScore();

        ~LScore();

        bool loadFromRenderedText(SDL_Renderer *gRenderer, std::string textureText, SDL_Color textColor );

        bool loadString(SDL_Renderer *gRender, std::string path, std::string str);

        void render(SDL_Renderer *gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        void setWidth(int w);

        void setHeight(int h);

        int getWidth();

        int getHeight();

        void free();

    private:
        TTF_Font *gFont;
        //The actual hardware texture
        SDL_Texture* mTexture;
        //Image dimensions
        int mWidth;
        int mHeight;
};

#endif