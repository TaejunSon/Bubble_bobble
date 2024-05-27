#include "Monster.h"
#include "Constants.h"

Monster::Monster() {
	center[0] = 0;
	center[1] = 0;
	center[2] = 0;
	moving_v = moving;
	size = 0;
	now_moving = true;
	trap = false;
	trap_bubble_num = -1;
}
Monster::Monster(float x, float y, float z, int size) {
	center[0] = x;
	center[1] = y;
	center[2] = z;
	moving_v = moving;
	size = size;
	now_moving = true;
	trap = false;
	trap_bubble_num = -1;

}
void Monster::set_Monster(float x, float y, float z, int size) {
	center[0] = x;
	center[1] = y;
	center[2] = z;
	moving_v = moving;
	size = size;
}

void Monster::setCenterX(float x)
{
	center[0] = x;
}
void Monster::setCenterY(float y)
{
	center[1] = y;
}
void Monster::setCenterZ(float z)
{
	center[2] = z;
}
float Monster::getCenterX() {
	return center[0];
}
float Monster::getCenterY() {
	return center[1];
}
float Monster::getCenterZ() {
	return center[2];
}
int Monster::getSize() {
	return size;
}

void Monster::move()
{
	center[0] += moving_v;
}
void Monster::set_v(float v)
{
	moving_v = v;
}
float Monster::getmoving_v()
{
	return moving_v;
}
void Monster::set_trap_bubble_num(int n) {
	trap_bubble_num = n;
}
int Monster::get_trap_bubble_num() {
	return trap_bubble_num;
}
bool Monster::trun_left()
{
	if (moving_v < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Monster::get_move()
{
	return now_moving;
}
void Monster::set_move(bool move_) {
	now_moving = move_;
}
bool Monster::getTrap() {
	return trap;
}
void Monster::setTrap(bool tr) {
	trap = tr;
}