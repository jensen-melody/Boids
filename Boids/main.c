#include <stdio.h>
#include <SDL.h>
#include "./constants.h"

//Global variables used for things :3
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int isRunning = false;
int lr = 1;

//ball structure for placing rectangles
struct ball {
	float x;
	float y;
	float width;
	float height;
}ball;

//Setup window and it's parameters
int initializeWindow(void) {
	//Initializes SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
			return false;
	}

	//Window variable setup
	window = SDL_CreateWindow(
		"Gay Sex", //Window Title
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

	//Sets ball variables
	ball.x = 0;
	ball.y = 0;
	ball.width = 400;
	ball.height = 600;
}

//Takes and processes inputs
int processInput() {
	//init events
	SDL_Event event;
	SDL_PollEvent(&event);

	//check if certain keys are pressed
	switch (event.type) {
		//if Exit, Alt+F4, or Win button is pressed
		case SDL_QUIT: 
			printf("Quit");
			return false;
			break;
		//if X is pressed
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				printf("Esc");
				return false;
			break;

	}
}

//Update objects every frame
void update() {
	if (ball.x >= windowWidth - ball.width) {
		//todo
	}
}

//Tell renderer to show objects on screen
void render() {
	//Draws background color
	SDL_SetRenderDrawColor(renderer, 245, 169, 184, 255);

	//Clears screen
	SDL_RenderClear(renderer);

	//Draw Rectangle
	SDL_Rect rectangle = { ball.x, ball.y, ball.width, ball.height };

	SDL_SetRenderDrawColor(renderer, 91, 206, 250, 255);
	SDL_RenderFillRect(renderer, &rectangle);

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
	//set isRunning variable to true
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