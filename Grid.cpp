#include "Grid.h"

Grid::Grid(int s, float Y){
	size = s;
	y = Y;
}


Grid::~Grid(void){}

void Grid::setY(float Y){
	y = Y;
}

void Grid::draw(){
	glPushMatrix();

	glColor3f(1, 1, 1);
	for (int i = -size; i <= size; i += 2) {
		for (int j = -size; j <= size; j += 2) {
			glBegin(GL_QUADS);
				glVertex3f(i, y, j);
                glVertex3f(i, y, j - 1);
                glVertex3f(i - 1, y, j - 1);
                glVertex3f(i - 1, y, j);
			glEnd();
         }
	}

	glPopMatrix();
}
