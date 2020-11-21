/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 800;

//Starts up SDL and creates window
bool init();

SDL_Texture* loadTexture(std::string path);

//Loads media
bool loadMedia(std::string path);

bool isCollision(SDL_Rect renderArea1, SDL_Rect renderArea2);

bool compare(int number1, int number2);

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

SDL_Texture* snakeSegImage = NULL;

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

SDL_Texture* loadTexture(std::string path)
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

bool loadMedia(std::string path)
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

//the init(), loadtexture, loadmedia, and close functions are provided from a SDL tutorial mentioned at the top of the file
//98% of the rest of the code is mine.  Only code that isnt is the event handle and initialization loop at the start of main


//This is the SnakeSegment class, it holds the values for each segment spawned.
class SnakeSegment
{
public:

	//default constructor, initializes all of the members of the class to base values
	SnakeSegment()
	{
		segmentPosX = 0;
		segmentPosY = 0;
		setIsX(true);
		setVelocity(0);
	}

	//getters and setters
	int getVelocity()
	{
		return segmentVelocity;
	}

	//sets the velocity according to the defined min and max velocities.
	//the min and max velocities are related to the size of the segment for movement purposes explained later
	void setVelocity(int vel)
	{
		if (vel > maxVel)
		{
			std::cout << "Velocity is too large";
		}
		else if (vel < minVel)
		{
			std::cout << "Velocity is too small";
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

	//changes the isX boolean which is responsible for telling if the segment is moving in the x or y direction. true = x false = y
	void setIsX(bool currentDir)
	{
		isX = currentDir;
	}

	int getPosX()
	{
		return segmentPosX;
	}

	//the setPosX function changes the x position of the segment based on the velocity passed through.
	//the if statements are to bound check so the objects cannout go off the screen.
	void setPosX(int velocity)
	{
		int minPos = 0;
		int maxPos = screenSizeX;
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

	//the setPosY function changes the y position of the segment based on the velocity passed through.
	//the if statements are to bound check so the objects cannout go off the screen.
	void setPosY(int velocity)
	{
		int minPos = 0;
		int maxPos = screenSizeY;
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

	//these two functions are used to set the position unrelated to the velocity, generally used to initialize a segment when it is spawning in
	//also used to set the segment position to the segment in front of its position for the snake segmenet follow logic
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

	std::string getImage()
	{
		return image;
	}

	int getMinVel()
	{
		return minVel;
	}

	int getMaxVel()
	{
		return maxVel;
	}

private:

	//depending on key clicked, velocity will change
	int segmentVelocity = 0;
	bool isX = true;
	int maxVel = 50;
	int minVel = -50;
	int segmentPosX, segmentPosY;
	int screenSizeY = 800;
	int screenSizeX = 1024;
	const std::string image = "dotTest3.png";
	//segment length
	static const int segmentLength = 50;
	static const int segmentWidth = 50;
};

//collision detection logic
bool isCollision(SDL_Rect renderArea1, SDL_Rect renderArea2)
{
	bool x = false;
	bool y = false;

	//bounding box coordinates related to the render area.  renderarea.x and renderarea.y are the top left x and y coordinates of the
	//SDL_Rect.  Adding the width to the x gives us the right side of the rectangle, and adding the heigh to the y
	//gives us the bottom of the rectangle.
	int bound1[4] = {renderArea1.x, renderArea1.x + renderArea1.w, renderArea1.y, renderArea1.y + renderArea1.h};
	int bound2[4] = {renderArea2.x, renderArea2.x + renderArea2.w, renderArea2.y, renderArea2.y + renderArea2.h};
	
	int left1 = bound1[0];
	int right1 = bound1[1];
	int top1 = bound1[2];
	int bot1 = bound1[3];

	int left2 = bound2[0];
	int right2 = bound2[1];
	int top2 = bound2[2];
	int bot2 = bound2[3];

	//we compare the y coordinates of the rectangles
	if ((top1 <= bot2 && top1 >= top2) || (bot1 >= top2 && bot1 <= bot2))
	{
		y = true;
	}
	//then we compare the x coordinates of the rectangles
	if ((left1 <= right2 && left1 >= left2) || (right1 >= left2 && right1 <= right2))
	{
		x = true;
		
	}

	//if and only if the x coordinates are crossing and the y coordinates are crossing, are the objects colliding
	return (y && x);
}

int main(int argc, char* args[])
{

	//array that holds all of our snake segments
	//because of how c++ works, creating that array actually initializes all of the segments for us.
	//used to have an initialize functions because i hadn't realized that fact.  In C# for example i believe you would have to initialize
	//each element in the area
	SnakeSegment segment [100];
	//lose state boolean, we check it at the end of the main function
	bool lose = false;
	int velocity = 0;
	bool isX = true;
	int x = 0, y = 0;
	//the SDL rectangle used for the render size for specifically the first segment
	SDL_Rect headArea = { segment[0].getPosX(), segment[0].getPosY() , segment[0].getSegLength(), segment[0].getSegWidth()};
	//rectangle used to render the subsequent snake segments, also used for lose state as to make an additional rectangle
	SDL_Rect tailArea = { 400, 400, segment[0].getSegLength(), segment[0].getSegLength()};
	//rectangle used for the square that the snake is trying to eat
	SDL_Rect squareRenderArea = {200, 200, 100, 100};
	int w = 0;
	bool rendered = false;
	int count = 0;
	int timer = 0;

	srand(time(NULL));
	
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

					//checking if a key has been pressed
					if (e.type = SDL_KEYDOWN && e.key.repeat == 0)
					{
						//checking which key has been pressed and then adjusting the isX variable and velocity variable.
						//it first checks if the segment is going in the opposite direction of the button click
						//if it is then the values dont change, this is to prevent the snake from doing a 180 in direction
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							if (segment[0].getIsX() == false && segment[0].getVelocity() == segment[0].getMaxVel())
							{
								break;
							}
							segment[0].setIsX(false);
							segment[0].setVelocity(segment[0].getMinVel());
							break;
						case SDLK_DOWN:
							if (segment[0].getIsX() == false && segment[0].getVelocity() == segment[0].getMinVel())
							{
								break;
							}
							segment[0].setIsX(false);
							segment[0].setVelocity(segment[0].getMaxVel());
							break;
						case SDLK_LEFT:
							if (segment[0].getIsX() == true && segment[0].getVelocity() == segment[0].getMaxVel())
							{
								break;
							}
							segment[0].setIsX(true);
							segment[0].setVelocity(segment[0].getMinVel());
							break;
						case SDLK_RIGHT:
							if (segment[0].getIsX() == true && segment[0].getVelocity() == segment[0].getMinVel())
							{
								break;
							}
							segment[0].setIsX(true);
							segment[0].setVelocity(segment[0].getMaxVel());
							break;
						}
					}
				}
				
				// since this is in a while loop we dont want to render this a bunch of times so they have a switch
				if (!rendered)
				{
					snakeSegImage = loadTexture(segment[0].getImage());
				}
				if (!rendered)
				{
					blackSquare = loadTexture("blackSquare.png");
				}
				if (!rendered)
				{
					endScreen = loadTexture("endScreen.png");
				}
				rendered = true;

				
				
				

				//clear screen
				SDL_RenderClear(gRenderer);

				//Render Texture to screen
				//render snake segment at the headarea
				SDL_RenderCopy(gRenderer, snakeSegImage, NULL, &headArea);
				
				//render black square(object to be eaten) at square render area
				SDL_RenderCopy(gRenderer, blackSquare, NULL, &squareRenderArea);

				//check collision between head and the square
				if (isCollision(headArea, squareRenderArea))
				{
					//randomize the render area of the next square
					count++;
					int c1 = rand() % (1024 - 100) + 1;
					int c2 = rand() % (800 - 100) + 1;
					squareRenderArea = { c1, c2, 100, 100 };

					//count is how many segments there are.
					//this code sets the new segment equal to the direction and velocity of the one in front of it
					segment[count].setIsX(segment[count - 1].getIsX());
					segment[count].setVelocity(segment[count - 1].getVelocity());

					//sets the x and y positions of the new segment based on the direction and velocity of the segment in
					//front of it so that the new segment is directly behind the segment in front of it
					if (segment[count - 1].getIsX() && segment[count - 1].getVelocity() == segment[0].getMaxVel())
					{
						segment[count].setSegmentSpotX(segment[count - 1].getPosX() - segment[count].getSegWidth());
						segment[count].setSegmentSpotY(segment[count - 1].getPosY());
					}
					else if (segment[count - 1].getIsX() && segment[count - 1].getVelocity() == segment[0].getMinVel())
					{
						segment[count].setSegmentSpotX(segment[count - 1].getPosX() + segment[count].getSegWidth());
						segment[count].setSegmentSpotY(segment[count - 1].getPosY());
					}
					else if (!segment[count - 1].getIsX() && segment[count - 1].getVelocity() == segment[0].getMaxVel())
					{
						segment[count].setSegmentSpotY(segment[count - 1].getPosY() - segment[count].getSegLength());
						segment[count].setSegmentSpotX(segment[count - 1].getPosX());
					}
					else if (!segment[count - 1].getIsX() && segment[count - 1].getVelocity() == segment[0].getMinVel())
					{
						segment[count].setSegmentSpotY(segment[count - 1].getPosY() + segment[count].getSegLength());
						segment[count].setSegmentSpotX(segment[count - 1].getPosX());
					}

					
				}

				//tails follow head
				//for each tail(count) set the tail area to its position and render it
				for (int i = count; i > 0; i--)
				{

					tailArea.x = segment[i].getPosX();
					tailArea.y = segment[i].getPosY();
					SDL_RenderCopy(gRenderer, snakeSegImage, NULL, &tailArea);
					
					//this is the same incrememnt for when the head moves, so everything moves at the same time
					//move each tail x and y position to the position in front of it as well its velocity and isX values
					//updating the velocity and isX values are important for the spawning of tail segments in the collision check
					if (timer % segment[0].getSegLength() == 0)
					{
						//update tail segment to one in front it position wise.
						segment[i].setVelocity(segment[i - 1].getVelocity());
						segment[i].setIsX(segment[i - 1].getIsX());
						segment[i].setSegmentSpotX(segment[i - 1].getPosX());
						segment[i].setSegmentSpotY(segment[i - 1].getPosY());
					}
				}

				//this is the if statement that governs how often the head segment updates
				//once the timer has increased an amount equal to the same number as the size of the segment the update can happen
				//the way this works in reality is it simulates a velocity of 1. Every 50 updates the head will move 50 pixels
				//so it "moves" 1 pixel per update.  This does make the game look a bit jittery and choppy, but this was implemented to
				//solve an issue where when you would set the snake segment to the one in front of its position, they would just start
				//stacking with a difference of 1 pixel (because that was the velocity)
				//with the 50 pixel jumps, they are separated by just as many pixels
				//also because of how timer is updated this is frame based, so the faster your computer, the faster the snake will move
				//also had head movement above tail update but that would make it so that on the first square you ate
				//a new tail segment would spawn but on top of the head, putting it after the tail update fixes that issue.
				if (timer % segment[0].getSegLength() == 0)
				{
					//check which direction the segment is going
					if (segment[0].getIsX())
					{
						//update its position based on its velocity
						segment[0].setPosX(segment[0].getVelocity());
						//if the segment is about to go out of bounds with this update set the lose state to true
						if (segment[0].getPosX() > 1024 || segment[0].getPosX() < 0)
						{
							lose = true;
						}
						//update the headarea (the render position for the head) to the new x and y values 
						headArea = { segment[0].getPosX(), segment[0].getPosY(), segment[0].getSegLength(), segment[0].getSegWidth() };
					}
					//same as above just in different direction
					else if (!segment[0].getIsX())
					{
						segment[0].setPosY(segment[0].getVelocity());
						if (segment[0].getPosY() > 800 || segment[0].getPosY() < 0)
						{
							lose = true;
						}
						headArea = { segment[0].getPosX(), segment[0].getPosY(), segment[0].getSegLength(), segment[0].getSegWidth() };
					}
				}

				timer++;

				//lose state check
				if (lose)
				{
					SDL_RenderClear(gRenderer);
					tailArea = { 0,0, 1024, 800 };
					SDL_RenderCopy(gRenderer, endScreen, NULL, &tailArea);
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