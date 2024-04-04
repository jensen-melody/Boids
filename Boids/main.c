//Boid algorithms developed by Craig Reynolds, adapted by V. Hunter Adams
//https://vanhunteradams.com/Pico/Animal_Movement/Boids-algorithm.html

#include <stdio.h>
#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "./constants.h"

//Global variables used for things :3
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int isRunning = false;
int lastFrameTime = 0;

int windowWidth = defaultWindowWidth;
int windowHeight = defaultWindowHeight;

boid boids[numBoids];

//Calculate two boids distances
float dist(boid boid1, boid boid2) {
	float dx = boid2.pos.x - boid1.pos.x;
	float dy = boid2.pos.y - boid1.pos.y;

	float distance = sqrt(pow(dx, 2) + pow(dy, 2));

	return distance;
}

//Calculate the position of vertecies of boids based on their position and velocity
void calcVertecies(boid boid[], int i) {
	//get center variables
	vector2 center = boid[i].pos;

	//Get r from velocity
	float r = -atan2(boid[i].vel.y,boid[i].vel.x) + 1.571;

	//Get head position
	vector2 head;
	head.x = boidSize * sin(r) + center.x;
	head.y = boidSize * cos(r) + center.y;

	//Get left wing position
	vector2 lWing;
	lWing.x = -boidSize * cos(r) - boidSize * sin(r) + center.x;
	lWing.y = boidSize * sin(r) - boidSize * cos(r) + center.y;

	//Get right wing position
	vector2 rWing;
	rWing.x = boidSize * cos(r) - boidSize * sin(r) + center.x;
	rWing.y = -boidSize * sin(r) - boidSize * cos(r) + center.y;

	//Get trail position
	vector2 tail;
	tail.x = -0.5 * boidSize * sin(r) + center.x;
	tail.y = -0.5 * boidSize * cos(r) + center.y;

	//set boid shape to proper variables
	boid[i].shape.head = head;
	boid[i].shape.lWing = lWing;
	boid[i].shape.rWing = rWing;
	boid[i].shape.tail = tail;
}

//Setup window and it's parameters
int initializeWindow(void) {
	//Initializes SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
			return false;
	}

	//Window variable setup
	window = SDL_CreateWindow(
		"Boids", //Window Title
		SDL_WINDOWPOS_CENTERED, //Xpos
		SDL_WINDOWPOS_CENTERED, //Ypos
		defaultWindowWidth, //Width
		defaultWindowHeight, //Height
		SDL_WINDOW_ALLOW_HIGHDPI //Flags
	);

	//Set window to fullscreen
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	//Get fullscreen size and set windowWidth/Height to it
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	windowWidth = DM.w;
	windowHeight = DM.h;

	//Checks if window initialized correctly
	if (!window) {
		fprintf(stderr, "Error creating SDL Window.\n");
		return false;
	}
	
	//Renderer variable setup 
	renderer = SDL_CreateRenderer(window, -1, 0);

	//Checks if renderer initialized correctly
	if (!renderer) {
		fprintf(stderr, "Error creating SDL Renderer.\n");
		return false;
	}

	return true;
}

//Sets up the scene
void setup() {
	//Sets boid variables
	for (int i = 0; i < numBoids; i++) {
		//random pos
		boids[i].pos.x = rand() % (windowWidth);
		boids[i].pos.y = rand() % (windowHeight);

		//random velocity
		boids[i].vel.x = (((rand() % 60) + 5) / 10);
		if (rand() % 2 == 1) {
			boids[i].vel.x *= -1;
		}
		boids[i].vel.y = (((rand() % 60) + 5) / 10);
		
		//color :D
		boids[i].color.r = 255;
		boids[i].color.g = 255;
		boids[i].color.b = 255;

		//give computer some time to get a new seed for random velocity for y
		if (rand() % 2 == 0) {
			boids[i].vel.y *= -1;
		}
	}
}

//Takes and processes inputs
int processInput() {
	//Init events
	SDL_Event event;
	SDL_PollEvent(&event);

	//Check if certain keys are pressed
	switch (event.type) {
		//If Exit or Alt+F4 button is pressed
		case SDL_QUIT:
			return false;
			break;
		//If key is pressed
		case SDL_KEYDOWN:
			//If key pressed is Esc
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				return false;
			}
			//If key pressed is R
			if (event.key.keysym.sym == SDLK_r) {
				setup();
			}
			break;

	}
}

//Update objects every frame
void update() {
	//Wait an amount of time until target FPS reached
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), lastFrameTime + targetFrameTime));
	lastFrameTime = SDL_GetTicks();


	//Separation
	vector2 closeBoids;
	vector2 avgVel;
	vector2 avgPos;
	int neighboringBoids;

	//loop through every boid
	for (int i = 0; i < numBoids; i++) {
		//Zero vars
		closeBoids.x = 0;
		closeBoids.y = 0;
		avgVel.x = 0;
		avgVel.y = 0;
		neighboringBoids = 0;
		avgPos.x = 0;
		avgPos.y = 0;

		//Loop through ever other boid
		for (int j = 0; j < numBoids; j++) {
			if (i != j) {
				//If other boid is within protected range
				if (dist(boids[i], boids[j]) < protectedRange) {
					closeBoids.x += boids[i].pos.x - boids[j].pos.x;
					closeBoids.y += boids[i].pos.y - boids[j].pos.y;
				}
				//If other boid distance is within visual range
				if (dist(boids[i], boids[j]) < visualRange) {
					avgVel.x += boids[j].vel.x;
					avgVel.y += boids[j].vel.y;
					avgPos.x += boids[j].pos.x;
					avgPos.y += boids[j].pos.y;
					neighboringBoids++;
				}
			}
		}
		//Average Velocities/Positions
		if (neighboringBoids > 0) {
			avgVel.x = avgVel.x / neighboringBoids;
			avgVel.y = avgVel.y / neighboringBoids;
			avgPos.x = avgPos.x / neighboringBoids;
			avgPos.y = avgPos.y / neighboringBoids;
		}

		//Update velocities accordingly
		boids[i].vel.x += closeBoids.x * avoidFactor;
		boids[i].vel.y += closeBoids.y * avoidFactor;
		boids[i].vel.x += (avgVel.x - boids[i].vel.x) * alignFactor;
		boids[i].vel.y += (avgVel.y - boids[i].vel.y) * alignFactor;
		boids[i].vel.x += (avgPos.x - boids[i].pos.x) * centeringFactor;
		boids[i].vel.y += (avgPos.y - boids[i].pos.y) * centeringFactor;
	}

	//Edge maneuvering
	for (int i = 0; i < numBoids; i++) {
		if (boids[i].pos.x < margin) {
			boids[i].vel.x += turnFactor;
		}
		if (boids[i].pos.x > windowWidth - margin) {
			boids[i].vel.x -= turnFactor;
		}
		if (boids[i].pos.y < margin) {
			boids[i].vel.y += turnFactor;
		}
		if (boids[i].pos.y > windowHeight - margin) {
			boids[i].vel.y -= turnFactor;
		}
	}


	//Clamp Speed
	for (int i = 0; i < numBoids; i++) {
		//Calc speed
		float speed = sqrt(pow(boids[i].vel.x, 2) + pow(boids[i].vel.y, 2));

		//If too fast
		if (speed > maxSpeed) {
			boids[i].vel.x = (boids[i].vel.x / speed) * maxSpeed;
			boids[i].vel.y = (boids[i].vel.y / speed) * maxSpeed;
		}

		//If too slow
		if (speed < minSpeed) {
			boids[i].vel.x = (boids[i].vel.x / speed) * minSpeed;
			boids[i].vel.y = (boids[i].vel.y / speed) * minSpeed;
		}
	}
	

	//Update boid position
	for (int i = 0; i < numBoids; i++) {
		boids[i].pos.x += boids[i].vel.x;
		boids[i].pos.y += boids[i].vel.y;
	}
}

//Tell renderer to show objects on screen
void render() {
	//Draw background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	//Clear Screen
	SDL_RenderClear(renderer);

	////Set font
	//TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);

	////Set color to white
	//SDL_Color White = { 255, 255, 255 };

	////Render Text
	//SDL_Surface* surfaceMessage =
	//	TTF_RenderText_Solid(Sans, "Press R to Reset", White);

	//// now you can convert it into a texture
	//SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	//SDL_Rect Message_rect; //create a rect
	//Message_rect.x = 0;  //controls the rect's x coordinate 
	//Message_rect.y = 0; // controls the rect's y coordinte
	//Message_rect.w = 100; // controls the width of the rect
	//Message_rect.h = 100; // controls the height of the rect

	//// and coordinate of your texture
	//SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

	//// Don't forget to free your surface and texture
	//SDL_FreeSurface(surfaceMessage);
	//SDL_DestroyTexture(Message);

	//Loop through every boid
	for (int i = 0; i < numBoids; i++) {
		//Calculate boid shape vertecies from position and rotation
		calcVertecies(boids, i);

		//set vertecies
		SDL_Vertex vhead = {{boids[i].shape.head.x,boids[i].shape.head.y}, {boids[i].color.r,boids[i].color.g,boids[i].color.b,255}, {1,1}};
		SDL_Vertex vlWing = { {boids[i].shape.lWing.x,boids[i].shape.lWing.y}, {boids[i].color.r,boids[i].color.g,boids[i].color.b,255}, {1,1} };
		SDL_Vertex vrWing = { {boids[i].shape.rWing.x,boids[i].shape.rWing.y}, {boids[i].color.r,boids[i].color.g,boids[i].color.b,255}, {1,1} };
		SDL_Vertex vtail = { {boids[i].shape.tail.x,boids[i].shape.tail.y}, {boids[i].color.r,boids[i].color.g,boids[i].color.b,255}, {1,1} };

		//Put boid shape vertecies into array
		SDL_Vertex vertices[] = {
			vhead,
			vlWing,
			vtail,
			vrWing
		};

		//Render boid
		const int indices[] = {
		   0,1,2,
		   2,3,0
		};
		SDL_RenderGeometry(renderer, NULL, vertices, 4, indices, 6);

	}

	//Swap buffer frame for current frame (Draws frame)
	SDL_RenderPresent(renderer);
}

//Uninitializes everything in reverse order it initialized it
void destroyWindow() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//main function :D
int main(int argc, char* argv[]) {
	//Initialize RNG
	srand(time(NULL));

	//Set isRunning variable to true
	int isRunning = initializeWindow();

	setup();

	//Main Game Loop
	while (isRunning) {
		isRunning = processInput();
		update();
		render();
	}

	//If isRunning is false, or loop breaks suddenly, destroy the window and uninitialize everything
	destroyWindow();

	return false;
}