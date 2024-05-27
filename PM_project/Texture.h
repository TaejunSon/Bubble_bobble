#pragma once
#include <GL/freeglut.h>
#include <FreeImage.h>
class Texture {
public:
	Texture();
	FIBITMAP* createBitMap(char const* filename);
	void generateTexture();
	void initializeTexture(char const* filename);
	void setposition(float x, float y, int width, int height);
	float getCenterX();
	float getCenterY();
	void move(float x);
	void jump(float y);
	GLuint getTextureID() const;
	void draw() const;

	void draw(float x, float y) const;


private:
	GLuint textureID;
	GLubyte* textureData;
	int imageWidth, imageHeight;
	float center[2];

	static int blink_counter;
};