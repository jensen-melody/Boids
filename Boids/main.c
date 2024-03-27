#include <stdio.h>
#include <SDL.h>
#include <time.h>
#include <stdlib.h>
#include "./constants.h"

//Global variables used for things :3
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int isRunning = false;
int lastFrameTime = 0;

//boid structure for placing boids
struct boid {
	float x;
	float y;
	float dx;
	float dy;
	float width;
	float height;
	int r;
	int g;
	int b;
}boids[numBoids];

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
		windowWidth, //Width
		windowHeight, //Height
		SDL_WINDOW_ALLOW_HIGHDPI //Flags
	);

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
	//TODO: Setup

	//Sets boid variables
	boids[0].x = 0;
	boids[0].y = 0;
	boids[0].dx = 3;
	boids[0].dy = 3;
	boids[0].width = 80;
	boids[0].height = 60;
	boids[0].r = 255;
	boids[0].g = 255;
	boids[0].b = 255;

	boids[1].x = 500;
	boids[1].y = 356;
	boids[1].dx = -4.6;
	boids[1].dy = 2;
	boids[1].width = 80;
	boids[1].height = 60;
	boids[1].r = 255;
	boids[1].g = 255;
	boids[1].b = 255;
}

//Takes and processes inputs
int processInput() {
	//Init events
	SDL_Event event;
	SDL_PollEvent(&event);

	//Check if certain keys are pressed
	switch (event.type) {
		//If Exit, Alt+F4, or Win button is pressed
		case SDL_QUIT: 
			printf("Quit");
			return false;
			break;
		//If X is pressed
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				printf("Esc");
				return false;
			}
			break;

	}
}

//Update objects every frame
void update() {
	//Fixed FPS stuff
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), lastFrameTime + targetFrameTime));
	lastFrameTime = SDL_GetTicks();

	for (int i = 0; i < numBoids; i++) {
		//If boid hits side edges
		if (boids[i].x < 0 || boids[i].x >(windowWidth - boids[i].width)) {
			boids[i].dx *= -1;
			boids[i].r = rand() % 256;
			boids[i].g = rand() % 256;
			boids[i].b = rand() % 256;
		}

		//If boid hits side edges
		if (boids[i].y < 0 || boids[i].y >(windowHeight - boids[i].height)) {
			boids[i].dy *= -1;
			boids[i].r = rand() % 256;
			boids[i].g = rand() % 256;
			boids[i].b = rand() % 256;
		}

		//Update boid pos
		boids[i].x += boids[i].dx;
		boids[i].y += boids[i].dy;

	}
}

//Tell renderer to show objects on screen
void render() {
	//Draws background color
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//Clears screen
	SDL_RenderClear(renderer);

	//Draw Rectangle
	for (int i = 0; i < numBoids; i++) {
		SDL_Rect rectangle = { boids[i].x, boids[i].y, boids[i].width, boids[i].height };

		SDL_SetRenderDrawColor(renderer, boids[i].r, boids[i].g, boids[i].b, 255);
		SDL_RenderFillRect(renderer, &rectangle);
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