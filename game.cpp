#include "game.h"
#include "lrandom.h"
#include "ltexture.h"

void LGame::loadGame()
{
    //Init program
    init();

    //Load Image
    loadMedia(gAnimal[SHARK], "pictures/simple/shark.png");

    loadMedia(gAnimal[JELLYFISH], "pictures/simple/jellyfish.png");

    loadMedia(gAnimal[TURTLE], "pictures/simple/turtle.png");

    loadMedia(gAnimal[LANTERNSHARK], "pictures/simple/lanternshark.png");

    loadMedia(gAnimal[EEL], "pictures/simple/eel.png");

    loadMedia(gAnimal[OCTOPUS], "pictures/simple/octopus.png");

    loadMedia(curBackGround, "pictures/simple/4.png");

    curBackGround.setBackGround();

	//Set image part to render
	setImagePart();

    //Set lantern shark to be your fish
    yourFish = gAnimal[LANTERNSHARK];

}


void LGame::playGame()
{
    bool quit = false;

    //Event handler
    SDL_Event e;

    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            
        }
        //Clear screen
        
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        curBackGround.render(gRenderer, 0, 0);

		yourFish.render(gRenderer, (SCREEN_WIDTH - yourFish.getWidth()) / 2, (SCREEN_HEIGHT - yourFish.getHeight()) / 2, &yourFish.gSpriteClips[yourFish.curPart / CNT_FRAME]);
		yourFish.curPart++;
		if(yourFish.curPart == yourFish.numPart * CNT_FRAME)
			yourFish.curPart = 0;

        //Update screen
        SDL_RenderPresent( gRenderer);

        // SDL_RenderPresent( gRenderer );
    }
}

bool LGame::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

                if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

// load string texture
bool LGame::loadString(LTexture &curTexture, const std::string &path, const std::string &str)
{
    bool success = true;

    if(!curTexture.loadString(gRenderer, path, str))
    {
        printf("Failed to load string texture\n");
        success = false;
    }

    return success;
}

bool LGame::loadMedia(LTexture &curTexture, const std::string &path)
{
	//Loading success flag
	bool success = true;

	//Load sprites
	if( !curTexture.loadFromFile(path, gRenderer) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	return success;
}

void LGame::setImagePart()
{
	gAnimal[SHARK].setImagePart(4);

	gAnimal[JELLYFISH].setImagePart(4);

	gAnimal[TURTLE].setImagePart(6);

	gAnimal[LANTERNSHARK].setImagePart(4);

	gAnimal[EEL].setImagePart(6);

	gAnimal[OCTOPUS].setImagePart(6);
}

SDL_Point LGame::lastestMousePosition(SDL_Event* e)
{
    SDL_Point res;
    int x = -1, y = -1;
    res.x = -1, res.y = -1;
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		SDL_GetMouseState( &x, &y );

		res.x = x, res.y = y;
	}
    return res;
}


void LGame::reset()
{
    
}

LGame::LGame()
{
    gWindow = NULL;
    gRenderer = NULL;
}

LGame::~LGame()
{
	//Free loaded images
	// gButtonSpriteSheetTexture.free();
	//Destroy window	
    
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
