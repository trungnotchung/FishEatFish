#include "score.h"

LScore::LScore()
{
    gFont = NULL;
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LScore::~LScore()
{
    free();
}

bool LScore::loadFromRenderedText(SDL_Renderer *gRenderer, std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	
	//Return success
	return mTexture != NULL;
}

bool LScore::loadString(SDL_Renderer *gRender, std::string path, std::string str)
{
    bool success = true;

	//Open the font
	gFont = TTF_OpenFont( path.c_str(), 32);
	if( gFont == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if( !loadFromRenderedText(gRender, str, textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	return success;
}

void LScore::render(SDL_Renderer *gRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

void LScore::setWidth(int w)
{
	mWidth = w;
}

void LScore::setHeight(int h)
{
	mHeight = h;
}

int LScore::getWidth()
{
	return mWidth;
}

int LScore::getHeight()
{
	return mHeight;
}

void LScore::free()
{
    if(mTexture != NULL)
    {
        TTF_CloseFont( gFont );
		SDL_DestroyTexture( mTexture );
		gFont = NULL;
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
    }
}