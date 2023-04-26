#include "game.h"

void LGame::loadGame()
{
	// Init program
	init();

	// load Fish
	for (int i = 0; i < TOTAL_ANIMAL; ++i)
		loadFish(gAnimal[i], i);

	// load Background
	setUpBackGround();

	// Set lantern shark to be your fish
	yourFish = gAnimal[LANTERNSHARK];

	// Set your fish's position
	yourFish.curPosition.x = 0, yourFish.curPosition.y = 0;

	setUpFish();

	setUpBomb();

	// set height and width for render score
	for (int i = 0; i < TOTAL_ANIMAL; ++i)
		gAnimal[i].setWidth(80), gAnimal[i].setHeight(60);
	for (int i = 0; i < TOTAL_ANIMAL; ++i)
		gAnimal[i].setNotFish();
	for (int i = 0; i < TOTAL_ANIMAL; ++i)
		gAnimal[perm[i]].curPosition.x = i * 100 + 295, gAnimal[perm[i]].curPosition.y = 0;

	loadString(textChoseBackGround, "../fonts/gomarice_no_continue.ttf", "Chose A Map For Hunting");
	myMusic.loadMusic("../sounds/sound1.mp3");
	explosionSound = Mix_LoadWAV("../sounds/explosion1.mp3");
	mouseClick = Mix_LoadWAV("../sounds/mouse_click.wav");
	eatingSound = Mix_LoadWAV("../sounds/chew-21768.mp3");
}

void LGame::playGame()
{
	bool quit = false;

	// Event handler
	SDL_Event e;

	int timer = 0;
	int timeBomb = 0;
	// While application is running
	while (!quit)
	{
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			if(e.type == SDL_MOUSEBUTTONDOWN)
			{
				Mix_PlayChannel(-1, mouseClick, 0);
			}

			// if (e.type == SDL_MOUSEBUTTONDOWN)
			// {
			// 	int x, y;
			// 	// Get mouse position
			// 	SDL_GetMouseState(&x, &y);
			// 	cout << x << " " << y << '\n';
			// }
			SDL_Point p = lastestMousePosition(&e);

			// Update your fish's position
			if (p.x != -1 || p.y != -1)
			{
				if (p.x - yourFish.getCurSize() / 2 < yourFish.curPosition.x)
					yourFish.left = true, yourFish.right = false;
				else if (p.x - yourFish.getCurSize() / 2 > yourFish.curPosition.x)
					yourFish.right = true, yourFish.left = false;
				yourFish.curPosition.x = p.x - yourFish.getCurSize() / 2;
				yourFish.curPosition.y = p.y - yourFish.getCurSize() / 2;
			}

			if(!watchingHighScore)
			{
				if(checkWatchingHighScore(&e))
					watchingHighScore = true;
			}
			if (!isStart)
			{
				if (startNewGame(&e))
					isStart = true, watchingHighScore = false;
			}
			else if(!haveBackGround)
			{
				if(choseMapBackGround(&e))
					haveBackGround = true;
			}
			else if (isLose)
			{
				if (startNewGame(&e))
				{
					isLose = false;
					reset();
				}
			}
		}
		// Clear screen

		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		if(watchingHighScore)
		{
			choseBackGround.render(gRenderer, 0, 0);
			renderHighScore();
			newGame.render(gRenderer, (SCREEN_WIDTH - newGame.getWidth()) / 2, 500);
			SDL_RenderPresent(gRenderer);
		}
		else if (!isStart)
		{
			waitScreen.render(gRenderer, 0, 0);
			newGame.render(gRenderer, (SCREEN_WIDTH - newGame.getWidth()) / 2, 500);
			highScore.render(gRenderer, (SCREEN_WIDTH - highScore.getWidth()) / 2, 600);
			SDL_RenderPresent(gRenderer);
		}
		else if(!haveBackGround)
		{
			choseBackGround.render(gRenderer, 0, 0);
			for(int i = 0; i < 6; ++i)
				gBackGround[i].render(gRenderer, backgroundPosition[i].first + 20, backgroundPosition[i].second + 20);
			for(int i = 0; i < 6; ++i)
				gSlotGame[i].render(gRenderer, backgroundPosition[i].first, backgroundPosition[i].second);
			textChoseBackGround.render(gRenderer, 400, 30);
			SDL_RenderPresent(gRenderer);
		}
		else if (!isLose)
		{
			++curTime;

			// No explosion
			for (int i = 0; i < 10; ++i)
				explosion[i].isOnScreen = false;

			myMusic.playMusic();

			fishAI();

			// Add a new fish
			++timer;
			if (timer == RANDOM_TIME)
			{
				addNewFish();
				timer = 0;
			}

			// Add a new bomb
			++timeBomb;
			if (timeBomb == TIME_BOMB)
			{
				addNewBomb();
				timeBomb = 0;
			}

			if(curTime - lastTime > COMBO_TIME)
				curCombo = 0;
			updateFishPosition();
			eatOtherFish();

			// Increase your fish's size
			for (int i = 0; i < TOTAL_ANIMAL; ++i)
			{
				if (yourFish.getCurPoint() >= limitPoint[i])
					yourFish.setSize(limitSize[i]);
			}
			curBackGround.render(gRenderer, 0, 0);
			render(yourFish);
			renderScore();
			renderCombo();
			renderBomb();
			checkBombExplosion();
			bombKillFish();

			for (int i = 0; i < totalFish; ++i)
			{
				if (!fishOnScreen[i].isOnScreen)
					continue;
				render(fishOnScreen[i]);
				// std::cout << fishOnScreen[i].curPosition.x << " " << fishOnScreen[i].curPosition.y << '\n';
			}
			SDL_RenderPresent(gRenderer);
			if (isLose)
			{
				highScoreVector.push_back(yourFish.getCurPoint());
				SDL_Delay(2000);
				myMusic.stopMusic();
			}
		}
		else
		{
			gameLose.render(gRenderer, 0, 0);
			newGame.render(gRenderer, (SCREEN_WIDTH - newGame.getWidth()) / 2, 500);
			highScore.render(gRenderer, (SCREEN_WIDTH - highScore.getWidth()) / 2, 600);
			SDL_RenderPresent(gRenderer);
		}
	}
}

bool LGame::init()
{
	// Initialization flag
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		// Create window
		gWindow = SDL_CreateWindow("Fish eat fish", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

string LGame::toString(int score)
{
	assert(0 <= score && score <= (int)1e9);
	string res = "";
	do
	{
		res += (char)('0' + score % 10);
		score /= 10;
	} while (score > 0);
	reverse(res.begin(), res.end());
	return res;
}

// load string texture
bool LGame::loadString(LScore &curTexture, const std::string &path, const std::string &str)
{
	bool success = true;

	if (!curTexture.loadString(gRenderer, path, str))
	{
		printf("Failed to load string texture\n");
		success = false;
	}

	return success;
}

bool LGame::loadFishImage(LFish &curFish, const std::string &path)
{
	// Loading success flag
	bool success = true;

	// Load sprites
	if (!curFish.loadFromFile(path, gRenderer))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	return success;
}

void LGame::setUpBackGround()
{
	loadBackGroundImage(curBackGround, "../pictures/useful/background/1.png");

	loadBackGroundImage(gameLose, "../pictures/useful/background/gamelose.png");

	loadBackGroundImage(highScore, "../pictures/useful/background/highscore.png");

	loadBackGroundImage(newGame, "../pictures/useful/background/newgame.png");

	loadBackGroundImage(choseBackGround, "../pictures/useful/background/slotgame.png");

	loadBackGroundImage(waitScreen, "../pictures/useful/background/fisheatfish2.png");

	loadBackGroundImage(gBackGround[0], "../pictures/useful/background/1.png");

	loadBackGroundImage(gBackGround[1], "../pictures/useful/background/2.png");
	
	loadBackGroundImage(gBackGround[2], "../pictures/useful/background/3.png");

	loadBackGroundImage(gBackGround[3], "../pictures/useful/background/4.png");

	loadBackGroundImage(gBackGround[4], "../pictures/useful/background/5.png");

	loadBackGroundImage(gBackGround[5], "../pictures/useful/background/6.png");

	for(int i = 0; i < 6; ++i)
		loadBackGroundImage(gSlotGame[i], "../pictures/useful/background/mapbackground.png");

	newGame.setWidth(210);
	newGame.setHeight(140);

	highScore.setWidth(150);
	highScore.setHeight(100);

	for(int i = 0; i < 6; ++i)
		gBackGround[i].setWidth(170), gBackGround[i].setHeight(170); 

	for(int i = 0; i < 6; ++i)
		gSlotGame[i].setWidth(250), gSlotGame[i].setHeight(250);

}

bool LGame::choseMapBackGround(SDL_Event* e)
{
	int id = -1;
	if(e->type == SDL_MOUSEBUTTONDOWN)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		for(int i = 0; i < 6; ++i)
		{
			if(backgroundPosition[i].first + 20 <= x && x <= backgroundPosition[i].first + 190)
			{
				if(backgroundPosition[i].second + 20 <= y && y <= backgroundPosition[i].second + 190)
				{
					id = i;
					break;
				}
			}
		}
	}
	if(id == -1)
		return false;
	++id;
	string path = "../pictures/useful/background/";
	path += toString(id);
	path += ".png";
	curBackGround.free();
	loadBackGroundImage(curBackGround, path);
	return true;
}

bool LGame::loadBackGroundImage(LBackGround &curBackGround, const std::string &path)
{
	// Loading success flag
	bool success = true;

	// Load sprites
	if (!curBackGround.loadFromFile(path, gRenderer))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	return success;
}

bool LGame::loadBombImage(LBomb &curBomb, const std::string &path)
{
	// Loading success flag
	bool success = true;

	// Load sprites
	if (!curBomb.loadFromFile(path, gRenderer))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	return success;
}

void LGame::render(LFish &curFish)
{
	curFish.render(gRenderer, curFish.curPosition.x, curFish.curPosition.y, &curFish.gSpriteClips[curFish.curPart / CNT_FRAME]);
	curFish.curPart++;
	if (curFish.curPart == curFish.numPart * CNT_FRAME)
		curFish.curPart = 0;
}

SDL_Point LGame::lastestMousePosition(SDL_Event *e)
{
	SDL_Point res;
	int x = -1, y = -1;
	res.x = -1, res.y = -1;
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		// Get mouse position
		SDL_GetMouseState(&x, &y);
		res.x = x, res.y = y;
	}

	return res;
}

void LGame::addNewFish()
{
	std::vector<std::pair<int, int>> diff;
	for (int i = 0; i < TOTAL_ANIMAL; ++i)
	{
		if (i == LANTERNSHARK)
			continue;
		diff.push_back(std::make_pair(abs(yourFish.getCurPoint() - gAnimal[i].getCurPoint()), i));
	}
	std::sort(diff.begin(), diff.end());

	std::vector<int> candidates;

	for(int i = 0; i < TOTAL_ANIMAL; ++i)
	{
		if(i == LANTERNSHARK)
			continue;
		if(yourFish.getCurPoint() > gAnimal[i].getCurPoint())
			for(int j = 1; j <= 10; ++j)
				candidates.push_back(i);
	}

	for (int i = 0; i < diff.size(); ++i)
	{
		for (int j = 0; j < cntFish[i]; ++j)
			candidates.push_back(diff[i].second);
	}
	// assert(diff[0].second == EEL);

	random_shuffle(candidates.begin(), candidates.end());

	int pos = randNum(0, (int)candidates.size() - 1);
	int dir = randNum(0, 1);

	int fishType = candidates[pos];

	for (int j = 0; j < totalFish; ++j)
	{
		if (fishOnScreen[j].fishType == fishType && fishOnScreen[j].isOnScreen == false && fishOnScreen[j].right == dir)
		{
			fishOnScreen[j].reset();
			fishOnScreen[j].isOnScreen = true;
			return;
		}
	}
}

bool LGame::startNewGame(SDL_Event *e)
{
	if (e->type == SDL_MOUSEBUTTONDOWN)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (499 <= x && x <= 620 && 555 <= y && y <= 587)
			return true;
	}
	return false;
}

void LGame::loadFish(LFish &curFish, int fishType)
{
	switch (fishType)
	{
	case SHARK:
	{
		loadFishImage(curFish, "../pictures/useful/fish/shark.png");
		curFish.setImagePart(4);
		break;
	}
	case JELLYFISH:
	{
		loadFishImage(curFish, "../pictures/useful/fish/jellyfish.png");
		curFish.setImagePart(4);
		break;
	}
	case TURTLE:
	{
		loadFishImage(curFish, "../pictures/useful/fish/turtle.png");
		curFish.setImagePart(6);
		break;
	}
	case LANTERNSHARK:
	{
		loadFishImage(curFish, "../pictures/useful/fish/lanternshark.png");
		curFish.setImagePart(4);
		break;
	}
	case EEL:
	{
		loadFishImage(curFish, "../pictures/useful/fish/eel.png");
		curFish.setImagePart(6);
		break;
	}
	case OCTOPUS:
	{
		loadFishImage(curFish, "../pictures/useful/fish/octopus.png");
		curFish.setImagePart(6);
		break;
	}
	default:
		loadFishImage(curFish, "../pictures/useful/fish/shark.png");
		curFish.setImagePart(4);
		break;
	}

	for (int i = 0; i < TOTAL_ANIMAL; ++i)
	{
		if (i == fishType)
		{
			curFish.setSize(sz[i]);
			curFish.setPoint(pt[i]);
			curFish.curSpeed = sp[i];
		}
	}
	curFish.fishType = fishType;
}

void LGame::setUpFish()
{
	for (int i = 0; i < TOTAL_ANIMAL; ++i)
	{
		for (int j = 1; j <= 15; ++j)
		{
			loadFish(fishOnScreen[totalFish], i);
			fishOnScreen[totalFish].curPosition.x = 0;
			fishOnScreen[totalFish].curPosition.y = randNum(0, SCREEN_HEIGHT);
			fishOnScreen[totalFish].right = true;
			++totalFish;
		}
		for (int j = 1; j <= 15; ++j)
		{
			loadFish(fishOnScreen[totalFish], i);
			fishOnScreen[totalFish].curPosition.x = SCREEN_WIDTH;
			fishOnScreen[totalFish].curPosition.y = randNum(0, SCREEN_HEIGHT);
			fishOnScreen[totalFish].left = true;
			++totalFish;
		}
	}
}

void LGame::updateFishPosition()
{
	for (int i = 0; i < totalFish; ++i)
	{
		if (!fishOnScreen[i].isOnScreen)
			continue;

		// //If this fish is near a bomb, it swim faster
		int speed = fishOnScreen[i].curSpeed;
		if (fishOnScreen[i].right)
			fishOnScreen[i].curPosition.x += speed;
		else
			fishOnScreen[i].curPosition.x -= speed;
		// std::cout << fishOnScreen[i].curPosition.x << " " << fishOnScreen[i].curPosition.y << '\n';
		if (fishOnScreen[i].curPosition.x < 0 || fishOnScreen[i].curPosition.x > SCREEN_WIDTH)
		{
			fishOnScreen[i].reset();
			continue;
		}

		if (fishOnScreen[i].curPosition.y < 0 || fishOnScreen[i].curPosition.y > SCREEN_HEIGHT)
		{
			fishOnScreen[i].reset();
			continue;
		}
	}
}

void LGame::eatOtherFish()
{
	// Fish eat fish
	for (int i = 0; i < totalFish; ++i)
	{
		if (!fishOnScreen[i].isOnScreen)
			continue;
		int tmp = attackResult(yourFish, fishOnScreen[i]);
		if (tmp == 0)
			continue;
		if (tmp == 1)
		{
			yourFish.setPoint(yourFish.getCurPoint() + fishOnScreen[i].getCurPoint());
			fishOnScreen[i].reset();
			if (curTime - lastTime > COMBO_TIME)
			{
				lastTime = curTime = 0;
				curCombo = 1;
			}
			else
			{
				lastTime = curTime;
				++curCombo;
			}
			Mix_PlayChannel(-1, eatingSound, 0);
		}
		else
			isLose = true;
	}
}

void LGame::fishAI()
{
	for (int i = 0; i < totalFish; ++i)
	{
		if (fishOnScreen[i].isOnScreen)
		{
			if (yourFish.getCurPoint() < fishOnScreen[i].getCurPoint() && fishOnScreen[i].left && fishOnScreen[i].curPosition.x < yourFish.curPosition.x)
				continue;

			if (yourFish.getCurPoint() < fishOnScreen[i].getCurPoint() && fishOnScreen[i].right && fishOnScreen[i].curPosition.x > yourFish.curPosition.x)
				continue;

			if (fishOnScreen[i].haveAI == false && fishOnScreen[i].fishType != OCTOPUS)
				continue;

			if (yourFish.getCurPoint() >= fishOnScreen[i].getCurPoint())
			{
				if(fishOnScreen[i].fishType != OCTOPUS)
				{
					if (yourFish.curPosition.y >= fishOnScreen[i].curPosition.y)
						fishOnScreen[i].curPosition.y -= sp[yourFish.fishType];
					else
						fishOnScreen[i].curPosition.y += sp[yourFish.fishType];
				} 	
				else 
				{
					if (yourFish.curPosition.y >= fishOnScreen[i].curPosition.y)
						fishOnScreen[i].curPosition.y = max(yourFish.curPosition.y, fishOnScreen[i].curPosition.y - 5);
					else
						fishOnScreen[i].curPosition.y = min(yourFish.curPosition.y, fishOnScreen[i].curPosition.y + 5);
				}
			}
			else
			{
				if (yourFish.curPosition.y <= fishOnScreen[i].curPosition.y)
					fishOnScreen[i].curPosition.y -= sp[yourFish.fishType];
				else
					fishOnScreen[i].curPosition.y += sp[yourFish.fishType];
			}
		}
	}
}

int LGame::attackResult(LFish &attackFish, LFish &targetFish)
{
	SDL_Point attackCenter;
	attackCenter.x = yourFish.curPosition.x + yourFish.getCurSize() / 2;
	attackCenter.y = yourFish.curPosition.y + yourFish.getCurSize() / 2;

	SDL_Point targetCenter;
	targetCenter.x = targetFish.curPosition.x + targetFish.getCurSize() / 2;
	targetCenter.y = targetFish.curPosition.y + targetFish.getCurSize() / 2;

	double dis = sqrt(1.0 * (attackCenter.x - targetCenter.x) * (attackCenter.x - targetCenter.x) + 1.0 * (attackCenter.y - targetCenter.y) * (attackCenter.y - targetCenter.y));

	if (dis <= 1.0 * min(attackFish.getCurSize(), targetFish.getCurSize()) / 2)
	{
		if (attackFish.getCurSize() >= targetFish.getCurSize())
			return 1;
		else
			return 2;
	}
	return 0;
}

void LGame::renderScore()
{
	for (int i = 0; i < TOTAL_ANIMAL; ++i)
	{
		render(gAnimal[perm[i]]);
		// cout << gAnimal[i].getHeight() << " " << gAnimal[i].getWidth() << " ";
	}
	// cout << '\n';
	for (int i = 0; i < TOTAL_ANIMAL; ++i)
	{
		textScore[i].free();
		string text = "";
		if (i == LANTERNSHARK)
			text += toString(yourFish.getCurPoint());
		else
			text += toString(gAnimal[i].getCurPoint());
		while ((int)text.length() < 6)
			text += ' ';
		text = ": " + text;
		loadString(textScore[i], "../fonts/gomarice_no_continue.ttf", text);
	}

	for (int i = 0; i < TOTAL_ANIMAL; ++i)
	{
		textScore[perm[i]].setWidth(50), textScore[i].setHeight(30);
		textScore[perm[i]].render(gRenderer, i * 100 + 345, 10);
	}
}

void LGame::renderCombo()
{
	textCombo.free();
	string text = "Combo: x";
	text += toString(curCombo);
	loadString(textCombo, "../fonts/gomarice_no_continue.ttf", text);

	textCombo.setWidth(80), textCombo.setHeight(40);
	textCombo.render(gRenderer, 50, 10);
}

void LGame::hungryMode()
{
	for(int i = 0; i < totalFish; ++i)
	{
		if(fishOnScreen[i].isOnScreen && fishOnScreen[i].getCurPoint() <= yourFish.getCurPoint())
		{
			yourFish.curPosition = fishOnScreen[i].curPosition;
		}
	}
}

void LGame::setUpBomb()
{
	for (int i = 0; i < 10; ++i)
	{
		loadBombImage(bombOnScreen[i], "../pictures/useful/bomb/bomb.png");
		bombOnScreen[i].setWidth(48);
		bombOnScreen[i].setHeight(48);
	}
	for (int i = 0; i < 10; ++i)
	{
		loadBombImage(explosion[i], "../pictures/useful/bomb/explosion1.png");
		explosion[i].setWidth(48);
		explosion[i].setHeight(48);
	}
}

void LGame::addNewBomb()
{
	for (int i = 0; i < 10; ++i)
	{
		if (!bombOnScreen[i].isOnScreen)
		{
			bombOnScreen[i].isOnScreen = true;
			bombOnScreen[i].curPosition.y = 0;
			bombOnScreen[i].curPosition.x = randNum(20, SCREEN_WIDTH - 20);
			return;
		}
	}
}

void LGame::renderBomb()
{
	for (int i = 0; i < 10; ++i)
	{
		if (bombOnScreen[i].isOnScreen)
		{
			bombOnScreen[i].curPosition.y += BOMB_SPEED;
			if (bombOnScreen[i].curPosition.y > SCREEN_HEIGHT)
				bombOnScreen[i].isOnScreen = false;
		}

		if (bombOnScreen[i].isOnScreen)
		{
			bombOnScreen[i].render(gRenderer, bombOnScreen[i].curPosition.x, bombOnScreen[i].curPosition.y);
		}
	}
}

void LGame::checkBombExplosion()
{
	for (int i = 0; i < 10; ++i)
	{
		if (!bombOnScreen[i].isOnScreen)
			continue;
		SDL_Point attackCenter;
		attackCenter.x = bombOnScreen[i].curPosition.x + 24;
		attackCenter.y = bombOnScreen[i].curPosition.y + 24;

		SDL_Point targetCenter;
		targetCenter.x = yourFish.curPosition.x + yourFish.getCurSize() / 2;
		targetCenter.y = yourFish.curPosition.y + yourFish.getCurSize() / 2;

		double dis = sqrt(1.0 * (attackCenter.x - targetCenter.x) * (attackCenter.x - targetCenter.x) + 1.0 * (attackCenter.y - targetCenter.y) * (attackCenter.y - targetCenter.y));

		if (dis <= 1.0 * min(yourFish.getCurSize(), 48) / 2)
		{
			isLose = true;
			addNewExplosion(bombOnScreen[i].curPosition.x, bombOnScreen[i].curPosition.y);
		}
	}
}

void LGame::addNewExplosion(int x, int y)
{
	for (int i = 0; i < 10; ++i)
	{
		if (explosion[i].isOnScreen)
			continue;
		explosion[i].curPosition.x = x;
		explosion[i].curPosition.y = y;
		explosion[i].render(gRenderer, explosion[i].curPosition.x, explosion[i].curPosition.y);
	}
	Mix_PlayChannel(-1, explosionSound, 0);
}

void LGame::bombKillFish()
{
	for (int i = 0; i < totalFish; ++i)
	{
		if (!fishOnScreen[i].isOnScreen)
			continue;

		for (int j = 0; j < 10; ++j)
		{
			if (!bombOnScreen[j].isOnScreen)
				continue;
			SDL_Point attackCenter;
			attackCenter.x = bombOnScreen[j].curPosition.x + 24;
			attackCenter.y = bombOnScreen[j].curPosition.y + 24;

			SDL_Point targetCenter;
			targetCenter.x = fishOnScreen[i].curPosition.x + fishOnScreen[i].getCurSize() / 2;
			targetCenter.y = fishOnScreen[i].curPosition.y + fishOnScreen[i].getCurSize() / 2;

			double dis = sqrt(1.0 * (attackCenter.x - targetCenter.x) * (attackCenter.x - targetCenter.x) + 1.0 * (attackCenter.y - targetCenter.y) * (attackCenter.y - targetCenter.y));

			if (dis <= 40)
			{
				addNewExplosion(bombOnScreen[j].curPosition.x, bombOnScreen[j].curPosition.y);
				fishOnScreen[i].reset();
				bombOnScreen[j].isOnScreen = false;
			}
		}
	}
}

bool LGame::checkWatchingHighScore(SDL_Event *e)
{
	if(e->type != SDL_MOUSEBUTTONDOWN)
		return false;
	if(!isStart || isLose)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		return (502 <= x && x <= 621 && 646 <= y && y <= 684);
	}
	return false;
}

void LGame::renderHighScore()
{
	sort(highScoreVector.begin(), highScoreVector.end(), greater<int>());
	for(int i = 0; i < min(10, (int)highScoreVector.size()); ++i)
	{
		highScoreTexture[i].free();
		string s = "#" + toString(i + 1);
		s += ":";
		string t = toString(highScoreVector[i]);
		while((int)s.length() + (int)t.length() < 70) s += ' ';
		s += t;
		loadString(highScoreTexture[i], "../fonts/high_scores.ttf", s);
		// highScoreTexture[i].setWidth(200);
		// highScoreTexture[i].setHeight(50);
	}
	for(int i = 0; i < min(10, (int)highScoreVector.size()); ++i)
	{
		highScoreTexture[i].render(gRenderer, 420, 50 + 50 * i);
	}
}

void LGame::reset()
{
	yourFish.setSize(sz[LANTERNSHARK]);
	yourFish.setPoint(pt[LANTERNSHARK]);
	for (int i = 0; i < totalFish; ++i)
		fishOnScreen[i].reset();
	for (int i = 0; i < 10; ++i)
		bombOnScreen[i].isOnScreen = false;
	isLose = false;
	myMusic.stopMusic();
	lastTime = curTime = curCombo = 0;
	watchingHighScore = false;
	haveBackGround = false;
	curBackGround.free();
}

LGame::LGame()
{
	gWindow = NULL;
	gRenderer = NULL;
	totalFish = 0;
	isLose = false;
	isStart = false;
	haveBackGround = false;
	explosionSound = NULL;
	watchingHighScore = false;
	lastTime = curTime = curCombo = 0;
}

LGame::~LGame()
{
	free();
}

void LGame::free()
{
	// Free loaded images

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	for (int i = 0; i < TOTAL_ANIMAL; ++i)
		gAnimal[i].free();
	curBackGround.free();
	yourFish.free();
	for (int i = 0; i < totalFish; ++i)
		fishOnScreen[i].free();
	totalFish = 0;
	gameLose.free();
	newGame.free();
	waitScreen.free();
	choseBackGround.free();
	textChoseBackGround.free();
	for(int i = 0; i < 6; ++i)
		gBackGround[i].free();
	for(int i = 0; i < 6; ++i)
		gSlotGame[i].free();
	for (int i = 0; i < TOTAL_ANIMAL; ++i)
		textScore[i].free();
	textCombo.free();
	for (int i = 0; i < 10; ++i)
		bombOnScreen[i].free();
	for (int i = 0; i < 10; ++i)
		explosion[i].free();
	highScore.free();
	isLose = false;
	isStart = false;
	myMusic.free();
	Mix_FreeChunk(explosionSound);
	Mix_FreeChunk(mouseClick);
	Mix_FreeChunk(eatingSound);
	explosionSound = NULL;
	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}