#include "fish.h"

LFish::LFish()
{
	numPart = 0;
	curPart = 0;
	curSpeed = 0;
	fishType = 0;
	isOnScreen = false;
	left = false;
	right = false;
	haveAI = false;
    mWidth = 0;
    mHeight = 0;
    trueFish = true;
    mTexture = NULL;
}

LFish::~LFish()
{
    free();
}

bool LFish::loadFromFile( std::string path, SDL_Renderer *gRenderer)
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
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LFish::render(SDL_Renderer *gRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	if(trueFish)
        renderQuad.w = curSize, renderQuad.h = curSize;
	//Render to screen
	if(left)
		SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

void LFish::setImagePart(int _numPart)
{
	numPart = _numPart;
	for(int i=0; i<numPart; ++i)
	{
		gSpriteClips[i].x = i * IMAGE_SIZE;
		gSpriteClips[i].y = 0;
		gSpriteClips[i].w = IMAGE_SIZE - 2;
		gSpriteClips[i].h = IMAGE_SIZE - 2;
	}
}

void LFish::setPoint(int _point)
{
	curPoint = _point;
}

void LFish::setSize(int _size)
{
	curSize = _size;
}

int LFish::getCurSize()
{
	return curSize;
}

int LFish::getCurPoint()
{
	return curPoint;
}

void LFish::reset()
{
    isOnScreen = false;
    if(right) 
        curPosition.x = 0;
    else 
        curPosition.x = SCREEN_WIDTH;
    curPosition.y = randNum(0, SCREEN_HEIGHT);
    curPart = 0;
    haveAI = randNum(0, 1);
}

void LFish::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LFish::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LFish::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LFish::setWidth(int w)
{
	mWidth = w;
}

void LFish::setHeight(int h)
{
	mHeight = h;
}

int LFish::getWidth()
{
	return mWidth;
}

int LFish::getHeight()
{
	return mHeight;
}

void LFish::setNotFish()
{
    trueFish = false;
}

void LFish::free()
{
    numPart = curPart = 0;
    curSpeed = 0;
    fishType = 0;
    isOnScreen = false;
    left = false;
    right = false;
    haveAI = false;
    mWidth = 0;
    mHeight = 0;
    trueFish = true;
    if(mTexture != NULL)
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
    }
}