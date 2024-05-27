#include "Player.h"
#include "Constants.h"

Player::Player() {
    center[0] = 0;
    center[1] = 0;
    center[2] = 0;
    jumping_v = jump_v;
    size = 0;
    blink_counter = blink_count;

    slide_velocity = slide_v;
}

Player::Player(float x, float y, float z, int s) {
    center[0] = x;
    center[1] = y;
    center[2] = z;
    jumping_v = jump_v;
    size = s;
    blink_counter = blink_count;

    slide_velocity = slide_v;
}

void Player::set_Player(float x, float y, float z, int s) {
    center[0] = x;
    center[1] = y;
    center[2] = z;
    size = s;
}

void Player::draw() const {

    glBegin(GL_QUADS);
    glVertex3f(center[0] - size / 2, center[1] - size / 2, 0.0f);
    glVertex3f(center[0] - size / 2, center[1] + size / 2, 0.0f);
    glVertex3f(center[0] + size / 2, center[1] + size / 2, 0.0f);
    glVertex3f(center[0] + size / 2, center[1] - size / 2, 0.0f);
    glEnd();
}
void Player::setCenterX(float x)
{
    center[0] = x;
}
void Player::setCenterY(float y)
{
    center[1] = y;
}
void Player::setCenterZ(float z)
{
    center[2] = z;
}
float Player::getCenterX() {
    return center[0];
}
float Player::getCenterY() {
    return center[1];
}
float Player::getCenterZ() {
    return center[2];
}
int Player::getSize() {
    return size;
}

void Player::jump_move()
{
    center[1] += jumping_v;
    jumping_v += jump_a;
}
void Player::reset_v()
{
    jumping_v = jump_v;
}
float Player::getJumping_v()
{
    return jumping_v;
}

void Player::drop()
{
    center[1] += drop_v;
}


int Player::get_blink()
{
    return blink_counter;
}
void Player::blink()
{
    blink_counter -= 1;
    if (blink_counter < 0)
    {
        blink_counter = blink_count;
        dameged = false;
    }
}
void Player::setDameged(bool t)
{
    dameged = t;
}
bool Player::getDameged()
{
    return dameged;
}

void Player::blink_reset()
{
    dameged = false;
    blink_counter = blink_count;
}

void Player::slide_left()
{
    center[0] -= slide_velocity;
    slide_velocity += slide_a;
}
void Player::slide_right()
{
    center[0] += slide_velocity;
    slide_velocity += slide_a;
}


float Player::get_slide_v()
{
    return slide_velocity;
}
void Player::slide_v_reset()
{
    slide_velocity = slide_v;
}