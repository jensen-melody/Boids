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

SDL_Vertex vface = { {300,250}, {255,255,255,255}, {1,1} };
SDL_Vertex vlWing = { {250,300}, {255,255,255,255}, {1,1} };
SDL_Vertex vrWing = { {350,300}, {255,255,255,255}, {1,1} };
SDL_Vertex vtail = { {300,275}, {255,255,255,255}, {1,1} };

void calcVertecies(boid boid, float r) {
	//position variables
	vector2 center = boid.pos;

	vector2 face;
	face.x = center.x;
	face.y = center.y + (1 * boidSize); \

		vector2 lwing;
	lwing.x = center.x + (-1 * boidSize);
	lwing.y = center.y + (-1 * boidSize);

	vector2 rwing;
	rwing.x = center.x + (1 * boidSize);
	rwing.y = center.y + (-1 * boidSize);

	vector2 tail;
	tail.x = center.x;
	tail.y = center.y + (-0.5 * boidSize);

	//set new positions and color into the vertecies
	SDL_Vertex vface = { {face.x,face.y} , {boid.color.r, boid.color.g, boid.color.b, 255}, {1, 1} };
	printf("{{%f,%f} , {%i,%i,%i} , {1,1}}\n",face.x,face.y, boid.color.r, boid.color.g, boid.color.b);
	SDL_Vertex vlWing = { {lwing.x,lwing.y}, {boid.color.r,boid.color.g,boid.color.b, 255}, {1,1} };
	printf("{{%f,%f} , {%i,%i,%i} , {1,1}}\n", lwing.x, lwing.y, boid.color.r, boid.color.g, boid.color.b);
	SDL_Vertex vrWing = { {rwing.x,rwing.y }, { boid.color.r, boid.color.g, boid.color.b, 255 }, { 1, 1 } };
	printf("{{%f,%f} , {%i,%i,%i} , {1,1}}\n", rwing.x, rwing.y, boid.color.r, boid.color.g, boid.color.b);
	SDL_Vertex vtail = { {tail.x,tail.y }, { boid.color.r, boid.color.g, boid.color.b, 255 }, { 1, 1 } };
	printf("{{%f,%f} , {%i,%i,%i} , {1,1}}\n", tail.x, tail.y, boid.color.r, boid.color.g, boid.color.b);

	//set boid shape to SDL_Vertex variables
	/*boid.shape.face = vface;
	boid.shape.lWing = vlWing;
	boid.shape.rWing = vrWing;
	boid.shape.tail = vtail;*/
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
		/*boids[i].pos.x = rand() % (windowWidth);
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
		}*/
		boids[0].pos.x = windowWidth / 2;
		boids[0].pos.y = windowHeight / 2;

		boids[0].color.r = 255;
		boids[0].color.g = 255;
		boids[0].color.b = 255;
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

	/*
	//Do calculations for boid velocities
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
	}

	//Update boids pos
	for (int i = 0; i < numBoids; i++) {
		boids[i].x += boids[i].dx;
		boids[i].y += boids[i].dy;
	}
	*/
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
		calcVertecies(boids[i], 0);

		//Put boid shape vertecies into array (left)
		SDL_Vertex lVertices[] = {
			vface,
			vlWing,
			vtail,
		};

		//Put boid shape vertecies into array (right side)
		SDL_Vertex rVertices[] = {
			vface,
			vrWing,
			vrWing,
		};

		//Render boid
		SDL_RenderGeometry(renderer, NULL, lVertices, 3, NULL, 0);
		SDL_RenderGeometry(renderer, NULL, rVertices, 3, NULL, 0);

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