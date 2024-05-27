#include "map.h"

map::map(){};
void map::draw() const {
	glBegin(GL_QUADS);
	glVertex3f(center[0] - width / 2, center[1] - height/ 2, 0.0f);
	glVertex3f(center[0] - width / 2, center[1] + height / 2, 0.0f);
	glVertex3f(center[0] + width / 2, center[1] + height / 2, 0.0f);
	glVertex3f(center[0] + width / 2, center[1] - height / 2, 0.0f);
	glEnd();
}
void map::set_map(float center1, float center2, float w, float h) {
	center[0] = center1;
	center[1] = center2;
	width = w;
	height = h;
}
float map::getCenterX() {
	return center[0];
}
float map::getCenterY() {
	return center[1];
}
float map::getWidth()
{
	return width;
}
float map::getHeight() {
	return height;
}