#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include "glut.h" //необходимо указть путь к папке с файлом
#include <math.h>
#include "Planes.h"
#include "Point.h"
using namespace std;
// Прототипы глобальных функций
void RenderScene(void);								//вызывается при перерисовке экрана
void SetupRC(void);									//начальные установки
void ChangeSize(int, int);							//вызывается при изменении размеров окна
void TimerFunction(int); //работа с таймером
void keyboardFunc(unsigned char , int , int );		//обработка клавиатуры
//void SkeyboardFunc(int , int , int );				//обработка клавиатуры - функциональные клавиши
//void Mouse(int button, int state, int x, int y);	//обработка мыши
// Глобальные переменные:

GLfloat windowWidth = 10;
GLfloat windowHeight = 10;
int count_to_create = 0;
int EnemyLvl1count = 0;
int EnemyLvl2count = 0;

void DrawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);

    glEnd();
}

void DrawOs() {

    DrawLine(1000, 0, -1000, 0);
    DrawLine(0, 1000, 0, -1000);
    DrawLine(1000, 0, 950, 30);
    DrawLine(1000, 0, 950, -30);
    DrawLine(0, 1000, -30, 950);
    DrawLine(0, 1000, 30, 950);
    for (int i = -950; i <= 950; i += 50) {
        DrawLine(i, 15, i, -15);
    }
    for (int i = -950; i <= 950; i += 50) {
        DrawLine(15, i, -15, i);
    }
}

MyJet myjet = MyJet(Point(0,-800), 30);
vector <JetLvl1> Enemies1;
vector <JetLvl2> Enemies2;
BOSS boss = BOSS(Point(0, 1100),20);

class Button {
private:
    double x;
    double y;
    double width;
    double height;
    string text;
    int text_type;
public:
    Button() : x(0), y(0), width(0), height(0), text(""), text_type(1) {};
    Button(double x, double y, double width, double height, string text, int text_type)
        : x(x), y(y), width(width), height(height), text(text),  text_type(text_type) {};

    vector <double> colors = {0.5, 0.5, 0.5};
    void setColor(double first, double second, double third) {
        colors = { first, second, third };
    }
    vector <double> text_colors = { 0.0, 0.0, 0.0 };
    void set_text_colors(double first, double second, double third) {
        text_colors = { first, second, third };
    }

    void drawButton() {

        //Отрисовка кнопки
        glBegin(GL_POLYGON);
        glColor3f(colors[0], colors[1], colors[2]); // Серый цвет
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
        glColor3f(text_colors[0], text_colors[1], text_colors[2]); // Серый цветw
        if (text_type == 1) {
            int textLength = text.length();
            float textX = x + 20;
            float textY = y + 25;
            glRasterPos2f(textX, textY);
            for (int i = 0; i < textLength; i++) {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
            }
        }
        else if (text_type == 2) {
            int textLength = text.length();
            float textX = x + 10;
            float textY = y + 15;
            glRasterPos2f(textX, textY);
            for (int i = 0; i < textLength; i++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
            }
        }
    }

    bool isButtonHovered(Point& najatie) {
        return najatie.x >= this->x && najatie.x <= this->x + this->width &&
            najatie.y >= this->y && najatie.y <= this->y + this->height;
    }

};

vector <Bullet> MyBullets;
vector <Bullet> EnemyBullets;
enum MenuState
{
    MAIN,
    GAME_MODE_1,
    GAME_MODE_2,
    GAME_MODE_BOSS_ENTRANCE,
    GAME_MODE_BOSS_FIGHT,
    LOSE_STATE,
    WIN_STATE
};
MenuState currentMenuState = MAIN;

Button Start(-100, 10, 200, 80, "START", 1);
Button TryAgain(-155, -190, 310, 80, "TRY AGAIN", 1);
Button PlayAgain (-165, -190, 330, 80, "PLAY AGAIN", 1);
Button Exit_Game(690, -980, 300, 80, "EXIT GAME", 1);
Point MovePoint(0.0,0.0);

void mouseClickHandler(int button, int state, int x, int y) {
    // Проверка нажатия левой кнопки мыши
    setlocale(LC_ALL, "rus");
    double X = (x - 500) * 2, Y = 1000 - (y * 2);

    Point najatie = Point(X, Y);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        switch (currentMenuState)
        {
        case MAIN:
            if (Start.isButtonHovered(najatie)) {
                currentMenuState = GAME_MODE_1;
                glutPostRedisplay();
            }
            if (Exit_Game.isButtonHovered(najatie)) {
                exit(0);
            }
            break;
        case GAME_MODE_1:
            if (Exit_Game.isButtonHovered(najatie)) {
                MovePoint = Point(0, 0);
                myjet.to_default();
                Enemies1.erase(Enemies1.begin(), Enemies1.end());
                Enemies2.erase(Enemies2.begin(), Enemies2.end());
                MyBullets.erase(MyBullets.begin(), MyBullets.end());
                EnemyBullets.erase(EnemyBullets.begin(), EnemyBullets.end());
                boss = BOSS(Point(0, 1100), 20);
                glutPostRedisplay();
            }
            else {
                Bullet a(15, { 1.0,0.0,0.0 }, myjet.MainDot);
                MyBullets.push_back(a);
                glutPostRedisplay();
            }
            break;
        case GAME_MODE_2:
            if (Exit_Game.isButtonHovered(najatie)) {
                MovePoint = Point(0, 0);
                myjet.to_default();
                Enemies1.erase(Enemies1.begin(), Enemies1.end());
                Enemies2.erase(Enemies2.begin(), Enemies2.end());
                MyBullets.erase(MyBullets.begin(), MyBullets.end());
                EnemyBullets.erase(EnemyBullets.begin(), EnemyBullets.end());
                boss = BOSS(Point(0, 1100), 20);
                currentMenuState = MAIN;
                glutPostRedisplay();
            }
            else {
                Bullet a(15, { 1.0,0.0,0.0 }, myjet.MainDot);
                MyBullets.push_back(a);
                glutPostRedisplay();
            }
            break;
        case GAME_MODE_BOSS_ENTRANCE:

            break;
        case GAME_MODE_BOSS_FIGHT:
            if (Exit_Game.isButtonHovered(najatie)) {
                MovePoint = Point(0, 0);
                myjet = MyJet(Point(0, -800));
                Enemies1.erase(Enemies1.begin(), Enemies1.end());
                Enemies2.erase(Enemies2.begin(), Enemies2.end());
                MyBullets.erase(MyBullets.begin(), MyBullets.end());
                EnemyBullets.erase(EnemyBullets.begin(), EnemyBullets.end());
                boss = BOSS(Point(0, 1100), 20);
                currentMenuState = MAIN;
                glutPostRedisplay();
            }
            else {
                Bullet a(15, { 1.0,0.0,0.0 }, myjet.MainDot);
                MyBullets.push_back(a);
                glutPostRedisplay();
            }
            break;
        case LOSE_STATE:
            if (Exit_Game.isButtonHovered(najatie)) {
                exit(0);
            }
            else if(TryAgain.isButtonHovered(najatie)) {
                MovePoint = Point(0, 0);
                myjet.to_default();
                Enemies1.erase(Enemies1.begin(), Enemies1.end());
                Enemies2.erase(Enemies2.begin(), Enemies2.end());
                MyBullets.erase(MyBullets.begin(), MyBullets.end());
                EnemyBullets.erase(EnemyBullets.begin(), EnemyBullets.end());
                EnemyLvl1count = 0;
                EnemyLvl2count = 0;
                boss = BOSS(Point(0, 1100), 20);
                currentMenuState = MAIN;
                glutPostRedisplay();
            }
            break;
        case WIN_STATE:
            if (Exit_Game.isButtonHovered(najatie)) {
                exit(0);
            }
            else if (PlayAgain.isButtonHovered(najatie)) {
                MovePoint = Point(0, 0);
                myjet.to_default();
                Enemies1.erase(Enemies1.begin(), Enemies1.end());
                Enemies2.erase(Enemies2.begin(), Enemies2.end());
                MyBullets.erase(MyBullets.begin(), MyBullets.end());
                EnemyLvl1count = 0;
                EnemyLvl2count = 0;
                EnemyBullets.erase(EnemyBullets.begin(), EnemyBullets.end());
                boss = BOSS(Point(0, 1100), 20);
                currentMenuState = MAIN;
                glutPostRedisplay();
            }
            break;
        }
    }
}



//**********************************************************
// Рисование сцены
void RenderScene(void) {
    setlocale(LC_ALL, "rus");
    glClearColor(0.5f, 0.9f, 0.95f, 0.0f); // Цвет фона окна
    glClear(GL_COLOR_BUFFER_BIT);
    Start.setColor(0.0, 1.0, 0.0);
    Start.set_text_colors(0.0, 0.0, 0.0);
    Exit_Game.setColor(1.0, 0.0, 0.0);
    Exit_Game.set_text_colors(0.0, 0.0, 0.0);
    Button Health(-900, -980, 175, 80, "HP " + to_string(myjet.HP), 1);
    Button Kills(-700, -980, 250, 80, "Kills: " + to_string(EnemyLvl1count + EnemyLvl2count), 1);
    Button bossHP(boss.MainDot.x - 75, boss.MainDot.y + 150, 150, 80, "HP:" + to_string(boss.HP), 1);
    Button Lose(-135, 10, 270, 80, "YOU LOSE", 1);
    Button Win(-130, 10, 260, 80, "YOU WIN", 1);
    Lose.setColor(1.0,0.0,0.73);
    Win.setColor(1.0,0.0,0.73);
    Health.setColor(0.0, 1.0, 0.0);
    Kills.setColor(0.0, 0.0, 1.0);
    TryAgain.setColor(0.1,1.0,0.0);
    PlayAgain.setColor(0.1,1.0,0.0);
    bool flag = false;
    vector <int> delete_mybullets;
    vector <int> delete_enemybullets;
    vector <int> delete_enemy1;
    vector <int> delete_enemy2;
    switch (currentMenuState)
    {
    case MAIN:
        Start.drawButton();
        Exit_Game.drawButton();
        myjet.draw();     
        break;
    case GAME_MODE_1:
        if (currentMenuState == GAME_MODE_1) {
            for (int i = 0; i < MyBullets.size(); ++i) { //перемещаем все пули 
                MyBullets[i].move_bullet(15);
                if (MyBullets[i].center.y > 1050) { delete_mybullets.push_back(i); }
            }
            for (int i = 0; i < Enemies1.size(); ++i) {
                if (myjet.is_intersect(Enemies1[i])) {//проверка столкновения самолетов
                    currentMenuState = LOSE_STATE;
                    glutPostRedisplay();
                }
                if (Enemies1[i].count_to_shot == 60) {//стрельба каждого противника
                    EnemyBullets.push_back(Bullet(15, { 0.0,0.0,1.0 }, Enemies1[i].MainDot));
                    Enemies1[i].count_to_shot = 0;
                }
                for (int j = 0; j < MyBullets.size(); ++j) {//проверка попаданий по противникам
                    if (Enemies1[i].is_hit(MyBullets[j].center)) {
                        delete_mybullets.push_back(j);
                        Enemies1[i].HP--;
                        if (Enemies1[i].HP == 0) {
                            delete_enemy1.push_back(i);
                            EnemyLvl1count++;
                            if (EnemyLvl1count == 5) {
                                currentMenuState = GAME_MODE_2;
                                glutPostRedisplay();
                            }
                            break;
                        }
                    }
                }
            }
            for (int i = 0; i < delete_enemy1.size(); ++i) { // удаление пуль и противников
                Enemies1.erase(Enemies1.begin() + delete_enemy1[i]);
            }
            for (int i = 0; i < delete_mybullets.size(); ++i) {
                MyBullets.erase(MyBullets.begin() + delete_mybullets[i]);
            }
            for (int i = 0; i < EnemyBullets.size(); ++i) { //проверка попадания в мой самолет
                EnemyBullets[i].move_bullet(-15);
                if (EnemyBullets[i].center.y < -1050) { delete_enemybullets.push_back(i); }
                else if (myjet.is_hit(EnemyBullets[i].center)) {
                    delete_enemybullets.push_back(i);
                    myjet.HP--;
                    if (myjet.HP == 0) {
                        cout << "YOU LOSE" << endl;
                        currentMenuState = LOSE_STATE;
                        glutPostRedisplay();
                    }
                }
            }

            for (int i = 0; i < delete_enemybullets.size(); ++i) {
                EnemyBullets.erase(EnemyBullets.begin() + delete_enemybullets[i]);
            }

            for (int i = 0; i < MyBullets.size(); i++) {
                MyBullets[i].draw();
            }
            for (int i = 0; i < EnemyBullets.size(); i++) {
                EnemyBullets[i].draw();
            }
            for (int i = 0; i < Enemies1.size(); i++) {
                Enemies1[i].draw();
                Button EnemyHP(Enemies1[i].MainDot.x - 42, Enemies1[i].MainDot.y + 150, 82, 60, "HP:" + to_string(Enemies1[i].HP), 2);
                EnemyHP.setColor(0.15, 0.27, 0);
                EnemyHP.set_text_colors(1.0, 1.0, 1.0);
                EnemyHP.drawButton();
            }
            Exit_Game.drawButton();
            Health.drawButton();
            Kills.drawButton();
            myjet.draw();
            glutPostRedisplay();
            break;
        }
    case GAME_MODE_2:
        if (currentMenuState == GAME_MODE_2) {
            for (int i = 0; i < MyBullets.size(); ++i) { //перемещаем все пули 
                MyBullets[i].move_bullet(15);
                if (MyBullets[i].center.y > 1050) { delete_mybullets.push_back(i); }
            }
            for (int i = 0; i < Enemies1.size(); ++i) {
                if (myjet.is_intersect(Enemies1[i])) {//проверка столкновения самолетов
                    cout << "YOU LOSE" << endl;
                    currentMenuState = LOSE_STATE;
                    glutPostRedisplay();
                }
                if (Enemies1[i].count_to_shot == 60) {//стрельба каждого противника
                    EnemyBullets.push_back(Bullet(15, { 0.0,0.0,1.0 }, Enemies1[i].MainDot));
                    Enemies1[i].count_to_shot = 0;
                }
                for (int j = 0; j < MyBullets.size(); ++j) {//проверка попаданий по противникам
                    if (Enemies1[i].is_hit(MyBullets[j].center)) {
                        delete_mybullets.push_back(j);
                        Enemies1[i].HP--;
                        if (Enemies1[i].HP == 0) {
                            delete_enemy1.push_back(i);
                            EnemyLvl1count++;
                            break;
                        }
                    }
                }
            }
            for (int i = 0; i < Enemies2.size(); ++i) {
                if (myjet.is_intersect(Enemies2[i])) {//проверка столкновения самолетов
                    currentMenuState = LOSE_STATE;
                    glutPostRedisplay();
                }
                if (Enemies2[i].count_to_shot == 50) {//стрельба каждого противника
                    EnemyBullets.push_back(Bullet(15, { 0.0,0.0,1.0 }, Enemies2[i].MainDot));
                    Enemies2[i].count_to_shot = 0;
                }
                for (int j = 0; j < MyBullets.size(); ++j) {//проверка попаданий по противникам
                    if (Enemies2[i].is_hit(MyBullets[j].center)) {
                        delete_mybullets.push_back(j);
                        Enemies2[i].HP--;
                        if (Enemies2[i].HP == 0) {
                            delete_enemy2.push_back(i);
                            EnemyLvl2count++;
                            if (EnemyLvl2count == 5) {
                                currentMenuState = GAME_MODE_BOSS_ENTRANCE;
                                glutPostRedisplay();
                            }
                            break;
                        }
                    }
                }
            }
            for (int i = 0; i < delete_enemy1.size(); ++i) { // удаление пуль и противников
                Enemies1.erase(Enemies1.begin() + delete_enemy1[i]);
            }
            for (int i = 0; i < delete_enemy2.size(); ++i) { // удаление пуль и противников
                Enemies2.erase(Enemies2.begin() + delete_enemy2[i]);
            }
            for (int i = 0; i < delete_mybullets.size(); ++i) {
                MyBullets.erase(MyBullets.begin() + delete_mybullets[i]);
            }
            for (int i = 0; i < EnemyBullets.size(); ++i) { //проверка попадания в мой самолет
                EnemyBullets[i].move_bullet(-15);
                if (EnemyBullets[i].center.y < -1050) {
                    EnemyBullets.erase(EnemyBullets.begin() + i);
                    i--;
                }
            }
            delete_enemybullets.erase(delete_enemybullets.begin(), delete_enemybullets.end());
            for (int i = 0; i < EnemyBullets.size(); ++i) { //проверка попадания в мой самолет
                if (myjet.is_hit(EnemyBullets[i].center)) {
                    delete_enemybullets.push_back(i);
                    myjet.HP--;
                    if (myjet.HP == 0) {
                        cout << "YOU LOSE" << endl;
                        currentMenuState = LOSE_STATE;
                        glutPostRedisplay();
                    }
                }
            }
            for (int i = 0; i < delete_enemybullets.size(); ++i) {
                EnemyBullets.erase(EnemyBullets.begin() + delete_enemybullets[i]);
            }
            delete_enemybullets.erase(delete_enemybullets.begin(), delete_enemybullets.end());

            for (int i = 0; i < MyBullets.size(); i++) {
                MyBullets[i].draw();
            }
            for (int i = 0; i < EnemyBullets.size(); i++) {
                EnemyBullets[i].draw();
            }
            for (int i = 0; i < Enemies1.size(); i++) {
                Enemies1[i].draw();
                Button EnemyHP(Enemies1[i].MainDot.x - 42, Enemies1[i].MainDot.y + 150, 82, 60, "HP:" + to_string(Enemies1[i].HP), 2);
                EnemyHP.setColor(0.15, 0.27, 0);
                EnemyHP.set_text_colors(1.0, 1.0, 1.0);
                EnemyHP.drawButton();
            }
            for (int i = 0; i < Enemies2.size(); i++) {
                Enemies2[i].draw();
                Button EnemyHP(Enemies2[i].MainDot.x - 40, Enemies2[i].MainDot.y + 210, 80, 60, "HP:" + to_string(Enemies2[i].HP), 2);
                EnemyHP.setColor(0.56, 0.2, 0);
                EnemyHP.set_text_colors(1.0, 1.0, 1.0);
                EnemyHP.drawButton();
            }

            Exit_Game.drawButton();
            Health.drawButton();
            Kills.drawButton();
            myjet.draw();
            glutPostRedisplay();
            break;
        }
    case GAME_MODE_BOSS_ENTRANCE:
        if (currentMenuState == GAME_MODE_BOSS_ENTRANCE) {
            for (int i = 0; i < Enemies1.size(); i++) {
                Enemies1[i].draw();
            }
            for (int i = 0; i < Enemies2.size(); i++) {
                Enemies2[i].draw();
            }
            if (boss.MainDot.y < 460) {
                currentMenuState = GAME_MODE_BOSS_FIGHT;
                glutPostRedisplay();
            }
            myjet.draw();
            boss.draw();
            glutPostRedisplay();
            break;
        }
    case GAME_MODE_BOSS_FIGHT:
        if (currentMenuState == GAME_MODE_BOSS_FIGHT) {
            for (int i = 0; i < MyBullets.size(); ++i) { //перемещаем все пули 
                MyBullets[i].move_bullet(15);
                if (MyBullets[i].center.y > 1050) { delete_mybullets.push_back(i); }
            }
            for (int j = 0; j < MyBullets.size(); ++j) {//проверка попаданий по противникам
                if (boss.is_hit(MyBullets[j].center)) {
                    delete_mybullets.push_back(j);
                    boss.HP--;
                    cout << "BOSS HIT" << endl;
                    if (boss.HP == 0) {
                        currentMenuState = WIN_STATE;
                        cout << "YOU WIN" << endl;
                        glutPostRedisplay();
                        break;
                    }
                }
            }
            for (int i = 0; i < delete_mybullets.size(); ++i) {
                MyBullets.erase(MyBullets.begin() + delete_mybullets[i]);
            }
            for (int i = 0; i < EnemyBullets.size(); ++i) { //проверка попадания в мой самолет
                EnemyBullets[i].move_bullet(-15);
                if (EnemyBullets[i].center.y < -1050) { delete_enemybullets.push_back(i); }
            }
            for (int i = 0; i < delete_enemybullets.size(); ++i) {
                EnemyBullets.erase(EnemyBullets.begin() + delete_enemybullets[i]);
            }
            delete_enemybullets.erase(delete_enemybullets.begin(), delete_enemybullets.end());
            for (int i = 0; i < EnemyBullets.size(); ++i) { //проверка попадания в мой самолет
                if (myjet.is_hit(EnemyBullets[i].center)) {
                    delete_enemybullets.push_back(i);
                    myjet.HP--;
                    if (myjet.HP == 0) {
                        cout << "YOU LOSE" << endl;
                        currentMenuState = LOSE_STATE;
                        glutPostRedisplay();
                    }
                }
            }
            for (int i = 0; i < delete_enemybullets.size(); ++i) {
                EnemyBullets.erase(EnemyBullets.begin() + delete_enemybullets[i]);
            }
            delete_enemybullets.erase(delete_enemybullets.begin(), delete_enemybullets.end());
            if (myjet.is_intersect(boss)) { //проверка столкновения самолетов
                cout << "YOU LOSE" << endl;
                currentMenuState = LOSE_STATE;
                glutPostRedisplay();
            }
            boss.draw();
            bossHP.setColor(0.4, 0.0, 0.0);
            bossHP.set_text_colors(1.0, 1.0, 1.0);
            bossHP.drawButton();
            for (int i = 0; i < MyBullets.size(); i++) {
                MyBullets[i].draw();
            }
            for (int i = 0; i < EnemyBullets.size(); i++) {
                EnemyBullets[i].draw();
            }
            Exit_Game.drawButton();
            Health.drawButton();
            Kills.drawButton();
            myjet.draw();
            glutPostRedisplay();
            break;
        }
    case LOSE_STATE:
        if (currentMenuState == LOSE_STATE) {
            for (int i = 0; i < MyBullets.size(); i++) {
                MyBullets[i].draw();
            }
            for (int i = 0; i < EnemyBullets.size(); i++) {
                EnemyBullets[i].draw();
            }
            for (int i = 0; i < Enemies1.size(); i++) {
                Enemies1[i].draw();
                Button EnemyHP(Enemies1[i].MainDot.x - 42, Enemies1[i].MainDot.y + 150, 82, 60, "HP:" + to_string(Enemies1[i].HP), 2);
                EnemyHP.setColor(0.15, 0.27, 0);
                EnemyHP.set_text_colors(1.0, 1.0, 1.0);
                EnemyHP.drawButton();
            }
            for (int i = 0; i < Enemies2.size(); i++) {
                Enemies2[i].draw();
                Button EnemyHP(Enemies2[i].MainDot.x - 40, Enemies2[i].MainDot.y + 210, 80, 60, "HP:" + to_string(Enemies2[i].HP), 2);
                EnemyHP.setColor(0.56, 0.2, 0);
                EnemyHP.set_text_colors(1.0, 1.0, 1.0);
                EnemyHP.drawButton();
            }

            Health.drawButton();
            Lose.drawButton();
            Button TotalScore = Button(-165, -90, 330, 80, "Total Score: " + to_string(EnemyLvl1count + EnemyLvl2count), 1);
            TotalScore.setColor(0.1,0.0,1.0);
            TotalScore.drawButton();
            TryAgain.drawButton();
            Exit_Game.drawButton();
            boss.draw();
            myjet.draw();
            break;
        }
    case WIN_STATE:
        if (currentMenuState == WIN_STATE) {
            for (int i = 0; i < MyBullets.size(); i++) {
                MyBullets[i].draw();
            }
            for (int i = 0; i < EnemyBullets.size(); i++) {
                EnemyBullets[i].draw();
            }
            for (int i = 0; i < Enemies1.size(); i++) {
                Enemies1[i].draw();
                Button EnemyHP(Enemies1[i].MainDot.x - 42, Enemies1[i].MainDot.y + 150, 82, 60, "HP:" + to_string(Enemies1[i].HP), 2);
                EnemyHP.setColor(0.15, 0.27, 0);
                EnemyHP.set_text_colors(1.0, 1.0, 1.0);
                EnemyHP.drawButton();
            }
            for (int i = 0; i < Enemies2.size(); i++) {
                Enemies2[i].draw();
                Button EnemyHP(Enemies2[i].MainDot.x - 40, Enemies2[i].MainDot.y + 210, 80, 60, "HP:" + to_string(Enemies2[i].HP), 2);
                EnemyHP.setColor(0.56, 0.2, 0);
                EnemyHP.set_text_colors(1.0, 1.0, 1.0);
                EnemyHP.drawButton();
            }

            Health.drawButton();
            boss.draw();
            myjet.draw();
            Win.drawButton();
            Button TotalScore = Button(-165, -90, 330, 80, "Total Score: " + to_string(EnemyLvl1count + EnemyLvl2count + 1), 1);
            TotalScore.setColor(0.1, 0.0, 1.0);
            TotalScore.drawButton();
            PlayAgain.drawButton();
            Exit_Game.drawButton();
            break;
        }
    default:
        break;
    }
    glutSwapBuffers();

}

//**********************************************************
// Вызывается по таймеру библиотекой GLUT в спокойном состоянии,
// когда не меняются размеры окна приложения и нет перемещений параметр - номер таймера
void TimerFunction(int value) {
    if (currentMenuState == GAME_MODE_1) {
        myjet.move_Jet(MovePoint);
        for (int i = 0; i < Enemies1.size(); ++i) {
            Enemies1[i].move_Jet(Enemies1[i].MovePoint);
            Enemies1[i].count_to_shot++;
            Enemies1[i].direction_change++;
            if (Enemies1[i].direction_change == 25) {
                Enemies1[i].MovePoint = Point(rand() % 20 - 10, rand() % 6 - 4);
                Enemies1[i].direction_change = 0;
            }
            if (Enemies1[i].MainDot.x < -900 || Enemies1[i].MainDot.x > 900 || Enemies1[i].MainDot.y > 950) {
                Enemies1[i].MovePoint = Point(-Enemies1[i].MovePoint.x, Enemies1[i].MovePoint.y);
                Enemies1[i].direction_change = 0;
            }
        }
        count_to_create++;
        if (count_to_create == 200 || Enemies1.size() == 0) {
            Enemies1.push_back(JetLvl1(Point(((rand() % 1700)-850), 800)));
            if (count_to_create == 200) {
                count_to_create = 0;
            }
        }
    }
    if (currentMenuState == GAME_MODE_2) {
        myjet.move_Jet(MovePoint);
        for (int i = 0; i < Enemies1.size(); ++i) {
            Enemies1[i].move_Jet(Enemies1[i].MovePoint);
            Enemies1[i].count_to_shot++;
            Enemies1[i].direction_change++;
            if (Enemies1[i].direction_change == 25) {
                Enemies1[i].MovePoint = Point(rand() % 20 - 10, rand() % 6 - 5);
                Enemies1[i].direction_change = 0;
            }
            if (Enemies1[i].MainDot.x < -900 || Enemies1[i].MainDot.x > 900 || Enemies1[i].MainDot.y > 950) {
                Enemies1[i].MovePoint = Point(-Enemies1[i].MovePoint.x, Enemies1[i].MovePoint.y);
                Enemies1[i].direction_change = 0;
            }
        }
        for (int i = 0; i < Enemies2.size(); ++i) {
            Enemies2[i].move_Jet(Enemies2[i].MovePoint);
            Enemies2[i].count_to_shot++;
            Enemies2[i].direction_change++;
            if (Enemies2[i].direction_change == 25) {
                Enemies2[i].MovePoint = Point(rand() % 30 - 15, rand() % 12 - 7);
                Enemies2[i].direction_change = 0;
            }
            if (Enemies2[i].MainDot.x < -900 || Enemies2[i].MainDot.x > 900 || Enemies2[i].MainDot.y > 950) {
                Enemies2[i].MovePoint = Point(-Enemies2[i].MovePoint.x, Enemies2[i].MovePoint.y);
                Enemies2[i].direction_change = 0;
            }
        }
        count_to_create++;
        if (count_to_create == 150 || Enemies2.size() == 0) {
            Enemies2.push_back(JetLvl2(Point(((rand() % 1800) - 900), 800),7));
            if (count_to_create == 150) {
                count_to_create = 0;
            }
        }
    }
    if (currentMenuState == GAME_MODE_BOSS_ENTRANCE) {
        myjet.move_Jet(MovePoint);
        for (int i = 0; i < Enemies1.size(); ++i) {
            if (Enemies1[i].MainDot.x >= 0) { Enemies1[i].MovePoint = Point(15, -1); }
            else { Enemies1[i].MovePoint = Point(-15, -1); }
            Enemies1[i].move_Jet(Enemies1[i].MovePoint);
        }
        for (int i = 0; i < Enemies2.size(); ++i) {
            if (Enemies2[i].MainDot.x >= 0) { Enemies2[i].MovePoint = Point(15, -1); }
            else { Enemies2[i].MovePoint = Point(-15, -1); }
            Enemies2[i].move_Jet(Enemies2[i].MovePoint);
        }
        for (int i = 0; i < Enemies1.size(); ++i) {
            if (Enemies1[i].MainDot.x < -1300 || Enemies1[i].MainDot.x > 1300 ) {
                Enemies1.erase(Enemies1.begin() + i);
                break;
            }
        }
        for (int i = 0; i < Enemies2.size(); ++i) {
            if (Enemies2[i].MainDot.x < -1300 || Enemies2[i].MainDot.x > 1300) {
                Enemies2.erase(Enemies2.begin() + i);
                break;
            }
        }
        if (boss.MainDot.y > 455) {
            boss.move_Jet(Point(0, -5));
        }
        
    }
    if (currentMenuState == GAME_MODE_BOSS_FIGHT) {
        myjet.move_Jet(MovePoint);
        boss.move_Jet(boss.MovePoint);
        if (rand() % 45 == 1) {
            EnemyBullets.push_back(Bullet(15, { 0.0,0.0,1.0 }, Point(boss.MainDot.x - 500, boss.MainDot.y + 250)));
        }
        if (rand() % 45 == 1) {
            EnemyBullets.push_back(Bullet(15, { 0.0,0.0,1.0 }, Point(boss.MainDot.x + 500, boss.MainDot.y + 250)));
        }
        if (rand() % 45 == 1) {
            EnemyBullets.push_back(Bullet(15, { 0.0,0.0,1.0 }, Point(boss.MainDot.x - 162, boss.MainDot.y + 96)));
        }
        if (rand() % 45 == 1) {
            EnemyBullets.push_back(Bullet(15, { 0.0,0.0,1.0 }, Point(boss.MainDot.x + 162, boss.MainDot.y + 96)));
        }
        if (rand() % 45 == 1) {
            EnemyBullets.push_back(Bullet(15, { 0.0,0.0,1.0 }, Point(boss.MainDot.x - 0, boss.MainDot.y - 75)));
        }
        if (boss.MainDot.x < -750) {
            boss.MovePoint = Point(6, 0);
        }
        else if (boss.MainDot.x > 750) {
            boss.MainDot = Point(-6, 0);
        }
        else if(rand() % 40 == 1) {
            boss.MovePoint = Point(rand() % 30 - 15, 0);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(25, TimerFunction, 25);
}
void Timer(int value) {

    glutTimerFunc(25, TimerFunction, 25);
}

//**********************************************************
// Установка состояния рендеринга (визуализации)
void SetupRC(void)
{

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

}

//**********************************************************
// Вызывается библиотекой GLUT при изменении размеров окна
// приложения. Осталась такой же, как в примере GLRect,
// только в вызовы glOrtho() поставили переменные
void ChangeSize(int width, int height)
{
    GLfloat aspectRatio;

    // Предотвращаем деление на нуль
    if (height == 0)
        height = 1;

    // Устанавливаем поле просмотра с размерами окна
    glViewport(0, 0, width, height);
    
    // Устанавливает матрицу преобразования в режим проецирования
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-windowWidth, windowWidth, -windowHeight, windowHeight, 1.0, -1.0);
    // Устанавливаем размеры перспективы (отсекающего объема)
    // (left, right, bottom, top, near, far)
    aspectRatio = (GLfloat)width / (GLfloat)height;
    if (width <= height)
    {
        windowWidth = 100;
        windowHeight = 100 / aspectRatio;
        glOrtho(-100.0, 100.0,
            -windowHeight, windowHeight, 1.0, -1.0);
    }
    else
    {
        windowWidth = 100 * aspectRatio;
        windowHeight = 100;
        glOrtho(-windowWidth, windowWidth,
            -100.0, 100.0, 1.0, -1.0);
    }

    // Восстановливает матрицу преобразования в исходный режим вида
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000); // Размер окна
    glutCreateWindow("War Thunder 2"); // Создание окна, его название
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutMouseFunc(mouseClickHandler);
    glutKeyboardFunc(keyboardFunc);
    glutTimerFunc(25, TimerFunction, 25);
    srand(time(NULL));

    SetupRC();
    glutMainLoop();

}


/* Обрабатывает сообщения от клавиатуры */
void keyboardFunc(unsigned char key, int x, int y)
{
    if (currentMenuState != MAIN) {
        switch (key) {
        case 'w':
        case 'W':
            cout << "W is pressed" << endl;
            MovePoint = Point(0, 15);
            glutPostRedisplay();
            break;
        case 'a':
        case 'A':
            cout << "A is pressed" << endl;
            MovePoint = Point(-15, 0);
            glutPostRedisplay();
            break;
        case 's':
        case 'S':
            cout << "S is pressed" << endl;
            MovePoint = Point(0, -15);
            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            cout << "D is pressed" << endl;
            MovePoint = Point(15, 0);
            glutPostRedisplay();
            break;
        case 27: // Escape key
            exit(0);
            break;
        case 32:
            /*MyBullets.push_back(Bullet(15, { 1.0,0.0,0.0 }, myjet.MainDot));
            isKeyPressed = false;
            MovePoint = Point(0, 0);
            glutPostRedisplay();
            break;*/
        default:
            MovePoint = Point(0, 0);
            glutPostRedisplay();
            break;
        }
    }
}
/* Обрабатывает сообщения от клавиатуры функциональные клавиши */
//void SkeyboardFunc(int key, int x, int y)
//{
//
//	 if( key == GLUT_KEY_UP  ){ MovePoint = Point(0, 0); }
//	 if( key == GLUT_KEY_LEFT  ){ MovePoint = Point(0, 0); }
//	if( key == GLUT_KEY_DOWN  ){ MovePoint = Point(0, 0); }
//	if( key == GLUT_KEY_RIGHT  ){ MovePoint = Point(0, 0); }
//	
//}
///* Обрабатывает сообщения от мыши */
//void Mouse(int button, int state, int x, int y)
//{
//    if (currentMenuState != MAIN) {
//        setlocale(LC_ALL, "rus");
//        double X = (x - 500) * 2, Y = 1000 - (y * 2);
//        najatie = Point(X, Y);
//        
//        
//        
//    }
//}