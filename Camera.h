#pragma once
#define _USE_MATH_DEFINES
#include <gl/glut.h>
#include <math.h>

class Camera {
public:
	GLdouble x, y, z, distance, degree;
	Camera() {
		distance = 1;
		x = distance;
		y = 0;
		z = 0;
		degree = 0;
	}

	void turnLeft(GLdouble value) {
		degree -= (M_PI / 180) * value;
		x = distance * cos(degree);
		z = distance * sin(degree);
	}

	void turnRight(GLdouble value) {
		degree += (M_PI / 180) * value;
		x = distance * cos(degree);
		z = distance * sin(degree);
	}

	void changeDistance(GLdouble value) {
		GLdouble newDistance = distance + value;
		x = (x / distance) * newDistance;
		z = (z / distance) * newDistance;
		distance = newDistance;
	}

};