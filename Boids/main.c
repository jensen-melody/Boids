#include <stdio.h>
#include <SDL.h>
#include "./constants.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int isRunning = false;

struct ball {
	float x;
	float y;
	float width;
	float height;
}ball;

int initializeWindow(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
			return false;
	}
	window = SDL_CreateWindow(
		"Gay Sex",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_ALLOW_HIGHDPI
	);
	if (!window) {
		fprintf(stderr, "Error creating SDL Window.\n");
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL Renderer.\n");
		return false;
	}

	return true;
}

void setup() {
	//TODO: Setup
	ball.x = 20;
	ball.y = 20;
	ball.width = 15;
	ball.height = 15;
}

int processInput() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT: 
			printf("Quit");
			return false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				printf("Esc");
				return false;
			break;

	}
}

void update() {
	//TODO: Update
}

void render() {
	SDL_SetRenderDrawColor(renderer, 245, 169, 184, 255);
	SDL_RenderClear(renderer);

	//Draw Rectangle
	SDL_Rect ball_rect = { ball.x, ball.y, ball.width, ball.height };

	SDL_SetRenderDrawColor(renderer, 91, 206, 250, 255);
	SDL_FillRect(renderer, &ball_rect);

	SDL_RenderPresent(renderer);
}

void destroyWindow() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	int isRunning = initializeWindow();

	setup();

	while (isRunning) {
		isRunning = processInput();
		update();
		render();
	}

	destroyWindow();

	return false;
}