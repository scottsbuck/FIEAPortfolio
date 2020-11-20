/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
//WHADDUP
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 800;

//Starts up SDL and creates window
bool init();

SDL_Texture* loadTexture(string path);

//Loads media
bool loadMedia(string path);

bool isCollision(SDL_Rect renderArea1, SDL_Rect renderArea2);

bool compare(int number1, int number2);

int search(int** turns, int posX, int posY, int turnCount);

//Frees media and shuts down SDL
void close();

//Loads individual image
//SDL_Surface* loadSurface(string path);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Renderer* gRenderer = NULL;

// SDL_Surface* gScreenSurface = NULL;

//Current displayed PNG image
//SDL_Surface* gPNGSurface = NULL;
SDL_Texture* gTexture = NULL;

SDL_Texture* lTexture = NULL;

SDL_Texture* blackSquare = NULL;

SDL_Texture* endScreen = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{	
			//create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//initialize png loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

SDL_Texture* loadTexture(string path)
{
	//final texture
	SDL_Texture* newTexture = NULL;
	//load immage at path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error %s\n", path.c_str(), SDL_GetError());
		}
		
		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);

	}
	return newTexture;

}

bool loadMedia(string path)
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	gTexture = loadTexture(path);
	if (gTexture == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

class SnakeSegment
{
public:

	//getters and setters

	SnakeSegment()
	{
		segmentPosX = 0;
		segmentPosY = 0;
		setIsX(true);
		setVelocity(0);

	}

	void initialize()
	{
		segmentPosX = 0;
		segmentPosY = 0;
		setIsX(true);
		setVelocity(0);
	}

	int getVelocity()
	{
		return segmentVelocity;
	}

	void setVelocity(int vel)
	{
		int maxVel = 50;
		int minVel = -50;

		if (vel > maxVel)
		{
			cout << "Velocity is too large";
		}
		else if (vel < minVel)
		{
			cout << "Velocity is too small";
		}
		else
		{
			segmentVelocity = vel;
		}


	}

	bool getIsX()
	{
		return isX;
	}

	void setIsX(bool maybe)
	{
		isX = maybe;
	}

	int getPosX()
	{
		return segmentPosX;
	}

	void setPosX(int velocity)
	{
		int minPos = 0;
		int maxPos = 1024;
		int newPos = segmentPosX += velocity;

		if (newPos > maxPos || newPos < minPos)
		{
			segmentPosX = segmentPosX;
		}
		else
		{
			segmentPosX = newPos;
		}
	}


	int getPosY()
	{
		return segmentPosY;
	}

	void setPosY(int velocity)
	{
		int minPos = 0;
		int maxPos = 800;
		int newPos = segmentPosY += velocity;

		if (newPos > maxPos || newPos < minPos)
		{
			segmentPosY = segmentPosY;
		}
		else
		{
			segmentPosY = newPos;
		}

	}

	void setSegmentSpotX(int newSpot)
	{
		segmentPosX = newSpot;
	}

	void setSegmentSpotY(int newSpot)
	{
		segmentPosY = newSpot;
	}


	int getSegLength()
	{
		return segmentLength;
	}

	int getSegWidth()
	{
		return segmentWidth;
	}

	void handleButton(SDL_Event e)
	{
		 
	}

	string getImage()
	{
		return image;
	}

private:

	//depending on key clicked, velocity will change
	int segmentVelocity = 0;
	bool isX = true;

	int segmentPosX, segmentPosY;
	const string image = "dotTest3.png";
	//segment length
	static const int segmentLength = 50;
	static const int segmentWidth = 50;
};

bool isCollision(SDL_Rect renderArea1, SDL_Rect renderArea2)
{
	bool x = false;
	bool y = false;

	int xRadius1 = renderArea1.w / 2;
	int xRadius2 = renderArea2.w / 2;
	int yRadius1 = renderArea1.h / 2;
	int yRadius2 = renderArea2.h / 2;

	int boundx[4] = {renderArea1.x - xRadius1, renderArea1.x + xRadius1, renderArea2.x - xRadius2, renderArea2.x + xRadius2};
	int boundy[4] = {renderArea1.y - yRadius1, renderArea1.y + yRadius1, renderArea2.y - yRadius2, renderArea2.y + yRadius2};
	
	int top1 = boundy[0];
	int bot1 = boundy[1];
	int left1 = boundx[0];
	int right1 = boundx[1];

	int top2 = boundy[2];
	int bot2 = boundy[3];
	int left2 = boundx[2];
	int right2 = boundx[3];

	if ((top1 <= bot2 && top1 >= top2) || (bot1 >= top2 && bot1 <= bot2))
	{
		y = true;
	}
	if ((left1 <= right2 && left1 >= left2) || (right1 >= left2 && right1 <= right2))
	{
		x = true;
		
	}

	return (y && x);
}

bool compare(int number1, int number2)
{
	if (number1 == number2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int search(int ** turns, int posX, int posY, int turnCount)
{
	int count = 0;
	while (turns[2][count] == -1)
	{
		count++;
	}
	for (int i = count; count < turnCount; count++)
	{
		if (compare(turns[0][count], posX) && compare(turns[1][count], posY))
		{
			return turns[2][count];
		}
	}
	return -1;
}

int main(int argc, char* args[])
{

	SnakeSegment segment [100];
	bool lose = false;
	segment[0].initialize();
	int velocity = 0;
	bool isX = true;
	int x = 0, y = 0;
	bool button = false;
	SDL_Rect renderArea = { segment[0].getPosX(), segment[0].getPosY() , segment[0].getSegLength(), segment[0].getSegWidth()};
	SDL_Rect renderSpot = { 400, 400, segment[0].getSegLength(), segment[0].getSegLength() };
	SDL_Rect squareRenderArea = {200, 200, 100, 100};
	int w = 0;
	int rendered = 0;
	int count = 0;
	int timer = 0;
	
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia("whiteBack.png"))
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					if (e.type = SDL_KEYDOWN && e.key.repeat == 0)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							if (segment[0].getIsX() == false && segment[0].getVelocity() == 50)
							{
								break;
							}
							segment[0].setIsX(false);
							segment[0].setVelocity(-50);
							//button = true;
							break;
						case SDLK_DOWN:
							if (segment[0].getIsX() == false && segment[0].getVelocity() == -50)
							{
								break;
							}
							segment[0].setIsX(false);
							segment[0].setVelocity(50);
							//button = true;
							break;
						case SDLK_LEFT:
							if (segment[0].getIsX() == true && segment[0].getVelocity() == 50)
							{
								break;
							}
							segment[0].setIsX(true);
							segment[0].setVelocity(-50);
							//button = true;
							break;
						case SDLK_RIGHT:
							if (segment[0].getIsX() == true && segment[0].getVelocity() == -50)
							{
								break;
							}
							segment[0].setIsX(true);
							segment[0].setVelocity(50);
							//button = true;
							break;
						}
					}
				}
				
				if (rendered == 0)
				{
					lTexture = loadTexture(segment[0].getImage());
				}
				if (rendered == 0)
				{
					blackSquare = loadTexture("blackSquare.png");
				}
				if (rendered == 0)
				{
					endScreen = loadTexture("endScreen.png");
				}
				rendered = 1;

				if (timer % segment[0].getSegLength() == 0)
				{
					if (segment[0].getIsX())
					{
						segment[0].setPosX(segment[0].getVelocity());
						if (segment[0].getPosX() > 1024 || segment[0].getPosX() < 0)
						{
							lose = true;
						}
						renderArea = { segment[0].getPosX(), segment[0].getPosY(), segment[0].getSegLength(), segment[0].getSegWidth() };
					}
					else if (!segment[0].getIsX())
					{
						segment[0].setPosY(segment[0].getVelocity());
						if (segment[0].getPosY() > 800 || segment[0].getPosY() < 0)
						{
							lose = true;
						}
						renderArea = { segment[0].getPosX(), segment[0].getPosY(), segment[0].getSegLength(), segment[0].getSegWidth() };
					}
				}
				
				

				//clear screen
				SDL_RenderClear(gRenderer);

				//Render Texture to screen
				
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
				
				SDL_RenderCopy(gRenderer, lTexture, NULL, &renderArea);
				
				SDL_RenderCopy(gRenderer, blackSquare, NULL, &squareRenderArea);

				if (isCollision(renderArea, squareRenderArea))
				{
					count++;
					int c1 = rand() % (1024 - 100) + 1;
					int c2 = rand() % (800 - 100) + 1;
					squareRenderArea = { c1, c2, 100, 100 };

					segment[count].initialize();
					segment[count].setIsX(segment[count - 1].getIsX());
					segment[count].setVelocity(segment[count - 1].getVelocity());

					if (segment[count - 1].getIsX() && segment[count - 1].getVelocity() == 50)
					{
						segment[count].setSegmentSpotX(segment[count - 1].getPosX() - segment[count].getSegWidth());
						segment[count].setSegmentSpotY(segment[count - 1].getPosY());
					}
					else if (segment[count - 1].getIsX() && segment[count - 1].getVelocity() == -50)
					{
						segment[count].setSegmentSpotX(segment[count - 1].getPosX() + segment[count].getSegWidth());
						segment[count].setSegmentSpotY(segment[count - 1].getPosY());
					}
					else if (!segment[count - 1].getIsX() && segment[count - 1].getVelocity() == 50)
					{
						segment[count].setSegmentSpotY(segment[count - 1].getPosY() - segment[count].getSegLength());
						segment[count].setSegmentSpotX(segment[count - 1].getPosX());
					}
					else if (!segment[count - 1].getIsX() && segment[count - 1].getVelocity() == -50)
					{
						segment[count].setSegmentSpotY(segment[count - 1].getPosY() + segment[count].getSegLength());
						segment[count].setSegmentSpotX(segment[count - 1].getPosX());
					}

					
				}

				//tails follow head
				for (int i = count; i > 0; i--)
				{

					renderSpot.x = segment[i].getPosX();
					renderSpot.y = segment[i].getPosY();
					SDL_RenderCopy(gRenderer, lTexture, NULL, &renderSpot);

					if (timer % segment[0].getSegLength() == 0)
					{
						segment[i].setSegmentSpotX(segment[i - 1].getPosX());
						segment[i].setSegmentSpotY(segment[i - 1].getPosY());
					}
					

					/*if (segment[i -1].getVelocity() == 10 && segment[i -1].getIsX())
					{
						segment[i].setVelocity(10);
						segment[i].setIsX(true);
						segment[i].setSegmentSpotX(segment[i - 1].getPosX() - segment[i].getSegLength());
						segment[i].setSegmentSpotY(segment[i - 1].getPosY());
					}
					else if (segment[i - 1].getVelocity() == -10 && segment[i - 1].getIsX())
					{
						segment[i].setVelocity(-10);
						segment[i].setIsX(true);
						segment[i].setSegmentSpotX(segment[i - 1].getPosX() + segment[i].getSegLength());
						segment[i].setSegmentSpotY(segment[i - 1].getPosY());
					}
					else if (segment[i - 1].getVelocity() == 10 && !segment[i - 1].getIsX())
					{
						segment[i].setVelocity(10);
						segment[i].setIsX(false);
						segment[i].setSegmentSpotX(segment[i - 1].getPosX());
						segment[i].setSegmentSpotY(segment[i - 1].getPosY() - segment[i].getSegLength());
					}
					else if (segment[i - 1].getVelocity() == -10 && !segment[i - 1].getIsX())
					{
						segment[i].setVelocity(-10);
						segment[i].setIsX(false);
						segment[i].setSegmentSpotX(segment[i - 1].getPosX());
						segment[i].setSegmentSpotY(segment[i - 1].getPosY() + segment[i].getSegLength());
					}*/
					
					
				}
				timer++;
					/*if (button == true)
					{
						segment[i].setVelocity(segment[i - 1].getVelocity());
						segment[i].setIsX(segment[i - 1].getIsX());
					}
					if (segment[i].getIsX())
					{
						segment[i].setPosX(segment[i].getVelocity());
					}
					else if (!segment[i].getIsX())
					{
						segment[i].setPosY(segment[i].getVelocity());
					}
				}
				button = false;*/

				/*for (int i = 0; i < count; i++)
				{
					SnakeSegment* temp = new SnakeSegment();
					body->next = temp;

					temp->setSegmentSpotX(body->getPosX());
					temp->setSegmentSpotY(body->getPosY());
					if (body->getIsX() && body->getVelocity() == 1)
					{
						temp->setSegmentSpotX(body->getPosX() - temp->getSegWidth());
					}
					else if (body->getIsX() && body->getVelocity() == -1)
					{
						temp->setSegmentSpotX(body->getPosX() + temp->getSegWidth());
					}
					else if (!body->getIsX() && body->getVelocity() == 1)
					{
						temp->setSegmentSpotY(body->getPosY() - temp->getSegLength());
					}
					else if (!body->getIsX() && body->getVelocity() == -1)
					{
						temp->setSegmentSpotY(body->getPosY() + temp->getSegLength());
					}
					renderSpot = { temp->getPosX(), temp->getPosY(), temp->getSegLength(), temp->getSegWidth() };
					SDL_RenderCopy(gRenderer, lTexture, NULL, &renderSpot);
					body = body->next;
				}*/

				if (lose)
				{
					SDL_RenderClear(gRenderer);
					renderSpot = { 0,0, 1024, 800 };
					SDL_RenderCopy(gRenderer, endScreen, NULL, &renderSpot);
				}

				//update screen
				SDL_RenderPresent(gRenderer);
				
				//Update the surface
				SDL_UpdateWindowSurface(gWindow);


			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}