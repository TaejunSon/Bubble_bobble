#pragma once
#include<vector>
#include <GL/freeglut.h>
class map {
public:
	map();
	void draw() const;
	void set_map(float center1, float center2, float w, float h);
	float getCenterX();
	float getCenterY();
	float getWidth();
	float getHeight();

private:	
	float center[2];
	float width, height;
};