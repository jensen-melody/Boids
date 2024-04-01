#define false 0
#define true 1

#define windowWidth 960
#define windowHeight 540

#define FPS 0.5
#define targetFrameTime (1000/FPS)

#define numBoids 1000

#define boidSize 4

//structure that holds color data
typedef struct {
	int r;
	int g;
	int b;
}color;

//structure that holds vector2 data
typedef struct {
	float x;
	float y;
}vector2;

//structure that holds boid vertex data
typedef struct {
	SDL_Vertex face;
	SDL_Vertex lWing;
	SDL_Vertex rWing;
	SDL_Vertex tail;
}shape;

typedef struct {
	vector2 pos;
	vector2 vel;
	shape shape;
	color color;
}boid;