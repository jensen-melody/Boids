#define false 0
#define true 1

#define defaultWindowWidth 1920
#define defaultWindowHeight 1080

#define FPS 30
#define targetFrameTime (1000/FPS)

#define numBoids 2000

#define boidSize 8

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
	vector2 head;
	vector2 lWing;
	vector2 rWing;
	vector2 tail;
}shape;

//Boids Structure
typedef struct {
	vector2 pos;
	vector2 vel;
	shape shape;
	color color;
}boid;