#include <iostream>
#include <vector>
#include <fstream>
#include <string>  
#include <sstream> 
#include <algorithm>

#include "Constants.h"
#include "Bubble.h"
#include "Light.h"
#include "Texture.h"
#include "map.h"
#include "Player.h"
#include "Monster.h"


#include <windows.h>
#pragma comment (lib, "winmm.lib")
#include <mmsystem.h>;
// 음악 재생
int menu;
int menu2;

enum {
    CLEAR = 0, AUDIO0, AUDIO1, AUDIO2, NOAUDIO, THEME1, THEME2, THEME3, NOTHEME, QUIT = 999
};

using namespace std;

enum class state { Start, command, stage1, stage2, stage3, stage4, stage5, stage6, ScoreBoard, clear, Game_over };
state State = state::Start; // 초기 State는 Start, stage가 바뀔 때마다 변화
clock_t start_t = clock();
clock_t end_t;
clock_t start_time;

clock_t blink_t = clock();


vector<Bubble> bubbles; //bubbles vector
Light light(boundaryX, boundaryY, boundaryX / 2, GL_LIGHT0);
Texture texture;    // 첫 시작 배경화면

Player player(-200, -260, 0, PLAYER_SIZE);
vector<map> maps; // map을 만들 사각형들의 vector
vector<Monster> monsters;

vector<int> time_for_bubbles;

Texture map_image;  // map을 채우는 stage1,2 벽돌 이미지 조각들
Texture map_image2; // map을 채우는 stage3,4 벽돌 이미지 조각들
Texture map_image3; // map을 채우는 stage5,6 벽돌 이미지 조각들
Texture map_image_rock;

Texture playerimage_left;   //캐릭터 이미지 left
Texture playerimage_right;   //캐릭터 이미지 right
Texture monster_image[8][2];   // 몬스터 이미지
Texture life_image[3];              // 생명력 이미지
Texture theme1;
Texture theme2;
Texture theme3;
Texture command; // 입력 커맨드
Texture clear; // Clear 화면
Texture Gameover; //종료 화면

string input_char; // 치트키 입력
bool input_start = false; // 치트키 입력 판정
bool time_cheat = false; //시간 치트기 입력

long int score = 0;    //점수
int life = 3;          // 생명력
float buble_range = 300.0f;     // 버블 사정거리
float buble_size = 20.0f;

bool trun_left = false;   //방향전환 여부 판정
bool left_move = false;   // 좌로 움직이기
bool right_move = false;  // 우로 움직이기
bool jump_move = false;   // 점프 여부 판정
bool slide = false; // stage 5,6 미끄러짐 여부 판정

bool player_drop = false;  // 낙하 여부 판정

bool game_over = false;   // '라이프 = 0' (게임 오버) 판정

bool stage_clear[8] = {false,false,false,false,false,false,false,false};


void stage_setting();
void keyboard_start(unsigned char key, int x, int y);
void special_keyboard_start(int key, int x, int y);

void keyboard_input(unsigned char key, int x, int y);
void keyboard_input_close(unsigned char key, int x, int y);
void keyboard_restart(unsigned char key, int x, int y) {}
void keyboard_restart(int key, int x, int y) {}

void Player_Drop();
void monster_move();
void bubbles_chain(int num);

int bubble_num = 0;
vector<double> scoreBox;


void readFile() {
    ifstream ifs1("ScoreBoard.txt");
    float val;
    while (ifs1 >> val, !ifs1.eof())
        scoreBox.push_back(val);
    ifs1.close();
}
void writeFile(vector<double> vec)
{
    ofstream ofs("ScoreBoard.txt");
    for (int i = 0; i < vec.size(); i++)
        ofs << vec[i] << " ";

    ofs.close();
}
void initialize() {
    input_char.append("Enter : ");

    light.setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
    light.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
    light.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);

    /* Implement: initialize texture*/
    //시작 화면 이미지
    texture.initializeTexture("BubbleBobble.png");
    texture.setposition(0.0f, 0.0f, boundaryX, boundaryY);
    command.initializeTexture("command.png");
    command.setposition(0.0f, 0.0f, boundaryX, boundaryY);


    //캐릭터 이미지
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    playerimage_right.initializeTexture("player_right.png");
    playerimage_right.setposition(player.getCenterX(), player.getCenterY(), PLAYER_SIZE, PLAYER_SIZE);

    playerimage_left.initializeTexture("player_left.png");
    playerimage_left.setposition(player.getCenterX(), player.getCenterY(), PLAYER_SIZE, PLAYER_SIZE);


    //몬스터 이미지
    for (int i = 0; i < 8; i++)
    {
        monster_image[i][0].initializeTexture("monster1._left.png");
        monster_image[i][1].initializeTexture("monster1._right.png");
    } 


    //맵 이미지
    map_image.initializeTexture("block.jpg");
    map_image.setposition(0.0f, 0.0f, BLOCK, BLOCK);
    map_image2.initializeTexture("block2.jpg");
    map_image2.setposition(0.0f, 0.0f, BLOCK, BLOCK);
    map_image3.initializeTexture("block_ice.jpg");
    map_image3.setposition(0.0f, 0.0f, BLOCK, BLOCK);
    map_image_rock.initializeTexture("block_rock.png");
    map_image_rock.setposition(0.0f, 0.0f, BLOCK, BLOCK);


    //생명력 이미지
    for (int i = 0; i < 3; i++)
    {
        life_image[i].initializeTexture("green_heart.png");
        life_image[i].setposition(-290 + i * (10 + Life_size), -300, Life_size, Life_size);
    }

    theme1.initializeTexture("background1.jpg");
    theme2.initializeTexture("background2.jpg");
    theme3.initializeTexture("background3.jpg");
    Gameover.initializeTexture("gameover.png");
    Gameover.setposition(0.0f, 0.0f, boundaryX, boundaryY);
    clear.initializeTexture("Clear.png");
    clear.setposition(0.0f, 0.0f, boundaryX, boundaryY);
    readFile();
    start_time = clock();

}

void idle() {
    /* Implement: update spheres and handle collision at boundary*/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    end_t = clock();

    if (end_t - start_t > 1000 / 60 && !game_over)
    {
        if (menu2 == AUDIO0)
        {
            PlaySound(TEXT("Bouble Main-Theme.wav"), NULL, SND_ASYNC | SND_LOOP);
            menu2 = CLEAR;
        }
        if (menu2 == AUDIO1)
        {
            PlaySound(TEXT("AUDIO1.wav"), NULL, SND_ASYNC | SND_LOOP);
            menu2 = CLEAR;
        }
        if (menu2 == AUDIO2)
        {
            PlaySound(TEXT("AUDIO2.wav"), NULL, SND_ASYNC | SND_LOOP);
            menu2 = CLEAR;
        }
        if (menu2 == NOAUDIO)
        {
            PlaySound(NULL, 0, 0);
            menu2 = CLEAR;
        }

        start_t = end_t;

        monster_move();


        if (State == state::stage1 || State == state::stage2 || State == state::stage3
            || State == state::stage4 || State == state::stage5 || State == state::stage6)
        {

            for (int i = 0; i < monsters.size(); i++)
            {
                if (player.getCenterX() - monsters[i].getCenterX() < player.getSize() / 2 + monsters[i].getSize()
                    && player.getCenterX() - monsters[i].getCenterX() > -(player.getSize() / 2 + monsters[i].getSize()))
                {
                    if (player.getCenterY() - monsters[i].getCenterY() < player.getSize() / 2 + monsters[i].getSize()
                        && player.getCenterY() - monsters[i].getCenterY() > -(player.getSize() / 2 + monsters[i].getSize()))
                    {
                        if (!player.getDameged() && !game_over)
                        {


                            life -= 1;
                            player.setDameged(true);

                        }
                    }
                }
            }// 충돌 여부 판정     

            Player_Drop();

            if (jump_move)
            {
                if (player.getJumping_v() >= 0)   //jump 후 상승
                {
                    player.jump_move();
                    playerimage_right.jump(player.getCenterY());
                    playerimage_left.jump(player.getCenterY());
                }
                else    //jump 후 하강
                {
                    for (int i = 0; i < maps.size(); i++)
                    {
                        //바로 붙이기
                        if (player.getCenterY() - player.getSize() / 2 < maps[i].getCenterY() + maps[i].getHeight() / 2 - player.getJumping_v()
                            && player.getCenterY() - player.getSize() / 2 >= maps[i].getCenterY() + maps[i].getHeight() / 2)
                        {
                            //block 영역 내에 있을 때
                            if (player.getCenterX() - player.getSize() / 2 < maps[i].getCenterX() + maps[i].getWidth() / 2
                                && player.getCenterX() + player.getSize() / 2 > maps[i].getCenterX() - maps[i].getWidth() / 2)
                            {
                                player.setCenterY(maps[i].getCenterY() + maps[i].getHeight() / 2 + player.getSize() / 2);
                                playerimage_right.jump(player.getCenterY());
                                playerimage_left.jump(player.getCenterY());

                                player.reset_v();
                                jump_move = false;

                                player.slide_v_reset();
                                slide = true;
                                break;
                            }
                        }
                    }
                    if (jump_move)
                    {
                        player.jump_move();
                        playerimage_right.jump(player.getCenterY());
                        playerimage_left.jump(player.getCenterY());
                    }
                }
            }

            if (State == state::stage5 || State == state::stage6)
            {
                if (slide && !left_move && !right_move)
                {
                    if (player.get_slide_v() > 0)
                    {
                        if (trun_left)
                        {
                            if (player.getCenterX() - player.getSize() / 2 >= maps[1].getCenterX() + maps[1].getWidth() / 2 + player.get_slide_v())
                            {
                                player.slide_left();
                                playerimage_right.move(player.getCenterX());
                                playerimage_left.move(player.getCenterX());
                            }
                            else if (player.getCenterX() - player.getSize() / 2 < maps[1].getCenterX() + maps[1].getWidth() / 2 + player.get_slide_v())
                            {
                                player.setCenterX(maps[1].getCenterX() + maps[1].getWidth() / 2 + player.getSize() / 2);
                                playerimage_right.move(player.getCenterX());
                                playerimage_left.move(player.getCenterX());

                                player.slide_v_reset();
                                left_move = false;
                                right_move = false;
                                slide = false;
                            }
                        }
                        else
                        {
                            if (player.getCenterX() + player.getSize() / 2 <= maps[2].getCenterX() - maps[2].getWidth() / 2 - player.get_slide_v())
                            {
                                player.slide_right();
                                playerimage_right.move(player.getCenterX());
                                playerimage_left.move(player.getCenterX());
                            }
                            else if (player.getCenterX() + player.getSize() / 2 > maps[2].getCenterX() - maps[1].getWidth() / 2 - player.get_slide_v())
                            {
                                player.setCenterX(maps[2].getCenterX() - maps[2].getWidth() / 2 - player.getSize() / 2);
                                playerimage_right.move(player.getCenterX());
                                playerimage_left.move(player.getCenterX());

                                player.slide_v_reset();
                                left_move = false;
                                right_move = false;
                                slide = false;
                            }
                        }
                    }
                    else
                    {
                        slide = false;
                        player.slide_v_reset();
                    }
                }
            }//추가



            if (left_move)
            {
                if (player.getCenterX() - player.getSize() / 2 >= maps[1].getCenterX() + maps[1].getWidth() / 2 + moving)
                {
                    player.setCenterX(player.getCenterX() - moving);
                    playerimage_right.move(player.getCenterX());
                    playerimage_left.move(player.getCenterX());
                }
                else if (player.getCenterX() - player.getSize() / 2 < maps[1].getCenterX() + maps[1].getWidth() / 2 + moving)
                {
                    player.setCenterX(maps[1].getCenterX() + maps[1].getWidth() / 2 + player.getSize()/2);
                    playerimage_right.move(player.getCenterX());
                    playerimage_left.move(player.getCenterX());

                    slide = false;
                    player.slide_v_reset();
                }
            }
            if (right_move)
            {
                if (player.getCenterX() + player.getSize() / 2 <= maps[2].getCenterX() - maps[2].getWidth() / 2 - moving)
                {
                    player.setCenterX(player.getCenterX() + moving);
                    playerimage_right.move(player.getCenterX());
                    playerimage_left.move(player.getCenterX());
                }
                else if (player.getCenterX() + player.getSize() / 2 > maps[2].getCenterX() - maps[1].getWidth() / 2 - moving)
                {
                    player.setCenterX(maps[2].getCenterX() - maps[2].getWidth() / 2 - player.getSize() / 2);
                    playerimage_right.move(player.getCenterX());
                    playerimage_left.move(player.getCenterX());

                    slide = false;
                    player.slide_v_reset();
                }
            }

            // 세로 벽 구조물 충돌
            if (State == state::stage4 || State == state::stage5 || State == state::stage6)
            {                
                if (player.getCenterX() - player.getSize() / 2 < maps[7].getCenterX() + maps[7].getWidth() / 2
                    && player.getCenterX() + player.getSize() / 2 > maps[7].getCenterX() - maps[7].getWidth() / 2)
                {
                    //수정
                    if (player.getCenterY() - player.getSize() / 2 < maps[7].getCenterY() + maps[7].getHeight() / 2
                        && player.getCenterY() + player.getSize() / 2 > maps[7].getCenterY() - maps[7].getHeight() / 2)
                    {
                        if (player.getCenterY() + player.getSize() / 2 < maps[7].getCenterY() - maps[7].getHeight() / 2 + jump_v)
                        {
                            player.setCenterY(maps[7].getCenterY() - maps[7].getHeight() / 2 - player.getSize() / 2);
                            playerimage_right.jump(player.getCenterY());
                            playerimage_left.jump(player.getCenterY());
                        }
                        else
                        {
                            if (trun_left)
                            {
                                player.setCenterX(maps[7].getCenterX() + maps[7].getWidth() / 2 + player.getSize() / 2);
                            }
                            else
                            {
                                player.setCenterX(maps[7].getCenterX() - maps[7].getWidth() / 2 - player.getSize() / 2);
                            }
                            playerimage_right.move(player.getCenterX());
                            playerimage_left.move(player.getCenterX());
                        }                        
                    }
                }

            }


            // 풍선 : move
            for (int i = 0; i < bubbles.size(); i++)
            {
                if (!bubbles[i].get_pop_out())
                {
                    bubbles[i].move();

                    if (!bubbles[i].get_up())
                    {
                        if (bubbles[i].getCenterX() - bubbles[i].getRadius() <= maps[1].getCenterX() + maps[1].getWidth() / 2)
                        {
                            bubbles[i].setVelocity(0.0f, 3.0f, 0.0f);
                            bubbles[i].setCenter
                            (maps[1].getCenterX() + maps[1].getWidth() / 2 + bubbles[i].getRadius(), bubbles[i].getCenterY(), bubbles[i].getCenterZ());
                            bubbles[i].set_up(true);
                        }
                        else if (bubbles[i].getCenterX() + bubbles[i].getRadius() >= maps[2].getCenterX() - maps[2].getWidth() / 2)
                        {
                            bubbles[i].setVelocity(0.0f, 3.0f, 0.0f);
                            bubbles[i].setCenter
                            (maps[2].getCenterX() - maps[2].getWidth() / 2 - bubbles[i].getRadius(), bubbles[i].getCenterY(), bubbles[i].getCenterZ());
                            bubbles[i].set_up(true);
                        }
                        //양 사이드 충돌                        
                        if (State == state::stage4 || State == state::stage5 || State == state::stage6)
                        {
                            if (bubbles[i].getCenterX() - bubbles[i].getRadius() < maps[7].getCenterX() + maps[7].getWidth() / 2
                                && bubbles[i].getCenterX() + bubbles[i].getRadius() > maps[7].getCenterX() - maps[7].getWidth() / 2)
                            {
                                //수정
                                if (bubbles[i].getCenterY() - bubbles[i].getRadius() < maps[7].getCenterY() + maps[7].getHeight() / 2
                                    && bubbles[i].getCenterY() + bubbles[i].getRadius() > maps[7].getCenterY() - maps[7].getHeight() / 2)
                                {         
                                    if (bubbles[i].getVelocityX() > 0 )
                                    {
                                        bubbles[i].setCenter
                                        (maps[7].getCenterX() - maps[7].getWidth() / 2 - bubbles[i].getRadius(), bubbles[i].getCenterY(), bubbles[i].getCenterZ());
                                    }
                                    else
                                    {
                                        bubbles[i].setCenter
                                        (maps[7].getCenterX() + maps[7].getWidth() / 2 + bubbles[i].getRadius(), bubbles[i].getCenterY(), bubbles[i].getCenterZ());
                                    }
                                    bubbles[i].setVelocity(0.0f, 1.5f, 0.0f);                                    
                                    bubbles[i].set_up(true);
                                }
                            }                            
                        }//세로 벽 구조물 충돌


                        if (abs(bubbles[i].getCenterX() - bubbles[i].getFirstcenter()) > buble_range)
                        {
                            bubbles[i].setVelocity(0.0f, 3.0f, 0.0f);
                            bubbles[i].set_up(true);
                        }
                        // bubble이 벽면에 충돌하지 않고 300만큼 움직였을 때 위로 뜨는 것                   
                    }
                    else
                    {
                        if (bubbles[i].getCenterY() > boundaryY - 30)
                        {
                            bubbles[i].setVelocity(0.0f, 0.0f, 0.0f);
                            bubbles[i].setCenter(bubbles[i].getCenterX(), boundaryY - 30, bubbles[i].getCenterZ());
                        }
                        // bubble이 스크린 밖으로 나가지 않도록 지정. 
                    }
                }
            }


            for (int i = 0; i < bubbles.size(); i++)
            {
                for (int j = 0; j < monsters.size(); j++)
                {
                    if ((!bubbles[i].get_pop_out()) && (!bubbles[i].get_up()) && (bubbles[i].get_get_monster() == false))
                        // 풍선이 터지지 않았을 때
                        // 풍선이 위로 올라가는 중이 아닐 때 + 풍선이 아무것도 잡지 않았을 때
                        //  -> bubble & monster 충돌 판정
                    {
                        if (bubbles[i].getCenterX() - monsters[j].getCenterX() < bubbles[i].getRadius() + monsters[j].getSize() / 2
                            && bubbles[i].getCenterX() - monsters[j].getCenterX() > -(bubbles[i].getRadius() + monsters[j].getSize() / 2))
                        {
                            if (bubbles[i].getCenterY() - monsters[j].getCenterY() < bubbles[i].getRadius() + monsters[j].getSize() / 2
                                && bubbles[i].getCenterY() - monsters[j].getCenterY() > -(bubbles[i].getRadius() + monsters[j].getSize() / 2))
                            {
                                monsters[j].setCenterX(boundaryX + 100);  //몬스터 날려버리기
                                monsters[j].set_trap_bubble_num(i);
                                monsters[j].setTrap(true);

                                bubbles[i].setVelocity(0.0f, 1.0f, 0.0f);
                                bubbles[i].set_get_monster(true);
                                bubbles[i].set_up(true);

                                monster_image[j][0].setposition(bubbles[i].getCenterX(), bubbles[i].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                                monster_image[j][1].setposition(bubbles[i].getCenterX(), bubbles[i].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);

                            }
                        }

                    }
                }
            }// bubble & monster 충돌 판정


            for (int i = 0; i < bubbles.size(); i++)
            {
                if (bubbles[i].get_up() && !bubbles[i].get_pop_out())
                    // 공중으로 떠오르는 중이거나 맨 위에 모여있을 때 + 아직 터지지 않았을 때
                {
                    if (player.getCenterX() - bubbles[i].getCenterX() < player.getSize() / 2 + bubbles[i].getRadius()
                        && player.getCenterX() - bubbles[i].getCenterX() > -(player.getSize() / 2 + bubbles[i].getRadius()))
                    {
                        if (player.getCenterY() - bubbles[i].getCenterY() < player.getSize() / 2 + bubbles[i].getRadius()
                            && player.getCenterY() - bubbles[i].getCenterY() > -(player.getSize() / 2 + bubbles[i].getRadius()))
                            // 풍선과 플레이어 충돌 여부 판정

                        {

                            bubbles[i].set_pop_out(true);
                            for (int j = 0; j < monsters.size(); j++)
                            {
                                if (monsters[j].get_trap_bubble_num() == i)
                                {
                                    monster_image[j][0].setposition(boundaryX + 100, boundaryY + 100, BLOCK, BLOCK);
                                    monster_image[j][1].setposition(boundaryX + 100, boundaryY + 100, BLOCK, BLOCK);
                                    //몬스터 이미지 날려버리기
                                    monsters[j].setTrap(false);

                                    stage_clear[j] = true;
                                    cout << "monster " << j << " is clear" << endl;
                                }
                            }

                            bubbles_chain(i);
                        }

                    }
                }
            }


            for (int i = 0; i < monsters.size(); i++)
            {
                if (!stage_clear[i])
                {
                    break;
                }
                else if(i == monsters.size()-1)
                {
                    if (State == state::stage1)
                    {
                        cout << "stage 1 is clear" << endl << endl;

                        State = state::stage2;
                        stage_setting();
                    }
                    else if (State == state::stage2)
                    {
                        cout << "stage 2 is clear" << endl << endl;

                        State = state::stage3;
                        stage_setting();
                    }
                    else if (State == state::stage3)  
                    {
                        cout << "stage 3 is clear" << endl << endl;

                        State = state::stage4;
                        stage_setting();
                    }
                    else if (State == state::stage4)
                    {
                        cout << "stage 4 is clear" << endl << endl;

                        State = state::stage5;
                        stage_setting();
                    }
                    else if (State == state::stage5)
                    {
                        cout << "stage 5 is clear" << endl << endl;

                        State = state::stage6;
                        stage_setting();
                    }
                    else if (State == state::stage6)
                    {
                        cout << "stage 6 is clear" << endl << endl;
                        State = state::ScoreBoard;

                        double Play_time;
                        if (!time_cheat)
                        {
                            Play_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
                        }
                        else
                        {
                            Play_time = 0;
                        }

                        scoreBox.push_back(Play_time);
                        scoreBox.push_back(bubble_num);
                        writeFile(scoreBox);
                        glutKeyboardUpFunc(keyboard_start);
                        glutSpecialUpFunc(special_keyboard_start);
                        glutKeyboardFunc(keyboard_restart);

                    }

                }
            }    
        }

    }


    if (end_t - blink_t > 1000 / 10)
    {
        blink_t = end_t;
        if (player.getDameged())
        {
            player.blink();
        }
        else
        {
            if (game_over)
            {
                State = state::Game_over;
                glutKeyboardUpFunc(keyboard_start);
                glutSpecialUpFunc(special_keyboard_start);
                glutKeyboardFunc(keyboard_restart);

            }//gameover조건
        }
    }// 깜빡이기
    glutPostRedisplay();
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-boundaryX, boundaryX, -boundaryY, boundaryY, -100.0, 100.0);
    //gluOrtho2D(-boundaryX, boundaryX, -boundaryY, boundaryY);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    light.draw();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);


    /* Implement: Draw 2D (texture)*/
    if (State == state::Start) {
        texture.draw();  //start image draw
    }
    else if (State == state::command)
    {
        command.draw();   //command image draw

        if (input_start)
        {
            glRasterPos2f(-boundaryX + 30.0f, boundaryY - 60.0f);
            for (auto str : input_char)
            {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str);
            }
        }
    }
    else if (State == state::ScoreBoard)
    {

        string str = "Score Ranking";
        glColor3f(0.0f, 1.0f, 0.0f);
        glRasterPos2f(-300, 300);
        for (int i = 0; i < str.size(); i++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);

        string str2 = "RANK";
        glColor3f(0.0f, 1.0f, 0.0f);
        glRasterPos2f(0, 300);
        for (int i = 0; i < str2.size(); i++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str2[i]);

        string str3 = "PLAYTIME";
        glColor3f(0.0f, 1.0f, 0.0f);
        glRasterPos2f(40, 300);
        for (int i = 0; i < str3.size(); i++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str3[i]);

        string str4 = "BUBBLES";
        glColor3f(0.0f, 1.0f, 0.0f);
        glRasterPos2f(120, 300);
        for (int i = 0; i < str4.size(); i++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str4[i]);

        for (int i = 0; i < scoreBox.size(); i += 2)
        {
            for (int j = i + 2; j < scoreBox.size(); j += 2)
            {
                if (scoreBox[i] > scoreBox[j])
                {
                    swap(scoreBox[i], scoreBox[j]);
                    swap(scoreBox[i + 1], scoreBox[j + 1]);
                }
            }
        }

        if (scoreBox.size() < 20) {
            for (int i = 1; i < scoreBox.size() / 2 + 1; i++)
            {
                string rank = to_string(i);
                glColor3f(0.0f, 1.0f, 0.0f);
                glRasterPos2f(0, 300 - (i - 1) * 2 * 20 - 20);
                for (int i = 0; i < rank.size(); i++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, rank[i]);
            }
            for (int i = 0; i < scoreBox.size(); i += 2)
            {

                string play_time_i = to_string(scoreBox[i]);
                glColor3f(0.0f, 1.0f, 0.0f);
                glRasterPos2f(40, 300 - (i + 1) * 20);
                for (int i = 0; i < play_time_i.size() - 4; i++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time_i[i]);

                string bubbles_i = to_string(scoreBox[i + 1]);
                glColor3f(0.0f, 1.0f, 0.0f);
                glRasterPos2f(120, 300 - (i + 1) * 20);
                for (int i = 0; i < bubbles_i.size() - 7; i++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, bubbles_i[i]);


            }
        }
        else {
            for (int i = 1; i < 11; i++)
            {
                string rank = to_string(i);
                glColor3f(0.0f, 1.0f, 0.0f);
                glRasterPos2f(0, 300 - (i - 1) * 2 * 20 - 20);
                for (int i = 0; i < rank.size(); i++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, rank[i]);
            }
            for (int i = 0; i < 20; i += 2)
            {

                string play_time_i = to_string(scoreBox[i]);
                glColor3f(0.0f, 1.0f, 0.0f);
                glRasterPos2f(40, 300 - (i + 1) * 20);
                for (int i = 0; i < play_time_i.size() - 4; i++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time_i[i]);

                string bubbles_i = to_string(scoreBox[i + 1]);
                glColor3f(0.0f, 1.0f, 0.0f);
                glRasterPos2f(120, 300 - (i + 1) * 20);
                for (int i = 0; i < bubbles_i.size() - 7; i++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, bubbles_i[i]);


            }
        }
    }
    else if (State == state::clear)
    {
        clear.draw();
    }
    else if (State == state::Game_over)
    {

        Gameover.draw();
    }
    else
    {
        if (State == state::stage1)
        {
            if (menu == THEME1)
            {
                theme1.draw();
            }
            if (menu == THEME2)
            {
                theme2.draw();
            }
            if (menu == THEME3)
            {
                theme3.draw();
            }

            string str = "BUBBLES";
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-260, 300);
            for (int i = 0; i < str.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);

            string buble_num_str = to_string(bubble_num);
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-260, 280);
            for (int i = 0; i < buble_num_str.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, buble_num_str[i]);


            string play_time_str = "PLAY TIME";
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-180, 300);
            for (int i = 0; i < play_time_str.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time_str[i]);

            string play_time = to_string(round((double)(clock() - start_time) / CLOCKS_PER_SEC * 100) / 100);
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-180, 280);
            for (int i = 0; i < play_time.size() - 4; i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time[i]);


            for (int i = 0; i < 16; i++)
            {
                map_image.draw
                (maps[0].getCenterX() - maps[0].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[0].getCenterY());
            } //map ground draw  
            for (int j = 0; j < 15; j++)
            {
                map_image.draw
                (maps[1].getCenterX(), maps[1].getCenterY() + maps[1].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
                map_image.draw
                (maps[2].getCenterX(), maps[2].getCenterY() + maps[2].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
            } //map leftside, rightside draw

            for (int i = 0; i < 8; i++)
            {
                map_image.draw
                (maps[3].getCenterX() - maps[3].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[3].getCenterY());
                map_image.draw
                (maps[6].getCenterX() - maps[6].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[6].getCenterY());
                map_image.draw
                (maps[9].getCenterX() - maps[9].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[9].getCenterY());

            }//map4 draw
            for (int j = 0; j < 3; j++)
            {
                map_image.draw
                (maps[4 + 3 * j].getCenterX(), maps[4 + 3 * j].getCenterY());
                map_image.draw
                (maps[5 + 3 * j].getCenterX(), maps[5 + 3 * j].getCenterY());
            }//map5, 6 draw
        }
        else if (State == state::stage2)
        {
            if (menu == THEME1)
            {
                theme1.draw();
            }
            if (menu == THEME2)
            {
                theme2.draw();
            }
            if (menu == THEME3)
            {
                theme3.draw();
            }
            string str = "BUBBLES";
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-260, 300);
            for (int i = 0; i < str.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);

            string buble_num_str = to_string(bubble_num);
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-260, 280);
            for (int i = 0; i < buble_num_str.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, buble_num_str[i]);


            string play_time_str = "PLAY TIME";
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-180, 300);
            for (int i = 0; i < play_time_str.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time_str[i]);

            string play_time = to_string(round((double)(clock() - start_time) / CLOCKS_PER_SEC * 100) / 100);
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-180, 280);
            for (int i = 0; i < play_time.size() - 4; i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time[i]);
            for (int i = 0; i < 6; i++)
            {
                map_image.draw
                (maps[0].getCenterX() - maps[0].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[0].getCenterY());
                map_image.draw
                (maps[3].getCenterX() - maps[3].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[3].getCenterY());
            }//map ground 1,2 draw 
            for (int j = 0; j < 15; j++)
            {
                map_image.draw
                (maps[1].getCenterX(), maps[1].getCenterY() + maps[1].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
                map_image.draw
                (maps[2].getCenterX(), maps[2].getCenterY() + maps[2].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
            } //map leftside, rightside draw

            for (int i = 0; i < 4; i++)
            {
                map_image.draw
                (maps[4].getCenterX() - maps[4].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[4].getCenterY());
                map_image.draw
                (maps[5].getCenterX() - maps[5].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[5].getCenterY());
                map_image.draw
                (maps[8].getCenterX() - maps[8].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[8].getCenterY());
            } //map 4,5,8 draw 
            for (int i = 0; i < 10; i++)
            {
                map_image.draw
                (maps[6].getCenterX() - maps[6].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[6].getCenterY());
            }//map 6 draw 
            for (int i = 0; i < 2; i++)
            {
                map_image.draw
                (maps[7].getCenterX() - maps[7].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[7].getCenterY());
                map_image.draw
                (maps[9].getCenterX() - maps[9].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[9].getCenterY());
            }//map 7,9 draw 
        }
        else if (State == state::stage3)
        {
            if (menu == THEME1)
            {
                theme1.draw();
            }
            if (menu == THEME2)
            {
                theme2.draw();
            }
            if (menu == THEME3)
            {
                theme3.draw();
            }
            string str = "BUBBLES";
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-260, 300);
            for (int i = 0; i < str.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);

            string buble_num_str = to_string(bubble_num);
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-260, 280);
            for (int i = 0; i < buble_num_str.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, buble_num_str[i]);


            string play_time_str = "PLAY TIME";
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-180, 300);
            for (int i = 0; i < play_time_str.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time_str[i]);

            string play_time = to_string(round((double)(clock() - start_time) / CLOCKS_PER_SEC * 100) / 100);
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-180, 280);
            for (int i = 0; i < play_time.size() - 4; i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time[i]);

            for (int i = 0; i < 4; i++)
            {
                map_image2.draw
                (maps[0].getCenterX() - maps[0].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[0].getCenterY());
                map_image2.draw
                (maps[4].getCenterX() - maps[4].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[4].getCenterY());
            }   //map ground 1,2 draw 
            for (int j = 0; j < 15; j++)
            {
                map_image2.draw
                (maps[1].getCenterX(), maps[1].getCenterY() + maps[1].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
                map_image2.draw
                (maps[2].getCenterX(), maps[2].getCenterY() + maps[2].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
            } //map leftside, rightside draw

            for (int i = 0; i < 2; i++)
            {
                map_image2.draw
                (maps[3].getCenterX() - maps[3].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[3].getCenterY());
                map_image2.draw
                (maps[5].getCenterX() - maps[5].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[5].getCenterY());
                map_image2.draw
                (maps[6].getCenterX() - maps[6].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[6].getCenterY());
                map_image2.draw
                (maps[7].getCenterX() - maps[7].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[7].getCenterY());
                map_image2.draw
                (maps[8].getCenterX() - maps[8].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[8].getCenterY());
            } //map ground3, map 5,6,7,8 draw 
            for (int i = 0; i < 11; i++)
            {
                map_image2.draw
                (maps[9].getCenterX() - maps[9].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[9].getCenterY());
            } //map 9 draw 
            for (int i = 0; i < 3; i++)
            {
                map_image2.draw
                (maps[10].getCenterX() - maps[10].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[10].getCenterY());
            }//map 10 draw
            for (int i = 0; i < 7; i++)
            {
                map_image2.draw
                (maps[11].getCenterX() - maps[11].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[11].getCenterY());
            }//map 11 draw
            map_image2.draw(maps[12].getCenterX(), maps[12].getCenterY());
            //map 12 draw

        }
       else if (State == state::stage4)
        {
            if (menu == THEME1)
            {
                theme1.draw();
            }
            if (menu == THEME2)
            {
                theme2.draw();
            }
            if (menu == THEME3)
            {
                theme3.draw();
            }
            string str = "BUBBLES";
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-260, 300);
            for (int i = 0; i < str.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);

            string buble_num_str = to_string(bubble_num);
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-260, 280);
            for (int i = 0; i < buble_num_str.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, buble_num_str[i]);


            string play_time_str = "PLAY TIME";
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-180, 300);
            for (int i = 0; i < play_time_str.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time_str[i]);

            string play_time = to_string(round((double)(clock() - start_time) / CLOCKS_PER_SEC * 100) / 100);
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2f(-180, 280);
            for (int i = 0; i < play_time.size() - 4; i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time[i]);

            for (int i = 0; i < 4; i++)
            {
                map_image2.draw
                (maps[0].getCenterX() - maps[0].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[0].getCenterY());
                map_image2.draw
                (maps[4].getCenterX() - maps[4].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[4].getCenterY());
                map_image2.draw(-80 + BLOCK / 2 + i * BLOCK, -40 + BLOCK /2);
            }   //map ground 1,3 draw 
            for (int j = 0; j < 15; j++)
            {
                map_image2.draw
                (maps[1].getCenterX(), maps[1].getCenterY() + maps[1].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
                map_image2.draw
                (maps[2].getCenterX(), maps[2].getCenterY() + maps[2].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
            } //map leftside, rightside draw
            for (int i = 0; i < 2; i++)
            {
                map_image2.draw
                (maps[3].getCenterX() - maps[3].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[3].getCenterY());
                map_image2.draw
                (maps[5].getCenterX() - maps[5].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[5].getCenterY());
                map_image2.draw(- BLOCK / 2 + i * BLOCK, -80 + BLOCK / 2);
                map_image2.draw
                (maps[11].getCenterX() - maps[11].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[11].getCenterY());
            } //map ground2, map 5,9,11 draw 
            map_image2.draw(maps[10].getCenterX(), maps[10].getCenterY());
            //map 10 draw
            map_image2.draw(maps[13].getCenterX(), maps[13].getCenterY()); 
            //map 13 draw
            map_image2.draw(maps[14].getCenterX(), maps[14].getCenterY()); 
            //map 14 draw
            map_image2.draw(maps[15].getCenterX(), maps[15].getCenterY());
            //map 15 draw
            for (int i = 0; i < 3; i++)
            {
                map_image2.draw
                (maps[12].getCenterX() - maps[12].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[12].getCenterY());
            }//map 12 draw

            for (int i = 0; i < 6; i++)
            {
                map_image2.draw
                (maps[6].getCenterX() - maps[6].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[6].getCenterY() - BLOCK/2);
                map_image2.draw
                (maps[6].getCenterX() - maps[6].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[6].getCenterY() + BLOCK / 2);
            }//map 6 draw            
            for (int j = 0; j < 7; j++)
            {
                map_image_rock.draw
                (maps[7].getCenterX(), maps[7].getCenterY() + maps[7].getHeight() / 2 - BLOCK / 2 - j * BLOCK);                
            } //map 7 draw 

            
        }       
       else if (State == state::stage5)
       {
           if (menu == THEME1)
           {
               theme1.draw();
           }
           if (menu == THEME2)
           {
               theme2.draw();
           }
           if (menu == THEME3)
           {
               theme3.draw();
           }
           string str = "BUBBLES";
           glColor3f(0.0f, 1.0f, 0.0f);
           glRasterPos2f(-260, 300);
           for (int i = 0; i < str.size(); i++)
               glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);

           string buble_num_str = to_string(bubble_num);
           glColor3f(0.0f, 1.0f, 0.0f);
           glRasterPos2f(-260, 280);
           for (int i = 0; i < buble_num_str.size(); i++)
               glutBitmapCharacter(GLUT_BITMAP_9_BY_15, buble_num_str[i]);


           string play_time_str = "PLAY TIME";
           glColor3f(0.0f, 1.0f, 0.0f);
           glRasterPos2f(-180, 300);
           for (int i = 0; i < play_time_str.size(); i++)
               glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time_str[i]);

           string play_time = to_string(round((double)(clock() - start_time) / CLOCKS_PER_SEC * 100) / 100);
           glColor3f(0.0f, 1.0f, 0.0f);
           glRasterPos2f(-180, 280);
           for (int i = 0; i < play_time.size() - 4; i++)
               glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time[i]);

           for (int i = 0; i < 2; i++)
           {
               map_image3.draw
               (maps[0].getCenterX() - maps[0].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[0].getCenterY());
               map_image3.draw
               (maps[3].getCenterX() - maps[3].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[3].getCenterY());
               map_image3.draw
               (maps[6].getCenterX() - maps[6].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[6].getCenterY());
               map_image3.draw
               (maps[9].getCenterX() - maps[9].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[9].getCenterY());
               map_image3.draw
               (maps[11].getCenterX() - maps[11].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[11].getCenterY());               
           } 
           for (int j = 0; j < 15; j++)
           {
               map_image3.draw
               (maps[1].getCenterX(), maps[1].getCenterY() + maps[1].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
               map_image3.draw
               (maps[2].getCenterX(), maps[2].getCenterY() + maps[2].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
           } //map leftside, rightside draw
           for (int i = 0; i < 3; i++)
           {
               
               map_image3.draw
               (maps[8].getCenterX() - maps[8].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[8].getCenterY());
               map_image3.draw
               (maps[12].getCenterX() - maps[12].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[12].getCenterY());
           }
           for (int i = 0; i < 5; i++)
           {
               map_image3.draw
               (maps[10].getCenterX() - maps[10].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[10].getCenterY());
           }
           for (int j = 0; j < 4; j++)
           {
               map_image3.draw
               (maps[4].getCenterX() - maps[4].getWidth() / 2 + BLOCK / 2 + j * BLOCK, maps[4].getCenterY());
               map_image3.draw
               (maps[5].getCenterX() - maps[5].getWidth() / 2 + BLOCK / 2 + j * BLOCK, maps[5].getCenterY());
               map_image_rock.draw
               (maps[7].getCenterX(), maps[7].getCenterY() + maps[7].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
           } //map 7 draw 

       }
       else if (State == state::stage6)
       {
           if (menu == THEME1)
           {
               theme1.draw();
           }
           if (menu == THEME2)
           {
               theme2.draw();
           }
           if (menu == THEME3)
           {
               theme3.draw();
           }
           string str = "BUBBLES";
           glColor3f(0.0f, 1.0f, 0.0f);
           glRasterPos2f(-260, 300);
           for (int i = 0; i < str.size(); i++)
               glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);

           string buble_num_str = to_string(bubble_num);
           glColor3f(0.0f, 1.0f, 0.0f);
           glRasterPos2f(-260, 280);
           for (int i = 0; i < buble_num_str.size(); i++)
               glutBitmapCharacter(GLUT_BITMAP_9_BY_15, buble_num_str[i]);


           string play_time_str = "PLAY TIME";
           glColor3f(0.0f, 1.0f, 0.0f);
           glRasterPos2f(-180, 300);
           for (int i = 0; i < play_time_str.size(); i++)
               glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time_str[i]);

           string play_time = to_string(round((double)(clock() - start_time) / CLOCKS_PER_SEC * 100) / 100);
           glColor3f(0.0f, 1.0f, 0.0f);
           glRasterPos2f(-180, 280);
           for (int i = 0; i < play_time.size() - 4; i++)
               glutBitmapCharacter(GLUT_BITMAP_9_BY_15, play_time[i]);


           map_image3.draw(maps[0].getCenterX(), maps[0].getCenterY());
           map_image3.draw(maps[4].getCenterX(), maps[4].getCenterY());
           map_image3.draw(maps[5].getCenterX(), maps[5].getCenterY());
           map_image3.draw(maps[6].getCenterX(), maps[6].getCenterY());
           for (int i = 8; i < 12; i++)
           {
               map_image3.draw(maps[i].getCenterX(), maps[i].getCenterY());
           }                 
           map_image3.draw(maps[13].getCenterX(), maps[13].getCenterY());
           map_image3.draw(maps[14].getCenterX(), maps[14].getCenterY());
           map_image3.draw(maps[15].getCenterX(), maps[15].getCenterY());
           map_image3.draw(maps[17].getCenterX(), maps[17].getCenterY());
           map_image3.draw(maps[20].getCenterX(), maps[20].getCenterY());
           map_image3.draw(maps[21].getCenterX(), maps[21].getCenterY());
           map_image3.draw(maps[22].getCenterX(), maps[22].getCenterY());           

           for (int j = 0; j < 16; j++)
           {
               map_image3.draw
               (maps[1].getCenterX(), maps[1].getCenterY() + maps[1].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
               map_image3.draw
               (maps[2].getCenterX(), maps[2].getCenterY() + maps[2].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
           } //map leftside, rightside draw

           for (int i = 0; i < 2; i++)
           {
               map_image3.draw
               (maps[3].getCenterX() - maps[3].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[3].getCenterY());
               map_image3.draw
               (maps[12].getCenterX() - maps[12].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[12].getCenterY());
               map_image3.draw
               (maps[16].getCenterX() - maps[16].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[16].getCenterY());
               map_image3.draw
               (maps[18].getCenterX() - maps[18].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[18].getCenterY());
               map_image3.draw
               (maps[19].getCenterX() - maps[19].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[19].getCenterY());
               map_image3.draw
               (maps[23].getCenterX() - maps[23].getWidth() / 2 + BLOCK / 2 + i * BLOCK, maps[23].getCenterY());
               
           }
           
           for (int j = 0; j < 7; j++)
           {               
               map_image_rock.draw
               (maps[7].getCenterX(), maps[7].getCenterY() + maps[7].getHeight() / 2 - BLOCK / 2 - j * BLOCK);
           } //map 7 draw 

           }
        
        if (!player.getDameged())
        {
            if (trun_left)
            {
                playerimage_left.draw();
            }
            else
            {
                playerimage_right.draw();
            } //Player draw
        }
        else
        {
            if (player.get_blink() % 2 == 0)
            {
                if (trun_left)
                {
                    playerimage_left.draw();
                }
                else
                {
                    playerimage_right.draw();
                } //Player draw
            }
        }

        switch (life)
        {
        case 3:
            life_image[0].draw();
            life_image[1].draw();
            life_image[2].draw();
            break;

        case 2:
            life_image[0].draw();
            life_image[1].draw();
            break;

        case 1:
            life_image[0].draw();
            break;

        case 0:
            game_over = true;
            break;
        }  //life draw


        /* Implement: Draw 3D (light and spheres)*/
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        light.draw();

        for (int i = 0; i < bubbles.size(); i++)
        {
            if (!bubbles[i].get_pop_out())
            {
                bubbles[i].draw();
            }
        }

        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        for (int i = 0; i < monsters.size(); i++)
        {
            if (monsters[i].trun_left())
            {
                monster_image[i][0].draw();
            }
            else
            {
                monster_image[i][1].draw();
            }
        } // monster draw

    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) 
{
    if (key == 32 && !game_over)
    {

        Bubble bub(buble_size, 100, 100);
        if (trun_left == true) {
            bub.setVelocity(-6.0f, 0.0f, 0.0f);
        }
        else
        {
            bub.setVelocity(6.0f, 0.0f, 0.0f);
        }
        bub.setCenter(player.getCenterX(), player.getCenterY(), player.getCenterZ());
        bub.setFirstcenter(); // 최초 x좌표를 넣어서, 올라가는 시점을 지정

        time_for_bubbles.push_back(30);
        bubbles.push_back(bub);
        bubble_num += 1;

    }

    return;
}
void special_keyboard(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP:
        if ((State == state::stage1 || State == state::stage2 || State == state::stage3 
            || State == state::stage4 || State == state::stage5 || State == state::stage6) && !game_over)
        {
            if (!player_drop)
            {
                jump_move = true;
            }
        }
        return;
    case  GLUT_KEY_LEFT:
        if ((State == state::stage1 || State == state::stage2 || State == state::stage3
            || State == state::stage4 || State == state::stage5 || State == state::stage6) && !game_over)
        {
            left_move = true;
            trun_left = true;

            slide = true;
        }
        return;
    case GLUT_KEY_RIGHT:
        if ((State == state::stage1 || State == state::stage2 || State == state::stage3
            || State == state::stage4 || State == state::stage5 || State == state::stage6) && !game_over)
        {
            right_move = true;
            trun_left = false;

            slide = true;
        }
        return;
    default:
        break;
    }
}
void special_keyboard_up(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        left_move = false;
        break;

    case GLUT_KEY_RIGHT:
        right_move = false;
        break;

    default:
        break;
    }

}

void stage_setting()
{
    if (State == state::Start)
    {
        State = state::command;
    }
    else if (State == state::command)
    {

        // stage1 map 입력
        map map_ground = map();
        map_ground.set_map(0, -320 + BLOCK / 2, BLOCK * 16, BLOCK);
        maps.push_back(map_ground);
        // ground

        map map_leftside = map();
        map_leftside.set_map(-320 + BLOCK / 2, BLOCK / 2, BLOCK, BLOCK * 15);
        maps.push_back(map_leftside);
        //left side

        map map_rightside = map();
        map_rightside.set_map(280 + BLOCK / 2, BLOCK / 2, BLOCK, BLOCK * 15);
        maps.push_back(map_rightside);
        //right side


        for (int k = 0; k < 3; k++)
        {
            map map4 = map();
            map4.set_map(0, -160 + BLOCK / 2 + BLOCK * 4 * k, BLOCK * 8, BLOCK);
            maps.push_back(map4);

            map map5 = map();
            map5.set_map(-280 + BLOCK / 2,
                -160 + BLOCK / 2 + BLOCK * 4 * k, BLOCK, BLOCK);
            maps.push_back(map5);

            map map6 = map();
            map6.set_map(240 + BLOCK / 2,
                -160 + BLOCK / 2 + BLOCK * 4 * k, BLOCK, BLOCK);
            maps.push_back(map6);

            Monster ex_monster = Monster();
            ex_monster.set_Monster(BLOCK / 2,
                -120 + BLOCK * 4 * k + BLOCK / 2, 0.0f, monster);
            monsters.push_back(ex_monster);

            monster_image[k][0].setposition(BLOCK / 2,
                -120 + BLOCK * 4 * k + BLOCK / 2, monster, monster);
            monster_image[k][1].setposition(BLOCK / 2,
                -120 + BLOCK * 4 * k + BLOCK / 2, monster, monster);


        }
        monsters[1].set_v(-moving);

        State = state::stage1;
        glutKeyboardFunc(keyboard);
        glutSpecialFunc(special_keyboard);
        glutSpecialUpFunc(special_keyboard_up);

        glutKeyboardUpFunc(keyboard_restart);


    }
    else if (State == state::stage2)
    {
        bubbles.clear();
        bubbles.shrink_to_fit();
        maps.clear();
        maps.shrink_to_fit();

        player.set_Player(-200, -260, 0, PLAYER_SIZE);
        player.blink_reset();
        player.reset_v();
        playerimage_left.setposition(-200, -260, PLAYER_SIZE, PLAYER_SIZE);
        playerimage_right.setposition(-200, -260, PLAYER_SIZE, PLAYER_SIZE);

        trun_left = false;
        left_move = false;
        right_move = false;
        jump_move = false;
        player_drop = false;

        map map_ground1 = map();
        map_ground1.set_map(-200, -320 + BLOCK / 2, BLOCK * 6, BLOCK);
        maps.push_back(map_ground1);
        // ground 1

        map map_leftside = map();
        map_leftside.set_map(-320 + BLOCK / 2, BLOCK / 2, BLOCK, BLOCK * 15);
        maps.push_back(map_leftside);
        //left side

        map map_rightside = map();
        map_rightside.set_map(280 + BLOCK / 2, BLOCK / 2, BLOCK, BLOCK * 15);
        maps.push_back(map_rightside);
        //right side

        map map_ground2 = map();
        map_ground2.set_map(200, -320 + BLOCK / 2, BLOCK * 6, BLOCK);
        maps.push_back(map_ground2);
        // ground 2


        map map4 = map();
        map4.set_map(-200, -160 + BLOCK / 2, BLOCK * 4, BLOCK);
        maps.push_back(map4);

        map map5 = map();
        map5.set_map(200, -160 + BLOCK / 2, BLOCK * 4, BLOCK);
        maps.push_back(map5);

        map map6 = map();
        map6.set_map(0, BLOCK / 2, BLOCK * 10, BLOCK);
        maps.push_back(map6);

        map map7 = map();
        map7.set_map(-240, 160 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps.push_back(map7);

        map map8 = map();
        map8.set_map(0, 160 + BLOCK / 2, BLOCK * 4, BLOCK);
        maps.push_back(map8);

        map map9 = map();
        map9.set_map(240, 160 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps.push_back(map9);


        //monster 위치 세팅
        monsters[0].set_Monster(-160 + BLOCK / 2, -120 + BLOCK / 2, 0.0f, monster);
        monster_image[0][0].setposition(-160 + BLOCK / 2, -120 + BLOCK / 2, monster, monster);
        monster_image[0][1].setposition(-160 + BLOCK / 2, -120 + BLOCK / 2, monster, monster);
        monsters[0].set_v(-moving);
        monsters[0].set_trap_bubble_num(-1);
        monsters[0].set_move(true);


        monsters[1].set_Monster(120 + BLOCK / 2, -120 + BLOCK / 2, 0.0f, monster);
        monster_image[1][0].setposition(120 + BLOCK / 2, -120 + BLOCK / 2, monster, monster);
        monster_image[1][1].setposition(120 + BLOCK / 2, -120 + BLOCK / 2, monster, monster);
        monsters[1].set_trap_bubble_num(-1);
        monsters[1].set_move(true);

        monsters[2].set_Monster(0, 40 + BLOCK / 2, 0.0f, monster);
        monster_image[2][0].setposition(0, 40 + BLOCK / 2, monster, monster);
        monster_image[2][1].setposition(0, 40 + BLOCK / 2, monster, monster);
        monsters[2].set_trap_bubble_num(-1);
        monsters[2].set_move(true);

        stage_clear[0] = false;
        stage_clear[1] = false;
        stage_clear[2] = false;

    }
    else if (State == state::stage3)
    {
        bubbles.clear();
        bubbles.shrink_to_fit();
        maps.clear();
        maps.shrink_to_fit();

        player.set_Player(-200, -260, 0, PLAYER_SIZE);
        player.blink_reset();
        player.reset_v();
        playerimage_left.setposition(-200, -260, PLAYER_SIZE, PLAYER_SIZE);
        playerimage_right.setposition(-200, -260, PLAYER_SIZE, PLAYER_SIZE);

        trun_left = false;
        left_move = false;
        right_move = false;
        jump_move = false;
        player_drop = false;


        map map_ground1 = map();
        map_ground1.set_map(-240, -320 + BLOCK / 2, BLOCK * 4, BLOCK);
        maps.push_back(map_ground1);
        // ground 1

        map map_leftside = map();
        map_leftside.set_map(-320 + BLOCK / 2, BLOCK / 2, BLOCK, BLOCK * 15);
        maps.push_back(map_leftside);
        //left side

        map map_rightside = map();
        map_rightside.set_map(280 + BLOCK / 2, BLOCK / 2, BLOCK, BLOCK * 15);
        maps.push_back(map_rightside);
        //right side

        map map_ground2 = map();
        map_ground2.set_map(0, -320 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps.push_back(map_ground2);
        // ground 2

        map map_ground3 = map();
        map_ground3.set_map(240, -320 + BLOCK / 2, BLOCK * 4, BLOCK);
        maps.push_back(map_ground3);
        // ground 3

        map map5 = map();
        map5.set_map(-200, -160 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps.push_back(map5);

        map map6 = map();
        map6.set_map(-80, -160 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps.push_back(map6);

        map map7 = map();
        map7.set_map(80, -160 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps.push_back(map7);

        map map8 = map();
        map8.set_map(200, -160 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps.push_back(map8);

        map map9 = map();
        map9.set_map(40 + BLOCK / 2, BLOCK / 2, BLOCK * 11, BLOCK);
        maps.push_back(map9);

        map map10 = map();
        map10.set_map(120 + BLOCK / 2, 160 + BLOCK / 2, BLOCK * 3, BLOCK);
        maps.push_back(map10);

        map map11 = map();
        map11.set_map(-160 + BLOCK / 2, 160 + BLOCK / 2, BLOCK * 7, BLOCK);
        maps.push_back(map11);

        map map12 = map();
        map12.set_map(-240 + BLOCK / 2, BLOCK / 2, BLOCK, BLOCK);
        maps.push_back(map12);


        //monster 위치 세팅
        monsters[0].set_Monster(-120 + BLOCK / 2, 40 + BLOCK / 2, 0.0f, monster);
        monster_image[0][0].setposition(-120 + BLOCK / 2, 40 + BLOCK / 2, monster, monster);
        monster_image[0][1].setposition(-120 + BLOCK / 2, 40 + BLOCK / 2, monster, monster);
        monsters[0].set_trap_bubble_num(-1);
        monsters[0].set_move(true);


        monsters[1].set_Monster(160 + BLOCK / 2, 40 + BLOCK / 2, 0.0f, monster);
        monster_image[1][0].setposition(160 + BLOCK / 2, 40 + BLOCK / 2, monster, monster);
        monster_image[1][1].setposition(160 + BLOCK / 2, 40 + BLOCK / 2, monster, monster);
        monsters[1].set_v(-moving);
        monsters[1].set_trap_bubble_num(-1);
        monsters[1].set_move(true);

        monsters[2].set_Monster(-200 + BLOCK / 2, 200 + BLOCK / 2, 0.0f, monster);
        monster_image[2][0].setposition(-200 + BLOCK / 2, 200 + BLOCK / 2, monster, monster);
        monster_image[2][1].setposition(-200 + BLOCK / 2, 200 + BLOCK / 2, monster, monster);
        monsters[2].set_trap_bubble_num(-1);
        monsters[2].set_move(true);

        stage_clear[0] = false;
        stage_clear[1] = false;
        stage_clear[2] = false;
    }
    else if (State == state::stage4)
    {   
        bubbles.clear();
        bubbles.shrink_to_fit();

        player.set_Player(-200, -260, 0, PLAYER_SIZE);
        player.blink_reset();
        player.reset_v();
        playerimage_left.setposition(-200, -260, PLAYER_SIZE, PLAYER_SIZE);
        playerimage_right.setposition(-200, -260, PLAYER_SIZE, PLAYER_SIZE);

        trun_left = false;
        left_move = false;
        right_move = false;
        jump_move = false;
        player_drop = false; 

        maps[0].set_map(-240, -320 + BLOCK / 2, BLOCK * 4, BLOCK);
        maps[3].set_map(0, -320 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps[4].set_map(240, -320 + BLOCK / 2, BLOCK * 4, BLOCK);
        maps[5].set_map(-240, -80 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps[6].set_map(0, 40, BLOCK * 6, BLOCK * 2);
        maps[7].set_map(200 + BLOCK / 2, -80 + BLOCK / 2, BLOCK, BLOCK * 7);
        maps[8].set_map(WINDOW_WIDTH, WINDOW_HEIGHT, BLOCK * 4, BLOCK);
        maps[9].set_map(WINDOW_WIDTH, WINDOW_HEIGHT, BLOCK * 2, BLOCK);
        maps[10].set_map(BLOCK / 2, 160 + BLOCK / 2, BLOCK, BLOCK);
        maps[11].set_map(200, 160 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps[12].set_map(-240 + BLOCK / 2, 200 + BLOCK / 2, BLOCK * 3, BLOCK);     

        map map13 = map();
        map13.set_map(-120 + BLOCK / 2, 200 + BLOCK / 2, BLOCK, BLOCK);
        maps.push_back(map13);

        map map14 = map();
        map14.set_map(160 + BLOCK / 2, -80 + BLOCK / 2, BLOCK, BLOCK);
        maps.push_back(map14);

        map map15 = map();
        map15.set_map(-160 + BLOCK / 2, -200 + BLOCK / 2, BLOCK, BLOCK);
        maps.push_back(map15);


        for (int i = 0; i < 3; i++)
        {
            monsters[i].set_Monster(-40 + BLOCK * i, 80 + BLOCK / 2, 0.0f, monster);
            monster_image[i][0].setposition(-40 + BLOCK * i, 80 + BLOCK / 2, monster, monster);
            monster_image[i][1].setposition(-40 + BLOCK * i, 80 + BLOCK / 2, monster, monster);
            monsters[i].set_trap_bubble_num(-1);
            monsters[i].set_move(true);
        }
        monsters[1].set_v(-moving);

        Monster ex_monster1 = Monster();
        ex_monster1.set_Monster(-280 + BLOCK / 2, -40 + BLOCK / 2, 0.0f, monster);
        monster_image[3][0].setposition(-280 + BLOCK / 2, -40 + BLOCK / 2, monster, monster);
        monster_image[3][1].setposition(-280 + BLOCK / 2, -40 + BLOCK / 2, monster, monster);
        ex_monster1.set_trap_bubble_num(-1);
        ex_monster1.set_move(true);
        monsters.push_back(ex_monster1);

        Monster ex_monster4 = Monster();
        ex_monster4.set_Monster(200 + BLOCK / 2, 200 + BLOCK / 2, 0.0f, monster);
        monster_image[4][0].setposition(200 + BLOCK / 2, 200 + BLOCK / 2, monster, monster);
        monster_image[4][1].setposition(200 + BLOCK / 2, 200 + BLOCK / 2, monster, monster);
        ex_monster4.set_trap_bubble_num(-1);
        ex_monster4.set_move(true);
        monsters.push_back(ex_monster4);
        monsters[4].set_v(-moving);

        Monster ex_monster5 = Monster();
        ex_monster5.set_Monster(-280 + BLOCK / 2, 240 + BLOCK / 2, 0.0f, monster);
        monster_image[5][0].setposition(-280 + BLOCK / 2, 240 + BLOCK / 2, monster, monster);
        monster_image[5][1].setposition(-280 + BLOCK / 2, 240 + BLOCK / 2, monster, monster);
        ex_monster5.set_trap_bubble_num(-1);
        ex_monster5.set_move(true);
        monsters.push_back(ex_monster5);

        stage_clear[0] = false;
        stage_clear[1] = false;
        stage_clear[2] = false;

    }
    else if (State == state::stage5)
    {
        bubbles.clear();
        bubbles.shrink_to_fit();

        player.set_Player(-200, -260, 0, PLAYER_SIZE);
        player.blink_reset();
        player.reset_v();
        playerimage_left.setposition(-200, -260, PLAYER_SIZE, PLAYER_SIZE);
        playerimage_right.setposition(-200, -260, PLAYER_SIZE, PLAYER_SIZE);

        trun_left = false;
        left_move = false;
        right_move = false;
        jump_move = false;
        player_drop = false;

        maps[0].set_map(-200, -320 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps[3].set_map(0, -320 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps[4].set_map(240, -320 + BLOCK / 2, BLOCK * 4, BLOCK);
        maps[5].set_map(120, -200 + BLOCK / 2, BLOCK * 4, BLOCK);
        maps[6].set_map(-160, -120 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps[7].set_map(-240 + BLOCK / 2, -40, BLOCK, BLOCK * 4);
        maps[8].set_map(-240 + BLOCK / 2, 40 + BLOCK / 2, BLOCK * 3, BLOCK);
        maps[9].set_map(-40, 40 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps[10].set_map(160 + BLOCK / 2, BLOCK / 2, BLOCK*5, BLOCK);
        maps[11].set_map(-200, 160 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps[12].set_map(120 + BLOCK / 2, 160 + BLOCK / 2, BLOCK * 3, BLOCK);
        
        maps.pop_back();
        maps.pop_back();
        maps.pop_back();


        for (int i = 0; i < 3; i++)
        {
            monsters[i].set_Monster(-240 + (BLOCK/2) * i, 80 + BLOCK / 2, 0.0f, monster);
            monster_image[i][0].setposition(-240 + (BLOCK / 2) * i, 80 + BLOCK / 2, monster, monster);
            monster_image[i][1].setposition(-240 + (BLOCK / 2) * i, 80 + BLOCK / 2, monster, monster);
            monsters[i].set_trap_bubble_num(-1);
            monsters[i].set_move(true);
        }
        monsters[1].set_v(-moving);


        monsters[3].set_Monster(40 + BLOCK / 2, -160 + BLOCK / 2, 0.0f, monster);
        monster_image[3][0].setposition(40 + BLOCK / 2, -160 + BLOCK / 2, monster, monster);
        monster_image[3][1].setposition(40 + BLOCK / 2, -160 + BLOCK / 2, monster, monster);
        monsters[3].set_trap_bubble_num(-1);
        monsters[3].set_move(true);

        monsters[4].set_Monster(-80 + BLOCK /2, 80 + BLOCK / 2, 0.0f, monster);
        monster_image[4][0].setposition(-80 + BLOCK / 2, 80 + BLOCK / 2, monster, monster);
        monster_image[4][1].setposition(-80 + BLOCK / 2, 80 + BLOCK / 2, monster, monster);
        monsters[4].set_trap_bubble_num(-1);
        monsters[4].set_move(true);

        monsters[5].set_Monster(160 + BLOCK /2, 200 + BLOCK / 2, 0.0f, monster);
        monster_image[5][0].setposition(160 + BLOCK / 2, 200 + BLOCK / 2, monster, monster);
        monster_image[5][1].setposition(160 + BLOCK / 2, 200 + BLOCK / 2, monster, monster);
        monsters[5].set_trap_bubble_num(-1);
        monsters[5].set_move(true);


        for (int i = 0; i < 6; i++)
        {
            stage_clear[i] = false;
            cout << "stage clear " << i << "is false" << endl;
        }

    }
    else if (State == state::stage6)
    {
        bubbles.clear();
        bubbles.shrink_to_fit();

        player.set_Player(-100, -260, 0, PLAYER_SIZE);
        player.blink_reset();
        player.reset_v();
        playerimage_left.setposition(-100, -220, PLAYER_SIZE, PLAYER_SIZE);
        playerimage_right.setposition(-100, -220, PLAYER_SIZE, PLAYER_SIZE);

        trun_left = false;
        left_move = false;
        right_move = false;
        jump_move = false;
        player_drop = false;

        maps[1].set_map(-320 + BLOCK / 2, 0, BLOCK, BLOCK * 16);
        maps[2].set_map(280 + BLOCK / 2, 0, BLOCK, BLOCK * 16);

        maps[0].set_map(-240 + BLOCK / 2, -320 + BLOCK / 2, BLOCK, BLOCK);
        maps[3].set_map(40, -320 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps[4].set_map(160 + BLOCK / 2, -320 + BLOCK / 2, BLOCK, BLOCK);

        maps[5].set_map(200 + BLOCK / 2, -280 + BLOCK / 2, BLOCK, BLOCK);
        maps[6].set_map(-120 + BLOCK / 2, -280 + BLOCK / 2, BLOCK, BLOCK);

        maps[7].set_map(-200 + BLOCK / 2, 40 + BLOCK / 2, BLOCK, BLOCK * 7);

        maps[8].set_map(-80 + BLOCK / 2, -200 + BLOCK / 2, BLOCK, BLOCK);
        maps[9].set_map(40 + BLOCK / 2, -200 + BLOCK / 2, BLOCK, BLOCK);
        maps[10].set_map(240 + BLOCK / 2, -200 + BLOCK / 2, BLOCK, BLOCK);

        maps[11].set_map(-240 + BLOCK / 2, -120 + BLOCK / 2, BLOCK, BLOCK);

        maps[12].set_map(-80, -80 + BLOCK / 2, BLOCK * 3, BLOCK);

        map map13 = map();
        map13.set_map(160 + BLOCK / 2, -160 + BLOCK / 2, BLOCK, BLOCK);
        maps.push_back(map13); //13
        map13.set_map(80 + BLOCK / 2, -80 + BLOCK / 2, BLOCK, BLOCK);
        maps.push_back(map13); //14

        map13.set_map(120 + BLOCK / 2, -40 + BLOCK / 2, BLOCK, BLOCK);
        maps.push_back(map13); //15

        map13.set_map(200, BLOCK / 2, BLOCK * 2, BLOCK);
        maps.push_back(map13); //16

        map13.set_map(-240 + BLOCK / 2, 80 + BLOCK / 2, BLOCK, BLOCK);
        maps.push_back(map13); //17
        map13.set_map(-80, 80 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps.push_back(map13); //18
        map13.set_map(80, 80 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps.push_back(map13); //19

        map13.set_map(240 + BLOCK / 2, 120 + BLOCK / 2, BLOCK, BLOCK);
        maps.push_back(map13); //20


        map13.set_map(-280 + BLOCK / 2, 200 + BLOCK / 2, BLOCK, BLOCK);
        maps.push_back(map13); //21
        map13.set_map(-120 + BLOCK / 2, 200 + BLOCK / 2, BLOCK, BLOCK);
        maps.push_back(map13); //22
        map13.set_map(120, 200 + BLOCK / 2, BLOCK * 2, BLOCK);
        maps.push_back(map13); //23

        monsters[0].set_Monster(maps[0].getCenterX(), maps[0].getCenterY() + BLOCK, 0.0f, monster);
        monster_image[0][0].setposition(maps[0].getCenterX(), maps[0].getCenterY() + BLOCK, monster, monster);
        monster_image[0][1].setposition(maps[0].getCenterX(), maps[0].getCenterY() + BLOCK, monster, monster);
        monsters[0].set_trap_bubble_num(-1);
        monsters[0].set_move(true);

        monsters[1].set_Monster(maps[3].getCenterX(), maps[3].getCenterY() + BLOCK, 0.0f, monster);
        monster_image[1][0].setposition(maps[3].getCenterX(), maps[3].getCenterY() + BLOCK, monster, monster);
        monster_image[1][1].setposition(maps[3].getCenterX(), maps[3].getCenterY() + BLOCK, monster, monster);
        monsters[1].set_trap_bubble_num(-1);
        monsters[1].set_move(true);

        monsters[2].set_Monster(maps[12].getCenterX(), maps[12].getCenterY() + BLOCK, 0.0f, monster);
        monster_image[2][0].setposition(maps[12].getCenterX(), maps[12].getCenterY() + BLOCK, monster, monster);
        monster_image[2][1].setposition(maps[12].getCenterX(), maps[12].getCenterY() + BLOCK, monster, monster);
        monsters[2].set_trap_bubble_num(-1);
        monsters[2].set_move(true);

        monsters[3].set_Monster(maps[16].getCenterX(), maps[16].getCenterY() + BLOCK, 0.0f, monster);
        monster_image[3][0].setposition(maps[16].getCenterX(), maps[16].getCenterY() + BLOCK, monster, monster);
        monster_image[3][1].setposition(maps[16].getCenterX(), maps[16].getCenterY() + BLOCK, monster, monster);
        monsters[3].set_trap_bubble_num(-1);
        monsters[3].set_move(true);

        monsters[4].set_Monster(maps[18].getCenterX(), maps[18].getCenterY() + BLOCK, 0.0f, monster);
        monster_image[4][0].setposition(maps[18].getCenterX(), maps[18].getCenterY() + BLOCK, monster, monster);
        monster_image[4][1].setposition(maps[18].getCenterX(), maps[18].getCenterY() + BLOCK, monster, monster);
        monsters[4].set_trap_bubble_num(-1);
        monsters[4].set_move(true);

        monsters[5].set_Monster(maps[19].getCenterX(), maps[19].getCenterY() + BLOCK, 0.0f, monster);
        monster_image[5][0].setposition(maps[19].getCenterX(), maps[19].getCenterY() + BLOCK, monster, monster);
        monster_image[5][1].setposition(maps[19].getCenterX(), maps[19].getCenterY() + BLOCK, monster, monster);
        monsters[5].set_trap_bubble_num(-1);
        monsters[5].set_move(true);

       
        Monster ex_monster = Monster();

        ex_monster.set_Monster(maps[21].getCenterX(), maps[21].getCenterY() + BLOCK, 0.0f, monster);
        monster_image[6][0].setposition(maps[21].getCenterX(), maps[21].getCenterY() + BLOCK, monster, monster);
        monster_image[6][1].setposition(maps[21].getCenterX(), maps[21].getCenterY() + BLOCK, monster, monster);
        ex_monster.set_trap_bubble_num(-1);
        ex_monster.set_move(true);
        monsters.push_back(ex_monster);

        ex_monster.set_Monster(maps[23].getCenterX(), maps[23].getCenterY() + BLOCK, 0.0f, monster);
        monster_image[7][0].setposition(maps[23].getCenterX(), maps[23].getCenterY() + BLOCK, monster, monster);
        monster_image[7][1].setposition(maps[23].getCenterX(), maps[23].getCenterY() + BLOCK, monster, monster);
        ex_monster.set_trap_bubble_num(-1);
        ex_monster.set_move(true);
        monsters.push_back(ex_monster);

       

        for (int i = 0; i < 6; i++)
        {
            stage_clear[i] = false;            
        }

    }
 
    else if (State == state::ScoreBoard)
    {
        State = state::clear;
    }
    else if (State == state::Game_over || State == state::clear)
    {
        State = state::Start;
        score = 0;
        life = 3;
        buble_range = 300.0f;
        buble_size = 20.0f;
        bubbles.clear();
        bubbles.shrink_to_fit();
        monsters.clear();
        monsters.shrink_to_fit();
        maps.clear();
        maps.shrink_to_fit();        

        player.set_Player(-200, -260, 0, PLAYER_SIZE);
        player.blink_reset();
        player.reset_v();
        playerimage_left.setposition(-200, -260, PLAYER_SIZE, PLAYER_SIZE);
        playerimage_right.setposition(-200, -260, PLAYER_SIZE, PLAYER_SIZE);

        trun_left = false;
        left_move = false;
        right_move = false;
        jump_move = false;
        player_drop = false;
        slide = false;

        game_over = false;

        
        bubble_num = 0;
        start_time = clock();

        for (int i = 0; i < 8; i++)
        {
            stage_clear[i] = false;
        }
    }
}
void keyboard_start(unsigned char key, int x, int y)
{
    if (State == state::command && key == 13)
    {
        input_char.clear();
        input_char.append("Enter : ");

        input_start = true;
        glutKeyboardUpFunc(keyboard_input_close);
        glutKeyboardFunc(keyboard_input);
        glutSpecialUpFunc(keyboard_restart);
    }
    else
    {
        stage_setting();
    }    
}
void special_keyboard_start(int key, int x, int y)
{
    stage_setting();
}
void keyboard_input(unsigned char key, int x, int y)
{
    if (key != 13)
    {
        if (key != 8)
        {
            input_char.append(1, key);
        }
        else
        {
            if (input_char.size() > 8)
            {
                input_char.pop_back();
            }
        }
    }
}
void keyboard_input_close(unsigned char key, int x, int y)
{
    if (key == 13)
    {
        glutKeyboardUpFunc(keyboard_start);
        glutSpecialUpFunc(special_keyboard_start);
        glutKeyboardFunc(keyboard_restart);
        input_start = false;

        if(input_char == power_overwhelming)
        {
            life = -1;
        }
        else if (input_char == long_shot)
        {
            buble_range = WINDOW_WIDTH;
        }
        else if (input_char == big_shot)
        {
            buble_size = 50.0f;
        }
        else if (input_char == as_soon_as_possible)
        {
            time_cheat = true;
        }
    }
}

void sub_menu_function(int option) {

    menu2 = option;
    glutPostRedisplay();
}
void sub_menu_function_quad(int option) {

    menu = option;
    glutPostRedisplay();
}
void main_menu_function(int option) {

    menu = option;
    if (option == QUIT)
        exit(0);
    else if (option == CLEAR)

        glutPostRedisplay();
}
void sub_menu_function_AUDIO(int option) {


    menu2 = option;
    glutPostRedisplay();
}
void sub_menu_function_THEME(int option) {

    menu = option;
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    // init GLUT and create Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(WINDOW_X, WINDOW_Y);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Bubble Bobble");

    initialize();
    // popup 
    int mainmenu, BackGroundChange, AudioChange;
    BackGroundChange = glutCreateMenu(sub_menu_function_THEME);
    glutAddMenuEntry("THEME1", THEME1);
    glutAddMenuEntry("THEME2", THEME2);
    glutAddMenuEntry("THEME3", THEME3);
    glutAddMenuEntry("NOTHEME", NOTHEME);
    AudioChange = glutCreateMenu(sub_menu_function_AUDIO);
    glutAddMenuEntry("AUDIO0", AUDIO0);
    glutAddMenuEntry("AUDIO1", AUDIO1);
    glutAddMenuEntry("AUDIO2", AUDIO2);
    glutAddMenuEntry("NOAUDIO", NOAUDIO);
    mainmenu = glutCreateMenu(main_menu_function);
    glutAddMenuEntry("Quit", QUIT);
    glutAddSubMenu("BackGroundChange", BackGroundChange);
    glutAddSubMenu("AudioChange", AudioChange);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    // register callbacks
    PlaySound(TEXT("Bouble Main-Theme.wav"), NULL, SND_ASYNC | SND_LOOP);


    // register callbacks
    glutDisplayFunc(display);
    glutKeyboardUpFunc(keyboard_start);
    glutSpecialUpFunc(special_keyboard_start);
    glutIdleFunc(idle);


    // enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}



//모든 맵에서 작동 가능
void Player_Drop()
{
    if (player.getCenterY() < -boundaryY - 100)
    {
        //위쪽에서 떨어지기
        player.setCenterY(-player.getCenterY());
        playerimage_left.setposition(player.getCenterX(), player.getCenterY(), PLAYER_SIZE, PLAYER_SIZE);
        playerimage_right.setposition(player.getCenterX(), player.getCenterY(), PLAYER_SIZE, PLAYER_SIZE);
    }

    if (!jump_move)
    {
        for (int i = 0; i < maps.size(); i++)
        {
            if (i != 1 && i != 2)
            {
                if (player.getCenterX() - player.getSize() / 2 < maps[i].getCenterX() + maps[i].getWidth() / 2
                    && player.getCenterX() + player.getSize() / 2 > maps[i].getCenterX() - maps[i].getWidth() / 2)
                {
                    //수정
                    if (player.getCenterY() - player.getSize() / 2 < maps[i].getCenterY() + maps[i].getHeight() / 2 - drop_v
                        && player.getCenterY() - player.getSize() / 2 >= maps[i].getCenterY() + maps[i].getHeight() / 2)
                    {
                        if (player_drop)
                        {
                            player.slide_v_reset();
                            slide = true;
                        }
                        player_drop = false;
                        player.setCenterY(maps[i].getCenterY() + maps[i].getHeight() / 2 + player.getSize() / 2);
                        playerimage_right.jump(player.getCenterY());
                        playerimage_left.jump(player.getCenterY());
                        break;
                    }
                }
            }
            if (i == maps.size() - 1)
            {
                player_drop = true;
            }
        }
        if (player_drop)
        {
            //떨어뜨리기  
            player.drop();
            playerimage_right.jump(player.getCenterY());
            playerimage_left.jump(player.getCenterY());

        }
    }
}

void monster_move()
{
    if (State == state::stage1)
    {
        for (int i = 0; i < monsters.size(); i++)
        {
            if (monsters[i].get_move() == true)
            {

                if (monsters[i].getCenterX() - monsters[i].getSize() / 2 <= maps[3 + 3 * i].getCenterX() - maps[3 + 3 * i].getWidth() / 2
                    || monsters[i].getCenterX() + monsters[i].getSize() / 2 >= maps[3 + 3 * i].getCenterX() + maps[3 + 3 * i].getWidth() / 2)
                {
                    if (monsters[i].getTrap() == false)
                    {
                        monsters[i].set_v(-monsters[i].getmoving_v());
                        monsters[i].move();
                        monster_image[i][0].move(monsters[i].getCenterX());
                        monster_image[i][1].move(monsters[i].getCenterX());
                    }
                    else {

                        monster_image[i][0].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                        monster_image[i][1].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                    }
                }
                else
                {
                    if (monsters[i].getTrap() == false)
                    {
                        monsters[i].move();
                        monster_image[i][0].move(monsters[i].getCenterX());
                        monster_image[i][1].move(monsters[i].getCenterX());
                    }
                    else {

                        monster_image[i][0].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                        monster_image[i][1].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                    }
                }

            }
        }
    }
    else if (State == state::stage2)
    {
        for (int i = 0; i < monsters.size(); i++)
        {
            if (monsters[i].get_move() == true)
            {

                if (monsters[i].getCenterX() - monsters[i].getSize() / 2 <= maps[4 + i].getCenterX() - maps[4 + i].getWidth() / 2
                    || monsters[i].getCenterX() + monsters[i].getSize() / 2 >= maps[4 + i].getCenterX() + maps[4 + i].getWidth() / 2)
                {
                    if (monsters[i].getTrap() == false)
                    {
                        monsters[i].set_v(-monsters[i].getmoving_v());
                        monsters[i].move();
                        monster_image[i][0].move(monsters[i].getCenterX());
                        monster_image[i][1].move(monsters[i].getCenterX());
                    }
                    else {

                        monster_image[i][0].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                        monster_image[i][1].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                    }
                }
                else
                {
                    if (monsters[i].getTrap() == false)
                    {
                        monsters[i].move();
                        monster_image[i][0].move(monsters[i].getCenterX());
                        monster_image[i][1].move(monsters[i].getCenterX());
                    }
                    else {

                        monster_image[i][0].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                        monster_image[i][1].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                    }
                }

            }
        }

    }
    else if (State == state::stage3)
    {
        for (int i = 0; i < monsters.size(); i++)
        {
            int k = 0;
            if (i == 2)
            {
                k = i;
            }
            if (monsters[i].get_move() == true)
            {

                if (monsters[i].getCenterX() - monsters[i].getSize() / 2 <= maps[9 + k].getCenterX() - maps[9 + k].getWidth() / 2
                    || monsters[i].getCenterX() + monsters[i].getSize() / 2 >= maps[9 + k].getCenterX() + maps[9 + k].getWidth() / 2)
                {
                    if (monsters[i].getTrap() == false)
                    {
                        monsters[i].set_v(-monsters[i].getmoving_v());
                        monsters[i].move();
                        monster_image[i][0].move(monsters[i].getCenterX());
                        monster_image[i][1].move(monsters[i].getCenterX());
                    }
                    else {

                        monster_image[i][0].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                        monster_image[i][1].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                    }
                }
                else
                {
                    if (monsters[i].getTrap() == false)
                    {
                        monsters[i].move();
                        monster_image[i][0].move(monsters[i].getCenterX());
                        monster_image[i][1].move(monsters[i].getCenterX());
                    }
                    else {

                        monster_image[i][0].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                        monster_image[i][1].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                    }
                }

            }
        }
    }
    if (State == state::stage4 || State == state::stage5 || State == state::stage6) 
    {
        vector<int> k = { 6, 6, 6, 5, 11, 12 };
        
        switch (State)
        {            
        case state::stage5:
            k[0] = 8;
            k[1] = 8;
            k[2] = 8;            
            k[4] = 9;            
            break;
        case state::stage6:
            k[0] = 0;
            k[1] = 3;
            k[2] = 12;
            k[3] = 16;
            k[4] = 18;
            k[5] = 19;
            k.push_back(21);
            k.push_back(23);
            break;        
        default:
            break;
        }
        

        for (int i = 0; i < monsters.size(); i++)
        {
            if (monsters[i].get_move() == true)
            {

                if (monsters[i].getCenterX() - monsters[i].getSize() / 2 <= maps[k[i]].getCenterX() - maps[k[i]].getWidth() / 2
                    || monsters[i].getCenterX() + monsters[i].getSize() / 2 >= maps[k[i]].getCenterX() + maps[k[i]].getWidth() / 2)
                {
                    if (monsters[i].getTrap() == false)
                    {
                        monsters[i].set_v(-monsters[i].getmoving_v());
                        monsters[i].move();
                        monster_image[i][0].move(monsters[i].getCenterX());
                        monster_image[i][1].move(monsters[i].getCenterX());
                    }
                    else {

                        monster_image[i][0].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                        monster_image[i][1].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                    }
                }
                else
                {
                    if (monsters[i].getTrap() == false)
                    {
                        monsters[i].move();
                        monster_image[i][0].move(monsters[i].getCenterX());
                        monster_image[i][1].move(monsters[i].getCenterX());
                    }
                    else {

                        monster_image[i][0].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                        monster_image[i][1].setposition(bubbles[monsters[i].get_trap_bubble_num()].getCenterX(), bubbles[monsters[i].get_trap_bubble_num()].getCenterY(), BLOCK / 1.3, BLOCK / 1.3);
                    }
                }

            }
        }
    }

}

void bubbles_chain(int num)
{
    for (int i = 0; i < bubbles.size(); i++)
    {
        if (bubbles[num].getCenterX() - bubbles[i].getCenterX() <= 2 * bubbles[num].getRadius()
            && bubbles[num].getCenterX() - bubbles[i].getCenterX() >= -2 * bubbles[num].getRadius())
        {
            if (bubbles[num].getCenterY() - bubbles[i].getCenterY() <= 2 * bubbles[num].getRadius()
                && bubbles[num].getCenterY() - bubbles[i].getCenterY() >= -2 * bubbles[num].getRadius())
            {
                if (!bubbles[i].get_pop_out() && bubbles[i].get_up())
                {
                    bubbles[i].set_pop_out(true);

                    for (int j = 0; j < monsters.size(); j++)
                    {
                        if (monsters[j].get_trap_bubble_num() == i)
                        {
                            monster_image[j][0].setposition(boundaryX + 100, boundaryY + 100, BLOCK, BLOCK);
                            monster_image[j][1].setposition(boundaryX + 100, boundaryY + 100, BLOCK, BLOCK);
                            //몬스터 이미지 날려버리기
                            monsters[j].setTrap(false);

                            stage_clear[j] = true;
                        }
                    }

                    bubbles_chain(i);
                }
            }
        }
    }
}
