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

    loadMedia(curBackGround, "pictures/simple/1.png");

    curBackGround.setBackGround();

	//Set image part to render
	setImagePart();

	//Init size and fish type for each fish
	initFish();

    //Set lantern shark to be your fish
    yourFish = gAnimal[LANTERNSHARK];

	//Set your fish's position
	yourFish.curPosition.x = 0, yourFish.curPosition.y = 0;
}	


void LGame::playGame()
{
    bool quit = false;

    //Event handler
    SDL_Event e;

	int timer = 0;

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
            SDL_Point p = lastestMousePosition(&e);

			//Update your fish's position
			if(p.x != -1 || p.y != -1)
			{	
				yourFish.curPosition.x = p.x - IMAGE_SIZE/2;
				yourFish.curPosition.y = p.y - IMAGE_SIZE/2;
			}
        }
        //Clear screen
        
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        curBackGround.render(gRenderer, 0, 0);

		render(yourFish);

		++timer;
		if(timer == RANDOM_TIME)
			addNewFish(), timer = 0;

		for(int i=(int)fishOnScreen.size()-1; i >= 0; --i)
		{
			fishOnScreen[i].curPosition.x += fishOnScreen[i].curSpeed;
			std::cout << fishOnScreen[i].curPosition.x << " " << fishOnScreen[i].curPosition.y << '\n';
			if(fishOnScreen[i].curPosition.x < 0 || fishOnScreen[i].curPosition.x > SCREEN_WIDTH)
			{
				std::swap(fishOnScreen[i], fishOnScreen.back());
				fishOnScreen.pop_back();
				continue;
			}
			render(fishOnScreen[i]);
		}

        //Update screen
        SDL_RenderPresent( gRenderer);

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

void LGame::render(LTexture &curTexture)
{
	curTexture.render(gRenderer, curTexture.curPosition.x, curTexture.curPosition.y, &curTexture.gSpriteClips[curTexture.curPart / CNT_FRAME]);
	curTexture.curPart++;
	if(curTexture.curPart == curTexture.numPart * CNT_FRAME)
		curTexture.curPart = 0;
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

void LGame::initFish()
{
	gAnimal[EEL].setFish();
	gAnimal[EEL].setSize(48);
	gAnimal[EEL].setPoint(1);
	gAnimal[EEL].curSpeed = 2;

	gAnimal[JELLYFISH].setFish();
	gAnimal[JELLYFISH].setSize(72);
	gAnimal[JELLYFISH].setPoint(20);
	gAnimal[JELLYFISH].curSpeed = 2;

	gAnimal[TURTLE].setFish();
	gAnimal[TURTLE].setSize(96);
	gAnimal[TURTLE].setPoint(200);
	gAnimal[TURTLE].curSpeed = 1;

	gAnimal[SHARK].setFish();
	gAnimal[SHARK].setSize(120);
	gAnimal[SHARK].setPoint(2000);
	gAnimal[SHARK].curSpeed = 3;

	gAnimal[OCTOPUS].setFish();
	gAnimal[OCTOPUS].setSize(144);
	gAnimal[OCTOPUS].setPoint(20000);
	gAnimal[OCTOPUS].curSpeed = 3;


	gAnimal[LANTERNSHARK].setFish();
	gAnimal[LANTERNSHARK].setSize(48);
	gAnimal[LANTERNSHARK].setPoint(1);
	gAnimal[LANTERNSHARK].curSpeed = 1;

	for(int i=0; i<TOTAL_ANIMAL; ++i)
		gAnimal[i].curSpeed = 1;
}

void LGame::addNewFish()
{
	std::vector<std::pair<int, int> > diff;
	for(int i = 0; i < TOTAL_ANIMAL; ++i)
	{
		if(i == LANTERNSHARK) continue;
		diff.push_back(std::make_pair(yourFish.getCurPoint() - gAnimal[i].getCurPoint(), i));
	}
	sort(diff.begin(), diff.end());

	std::vector<int> candidates;
	for(int i = 0; i < diff.size(); ++i)
	{
		for(int j = 0; j < TOTAL_ANIMAL - 1 - i; ++j)
			candidates.push_back(diff[i].second);
	}

	random_shuffle(candidates.begin(), candidates.end());
	int pos = randNum(0, (int)candidates.size() - 1);

	int fish = candidates[pos];

	int y = randNum(0, SCREEN_HEIGHT);
	fishOnScreen.push_back(gAnimal[fish]);
	fishOnScreen.back().curPosition.x = 0;
	fishOnScreen.back().curPosition.y = y;
}

void LGame::reset()
{
    yourFish = gAnimal[LANTERNSHARK];
	fishOnScreen.clear();

}

LGame::LGame()
{
    gWindow = NULL;
    gRenderer = NULL;
}

LGame::~LGame()
{
	free();
}

void LGame::free()
{
	//Free loaded images
	// gButtonSpriteSheetTexture.free();
	//Destroy window	
	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	for(int i=0; i<TOTAL_ANIMAL; ++i)
		gAnimal[i].free();
	curBackGround.free();
	yourFish.free();
	for(int i=0; i<(int)fishOnScreen.size(); ++i)
		fishOnScreen[i].free();
	fishOnScreen.clear();

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}