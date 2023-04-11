#include "background.h"

LBackGround::LBackGround()
{
    mWidth = SCREEN_WIDTH;
    mHeight = SCREEN_WIDTH;
    mTexture = NULL;
}

LBackGround::~LBackGround()
{
    free();
}

void LBackGround::render(SDL_Renderer *gRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    SDL_Rect renderQuad = { x, y, mWidth, mHeight};
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

bool LBackGround::loadFromFile( std::string path, SDL_Renderer *gRenderer)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = SCREEN_WIDTH;
			mHeight = SCREEN_HEIGHT;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LBackGround::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LBackGround::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LBackGround::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LBackGround::setWidth(int w)
{
	mWidth = w;
}

void LBackGround::setHeight(int h)
{
	mHeight = h;
}

int LBackGround::getWidth()
{
	return mWidth;
}

int LBackGround::getHeight()
{
	return mHeight;
}

void LBackGround::free()
{
    mWidth = 0;
    mHeight = 0;
    if(mTexture != NULL)
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
    }
}