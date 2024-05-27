#include "Texture.h"
#include "Constants.h"
#include <iostream>
using namespace std;

Texture::Texture() {}
FIBITMAP* Texture::createBitMap(char const* filename) {
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
    if (format == -1) { // couldn't find image
        exit(-1);
    }
    else if (format == FIF_UNKNOWN) { // couldn't determine file format
        // attemp to get from file extension
        format = FreeImage_GetFIFFromFilename(filename);
        if (!FreeImage_FIFSupportsReading(format)) {
            //cout << "Detected image format cannot be read!" << endl;
            exit(-1);
        }
    }
    FIBITMAP* bitmap = FreeImage_Load(format, filename);
    int bitsPerPixel = FreeImage_GetBPP(bitmap);
    //cout << "Source image has " << bitsPerPixel << " bits per pixel." << endl;
    FIBITMAP* bitmap32;
    if (bitsPerPixel == 32) {
        //cout << "Skipping conversion." << endl;
        bitmap32 = bitmap;
    }
    else {
        //cout << "Converting to 32-bits." << endl
        bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    }
    return bitmap32;
}
void Texture::generateTexture() {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureData);

}
void Texture::initializeTexture(char const* filename) {
    FIBITMAP* bitmap32 = createBitMap(filename);
    imageWidth = FreeImage_GetWidth(bitmap32);
    imageHeight = FreeImage_GetHeight(bitmap32);
    textureData = FreeImage_GetBits(bitmap32);
    generateTexture();

}
void Texture::setposition(float x, float y, int width, int height)
{
    center[0] = x;
    center[1] = y;
    imageWidth = width;
    imageHeight = height;
}
void Texture::move(float x)
{
    center[0] = x;
}
void Texture::jump(float y)
{
    center[1] = y;
}

GLuint Texture::getTextureID() const {
    return textureID;
} //이러면 이거 필요한가?

void Texture::draw() const
{
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);  glVertex2f(center[0] - imageWidth / 2, center[1] - imageHeight / 2);
    glTexCoord2f(0, 1);  glVertex2f(center[0] - imageWidth / 2, center[1] + imageHeight / 2);
    glTexCoord2f(1, 1);  glVertex2f(center[0] + imageWidth / 2, center[1] + imageHeight / 2);
    glTexCoord2f(1, 0);  glVertex2f(center[0] + imageWidth / 2, center[1] - imageHeight / 2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Texture::draw(float x, float y) const
{
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);  glVertex2f(x - imageWidth / 2, y - imageHeight / 2);
    glTexCoord2f(0, 1);  glVertex2f(x - imageWidth / 2, y + imageHeight / 2);
    glTexCoord2f(1, 1);  glVertex2f(x + imageWidth / 2, y + imageHeight / 2);
    glTexCoord2f(1, 0);  glVertex2f(x + imageWidth / 2, y - imageHeight / 2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

float Texture::getCenterX() {
    return center[0];
}
float Texture::getCenterY() {
    return center[1];
}