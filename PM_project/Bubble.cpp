#include"Bubble.h"
#include <GL/freeglut.h>

Bubble::Bubble() 
{
	radius = 0;
	slice = 0;
	stack = 0;
	get_monster = false;
	up = false;
	pop_out = false;
}
Bubble::Bubble(float r, int sl, int st) 
{
	radius = r;
	slice = sl;
	stack = st;
	get_monster = false;
	up = false;
	pop_out = false;
}
void Bubble::setCenter(float a, float b, float c) 
{
	center[0] = a;
	center[1] = b;
	center[2] = c;
}
Bubble::~Bubble() {}

void Bubble::setVelocity(float a, float b, float c) 
{
	velocity[0] = a;
	velocity[1] = b;
	velocity[2] = c;

} 
float Bubble::getCenterX() {
	return center[0];
}
float Bubble::getCenterY() {
	return center[1];
}
float Bubble::getCenterZ() {
	return center[2];
}
void Bubble::move() {
	setCenter(center[0] + velocity[0], center[1] + velocity[1], center[2] + velocity[2]);
}
float Bubble::getVelocityX() {
	return velocity[0];
}
float Bubble::getVelocityY() {
	return velocity[1];
}
float Bubble::getVelocityZ() {
	return velocity[2];
}
float Bubble::getRadius() {
	return radius;
}
void Bubble::setFirstcenter(){
	bubbles_first_center = center[0];
}
float Bubble::getFirstcenter() {
	return bubbles_first_center;
}
bool Bubble::get_get_monster() {
	return get_monster;
}
void Bubble::set_get_monster(bool m) {
	get_monster = m;
}
void Bubble::draw() const {
	float emission_[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float ambient_[] = { 0.1f, 0.1f, 0.0f, 1.0f };
	float diffuse_[] = { 0.5f, 0.5f, 0.0f, 1.0f };
	float specular_[] = { 0.5f, 0.5f, 0.0f, 1.0f };
	float shinininess_[] = { 32.0f };
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission_);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_);
	glMaterialfv(GL_FRONT, GL_SHININESS, shinininess_);

	glPushMatrix();
	glTranslatef(center[0], center[1], center[2]);
	glutSolidSphere(radius, slice, stack);
	glPopMatrix();
}


bool Bubble::get_up()
{
	return up;
}
void Bubble::set_up(bool u)
{
	up = u;
}
bool Bubble::get_pop_out()
{
	return pop_out;
}
void Bubble::set_pop_out(bool p)
{
	pop_out = p;
}
