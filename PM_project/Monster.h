#pragma once
#include <GL/freeglut.h>

class Monster
{
public:
	Monster();
	Monster(float x, float y, float z, int size);
	void set_Monster(float x, float y, float z, int size);
	void setCenterX(float x);
	void setCenterY(float y);
	void setCenterZ(float z);
	float getCenterX();
	float getCenterY();
	float getCenterZ();	
	int getSize();

	void move();
	void set_v(float v);
	float getmoving_v();
	void set_move(bool move_);
	void set_trap_bubble_num(int n);
	int get_trap_bubble_num();
	bool trun_left();
	bool get_move();
	bool getTrap();
	void setTrap(bool tr);

private:
	float center[3];
	float moving_v;
	int size;
	int trap_bubble_num;   //�� �� ǳ���� ���͸� ��Ҵ��� ����
	bool now_moving;
	bool trap;

};