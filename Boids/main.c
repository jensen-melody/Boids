#include <stdio.h>
#include <SDL.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "./constants.h"

//Global variables used for things :3
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int isRunning = false;
int lastFrameTime = 0;

boid boids[numBoids];

float r = 0;

void calcVertecies(boid boid[], int i) {
	//get center variables
	vector2 center = boid[i].pos;

	//Get r from velocity
	float r = atan(boid[i].vel.y/boid[i].vel.x);

	vector2 head;
	head.x = boidSize * sin(r) + center.x;
	head.y = boidSize * cos(r) + center.y;

	vector2 lWing;
	lWing.x = -boidSize * cos(r) - boidSize * sin(r) + center.x;
	lWing.y = boidSize * sin(r) - boidSize * cos(r) + center.y;

	vector2 rWing;
	rWing.x = boidSize * cos(r) - boidSize * sin(r) + center.x;
	rWing.y = -boidSize * sin(r) - boidSize * cos(r) + center.y;

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
	for (int i = 0; i < numBoids; i++) {
		boids[i].pos.x = rand() % (windowWidth);
		boids[i].pos.y = rand() % (windowHeight);
		boids[i].vel.x = (((rand() % 40) + 20) / 10);
		if (rand() % 2 == 1) {
			boids[i].vel.x *= -1;
		}
		boids[i].vel.y = ((rand() % 40) + 20) / 10;
		boids[i].color.r = 255;
		boids[i].color.g = 255;
		boids[i].color.b = 255;
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

	//check if beyond wall
	for (int i = 0; i < numBoids; i++) {
		//if beyond horizontal wall
		if (boids[i].pos.x <= 0 - sqrt(pow(boidSize, 2) * 2) || boids[i].pos.x >= windowWidth + sqrt(pow(boidSize, 2) * 2)) {
			boids[i].pos.x += -windowWidth;
		}

		//if beyond vertical wall
		if (boids[i].pos.y <= 0 - sqrt(pow(boidSize, 2) * 2) || boids[i].pos.y >= windowHeight + sqrt(pow(boidSize, 2) * 2)) {
			boids[i].pos.y += -windowHeight;
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

	//Loop through every boid
	for (int i = 0; i < numBoids; i++) {
		//Calculate boid shape vertecies from position and rotation
		calcVertecies(boids, i, r);

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

	//Get fullscreen size
	//SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

	//calcVertecies(boids[0], 0);

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