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
	bool get_monster;       //몬스터 잡았는지 여부 판정

	bool up; // 위로 올라가는 중인지 판정
	bool pop_out; //터진 상태인지 아닌지 판정

};
