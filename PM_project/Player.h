#pragma once
#include <GL/freeglut.h>
class Player
{
public:
	Player();
	Player(float x, float y, float z, int size);
	void set_Player(float x, float y, float z, int size);
	void setCenterX(float x);
	void setCenterY(float y);
	void setCenterZ(float z);
	float getCenterX();
	float getCenterY();
	float getCenterZ();	
	int getSize();
	void draw() const;

	void jump_move();
	void reset_v();
	void drop();
	float getJumping_v();

	int get_blink();
	void blink();
	void setDameged(bool t);
	bool getDameged();	

	void blink_reset();

	void slide_left();
	void slide_right();
	float get_slide_v();
	void slide_v_reset();

private:
	float center[3];
	float jumping_v;
	int size;	

	bool dameged = false; // 데미지 여부 판정
	int blink_counter;

	float slide_velocity = 0;
};

