#include "ltexture.h"

bool LTexture::loadFromFile( std::string path, SDL_Renderer *gRenderer)
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

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(SDL_Renderer *gRenderer, std::string textureText, SDL_Color textColor )
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

bool LTexture::loadString(SDL_Renderer *gRender, std::string path, std::string str)
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
#endif

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render(SDL_Renderer *gRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	if(isBackGround)
		renderQuad.w = SCREEN_WIDTH, renderQuad.h = SCREEN_HEIGHT;

	if(isFish)
		renderQuad.w = curSize, renderQuad.h = curSize;
	//Render to screen
	if(left)
		SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

void LTexture::setImagePart(int _numPart)
{
	numPart = _numPart;
	for(int i=0; i<numPart; ++i)
	{
		gSpriteClips[i].x = i * IMAGE_SIZE;
		gSpriteClips[i].y = 0;
		gSpriteClips[i].w = IMAGE_SIZE;
		gSpriteClips[i].h = IMAGE_SIZE;
	}
}

void LTexture::setWidth(int w)
{
	mWidth = w;
}

void LTexture::setHeight(int h)
{
	mHeight = h;
}

void LTexture::setBackGround()
{
	isBackGround = true;
}

void LTexture::setFish()
{
	isFish = true;
}

void LTexture::setPoint(int _point)
{
	curPoint = _point;
}

void LTexture::setSize(int _size)
{
	curSize = _size;
}

int LTexture::getCurSize()
{
	return curSize;
}

int LTexture::getCurPoint()
{
	return curPoint;
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

void LTexture::reset()
{
	if(isFish)
	{
		isOnScreen = false;
		if(right) 
			curPosition.x = 0;
		else 
			curPosition.x = SCREEN_WIDTH;
		curPosition.y = randNum(0, SCREEN_HEIGHT);
	}	

}

LTexture::LTexture()
{
	//Initialize
	gFont = NULL;
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	isBackGround = false;
	numPart = 0;
	curPart = 0;
	curSpeed = 0;
	isFish = false;
	fishType = 0;
	isOnScreen = false;
	left = false;
	right = false;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		TTF_CloseFont( gFont );
		SDL_DestroyTexture( mTexture );
		gFont = NULL;
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		isBackGround = false;
		isFish = false;
		numPart = curPart = 0;
		curSpeed = 0;
		fishType = 0;
		isOnScreen = false;
		left = false;
		right = false;
	}
}