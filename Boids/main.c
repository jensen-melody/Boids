//Boid algorithms developed by Craig Reynolds, adapted by V. Hunter Adams
//https://vanhunteradams.com/Pico/Animal_Movement/Boids-algorithm.html

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
int isDisco = false;
int bhMode = 1;
int lastFrameTime = 0;

int windowWidth = defaultWindowWidth;
int windowHeight = defaultWindowHeight;

color bgColor;
float h = 0;

boid boids[numBoids];

//Calculate two boids distances
float dist(boid boid1, boid boid2) {
	float dx = boid2.pos.x - boid1.pos.x;
	float dy = boid2.pos.y - boid1.pos.y;

	float distance = sqrt(pow(dx, 2) + pow(dy, 2));

	return distance;
}

//Calculate RGB value from HSV
color hsv(float H, float S, float V) {
	//Make define vars
	color out;
	float r = 0;
	float g = 0;
	float b = 0;

	//Set H, S, and V to their respective values
	H *= 24;
	H /= 17;
	S /= 255;
	V /= 255;
	
	//Set other vars
	float C = V * S;
	float X = (1 - fabs((fmod(H/60,2)-1)))*C;
	float M = V - C;

	//Calc the thingies
	if (H >= 0 && H < 60) {
		r = C;
		b = X;
		g = 0;
	}
	else if (H < 120) {
		r = X;
		b = C;
		g = 0;
	}
	else if (H < 180) {
		r = 0;
		b = C;
		g = X;
	}
	else if (H < 240) {
		r = 0;
		b = X;
		g = C;
	}
	else if (H < 300) {
		r = X;
		b = 0;
		g = C;
	}
	else if (H <= 360) {
		r = C;
		b = 0;
		g = X;
	}

	out.r = ((r + M) * 255);
	out.g = ((g + M) * 255);
	out.b = ((b + M) * 255);

	return out;
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

	//Get tail position
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
}

//Sets up the scene
void setup() {
	//Sets boid variables
	for (int i = 0; i < numBoids; i++) {
		//Random pos
		boids[i].pos.x = rand() % (windowWidth);
		boids[i].pos.y = rand() % (windowHeight);

		//Random velocity
		boids[i].vel.x = (((rand() % 60) + 5) / 10);
		if (rand() % 2 == 1) {
			boids[i].vel.x *= -1;
		}
		boids[i].vel.y = (((rand() % 60) + 5) / 10);
		
		//Color :D
		boids[i].color.r = 255;
		boids[i].color.g = 255;
		boids[i].color.b = 255;

		//Give computer some time to get a new seed for random velocity for y
		if (rand() % 2 == 0) {
			boids[i].vel.y *= -1;
		}
	}

	//Sets background color to black
	bgColor.r = 0;
	bgColor.g = 0;
	bgColor.b = 0;
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
			//If key pressed is C
			if (event.key.keysym.sym == SDLK_c) {
				for (int i = 0; i < numBoids; i++) {
					boids[i].color.r = rand() % 255;
					boids[i].color.b = rand() % 255;
					boids[i].color.g = rand() % 255;
				}
			}
			//If key pressed is D
			if (event.key.keysym.sym == SDLK_d) {
				if (isDisco) {
					isDisco = false;
				}
				else {
					isDisco = true;
				}
			}
			//If key pressed is B
			if (event.key.keysym.sym == SDLK_b) {
				bhMode *= -1;
			}
			break;

	}
}

//Update objects every frame
void update() {
	//Wait an amount of time until target FPS reached
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), lastFrameTime + targetFrameTime));
	lastFrameTime = SDL_GetTicks();

	//Variables for boid behavior
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
		boids[i].vel.x += closeBoids.x * bhMode * avoidFactor;
		boids[i].vel.y += closeBoids.y * bhMode * avoidFactor;
		boids[i].vel.x += (avgVel.x - boids[i].vel.x) * alignFactor;
		boids[i].vel.y += (avgVel.y - boids[i].vel.y) * alignFactor;
		boids[i].vel.x += (avgPos.x - boids[i].pos.x) * centeringFactor;
		boids[i].vel.y += (avgPos.y - boids[i].pos.y) * centeringFactor;
	}

	//Edging
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

	//Do disco mode colors
	if (isDisco) {
		//Update boid color for disco
		for (int i = 0; i < numBoids; i++) {
			boids[i].color = hsv(h, 255, 255);
		}
		bgColor = hsv(-h+255, 200, 150);
		h += 60 / FPS;
		if (h >= 255) {
			h = 0;
		}
	}
}

//Tell renderer to show objects on screen
void render() {
	//Draw background
	SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, 0);

	//Clear Screen
	SDL_RenderClear(renderer);

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

	//Set isRunning variable to if window initizalized
	int isRunning = initializeWindow();

	//Print Controls to console
	printf("---CONTROLS---\nPress ESC to Quit\nPress R to Reset Simulation\nPress C to Change Colors\n");

	//Set up scene
	setup();

	//Main Game Loop
	while (isRunning) {
		isRunning = processInput();
		update();
		render();
	}

	//If isRunning is false, or loop breaks suddenly, destroy the window and uninitialize everything
	destroyWindow();
}

//Thank you for an amazing year of CompSci Ms. Kant!!!!