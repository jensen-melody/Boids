//Constant Defining!!!
#define false 0
#define true 1

#define defaultWindowWidth 1920
#define defaultWindowHeight 1080

#define FPS 30
#define targetFrameTime (1000/FPS)

#define numBoids 200
#define boidSize 8

#define turnFactor 0.3
#define margin 100
#define visualRange 120
#define	protectedRange 24
#define	centeringFactor 0.0005
#define	avoidFactor 0.05
#define	alignFactor 0.05
#define	maxSpeed 6
#define	minSpeed 3
#define	maxBias 0.01
#define	biasIncrement 0.00004
#define	biasVal 0.001

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