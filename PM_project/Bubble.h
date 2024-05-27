#pragma once

#include <GL/freeglut.h>


class Bubble {
public:
	Bubble();
	Bubble(float r, int sl, int st);
	~Bubble();
	void setCenter(float a, float b, float c);
	void setVelocity(float a, float b, float c);

	float getCenterX();
	float getCenterY();
	float getCenterZ();

	float getVelocityX();
	float getVelocityY();
	float getVelocityZ();
	void setFirstcenter();
	float getFirstcenter();
	float getRadius();
	void move();
	void draw() const;
	bool get_get_monster();
	void set_get_monster(bool m);

	bool get_up();
	void set_up(bool u);
	bool get_pop_out();
	void set_pop_out(bool p);


private:
	float radius;
	int slice;
	int stack;

	float center[3];
	float velocity[3];
	float bubbles_first_center;
	bool get_monster;       //���� ��Ҵ��� ���� ����

	bool up; // ���� �ö󰡴� ������ ����
	bool pop_out; //���� �������� �ƴ��� ����

};
