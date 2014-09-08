#ifndef GRID_H_
#define GRID_H_

#include <GL/glut.h>

#include "Renderable.h"

/**
* This class represents a grid floor
*/
class Grid : public Renderable{
public:
	Grid(){};
	Grid(int s, float Y);
	~Grid(void);
	
	void setY(float Y);
	void draw();
	void update(){};
private:
	int size;
	float y;
};

#endif

