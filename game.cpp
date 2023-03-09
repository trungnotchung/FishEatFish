#include "game.h"

void LGame::loadGame()
{
    //Init program
    init();

    //load Fish
	for(int i=0; i<TOTAL_ANIMAL; ++i)
		loadFish(gAnimal[i], i);

	//load Background
    loadMedia(curBackGround, "pictures/simple/1.png");

    curBackGround.setBackGround();

    //Set lantern shark to be your fish
    yourFish = gAnimal[LANTERNSHARK];

	//Set your fish's position
	yourFish.curPosition.x = 0, yourFish.curPosition.y = 0;

	setUpFish();
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

		for(int i = 0; i < totalFish; ++i)
		{
			if(!fishOnScreen[i].isOnScreen)
				continue;
			if(fishOnScreen[i].right)
				fishOnScreen[i].curPosition.x += fishOnScreen[i].curSpeed;
			else 
				fishOnScreen[i].curPosition.x -= fishOnScreen[i].curSpeed;
			// std::cout << fishOnScreen[i].curPosition.x << " " << fishOnScreen[i].curPosition.y << '\n';
			if(fishOnScreen[i].curPosition.x < 0 || fishOnScreen[i].curPosition.x > SCREEN_WIDTH)
			{
				fishOnScreen[i].reset();
				continue;
			}
		}

		//Add a new fish
		++timer;
		if(timer == RANDOM_TIME)
		{
			addNewFish();
			timer = 0;
		}
		for(int i=0; i<totalFish; ++i) 
		{
			if(!fishOnScreen[i].isOnScreen)
				continue;
			render(fishOnScreen[i]);
			// std::cout << fishOnScreen[i].curPosition.x << " " << fishOnScreen[i].curPosition.y << '\n';
		}
		// for(int i=0; i<totalFish; ++i)
		// 	render(fishOnScreen[i]);
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
	int dir = randNum(0, 1);

	int fishType = candidates[pos];

	for(int j=0; j<totalFish; ++j)
	{
		if(fishOnScreen[j].fishType == fishType && fishOnScreen[j].isOnScreen == false && fishOnScreen[j].right == dir)
		{
			fishOnScreen[j].reset();
			fishOnScreen[j].isOnScreen = true;
			return;
		}
	}
}

void LGame::loadFish(LTexture &curTexture, int fishType)
{
	switch (fishType)
	{
		case SHARK: {
			loadMedia(curTexture, "pictures/simple/shark.png");
			curTexture.setImagePart(4);
			break;
		}
		case JELLYFISH: {
			loadMedia(curTexture, "pictures/simple/jellyfish.png");
			curTexture.setImagePart(4);
			break;
		}
		case TURTLE: {
			loadMedia(curTexture, "pictures/simple/turtle.png");
			curTexture.setImagePart(6);
			break;
		}
		case LANTERNSHARK: {
			loadMedia(curTexture, "pictures/simple/lanternshark.png");
			curTexture.setImagePart(4);
			break;
		}
		case EEL: {
			loadMedia(curTexture, "pictures/simple/eel.png");
			curTexture.setImagePart(6);
			break;
		}
		case OCTOPUS: {
			loadMedia(curTexture, "pictures/simple/octopus.png");
			curTexture.setImagePart(6);
			break;
		}
		default:
			loadMedia(curTexture, "pictures/simple/shark.png");
			curTexture.setImagePart(4);
			break;
	}

	curTexture.setFish();
	for(int i = 0; i < TOTAL_ANIMAL; ++i)
	{
		if(i == fishType)
		{
			curTexture.setSize(sz[i]);
			curTexture.setPoint(pt[i]);
			curTexture.curSpeed = sp[i];
		}
	}
	curTexture.fishType = fishType;
}

void LGame::setUpFish()
{
	for(int i = 0; i < TOTAL_ANIMAL; ++i)
	{
		for(int j=1; j<=10; ++j)
		{
			loadFish(fishOnScreen[totalFish], i);
			fishOnScreen[totalFish].curPosition.x = 0;
			fishOnScreen[totalFish].curPosition.y = randNum(0, SCREEN_HEIGHT);
			fishOnScreen[totalFish].right = true;
			++totalFish;
		}
		for(int j=1; j<=10; ++j)
		{
			loadFish(fishOnScreen[totalFish], i);
			fishOnScreen[totalFish].curPosition.x = SCREEN_WIDTH;
			fishOnScreen[totalFish].curPosition.y = randNum(0, SCREEN_HEIGHT);
			fishOnScreen[totalFish].left = true;
			++totalFish;
		}
	}
}

void LGame::reset()
{
    yourFish = gAnimal[LANTERNSHARK];
	for(int i=0; i<totalFish; ++i)
		fishOnScreen[i].reset();
}

LGame::LGame()
{
    gWindow = NULL;
    gRenderer = NULL;
	totalFish = 0;
}

LGame::~LGame()
{
	free();
}

void LGame::free()
{
	//Free loaded images
	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	for(int i=0; i<TOTAL_ANIMAL; ++i)
		gAnimal[i].free();
	curBackGround.free();
	yourFish.free();

	for(int i=0; i<totalFish; ++i)
		fishOnScreen[i].free();
	totalFish = 0;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}