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

MyJet myjet = MyJet(Point(0,-800), 20);
vector <JetLvl1> Enemies1 = {JetLvl1(Point(((rand() % 1800) - 900), 700), 5)};
vector <JetLvl2> Enemies2 = {JetLvl2(Point(((rand() % 1800) - 900), 700), 7)};

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
    GAME_MODE_BOSS,
    LOSE_STATE,
    WIN_STATE
};
MenuState currentMenuState = MAIN;

Button Start(-100, 10, 200, 80, "START", 1);
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
                myjet = MyJet(Point(0, -700));
                currentMenuState = MAIN;
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
                myjet = MyJet(Point(0, -700));
                currentMenuState = MAIN;
                glutPostRedisplay();
            }
            else {
                Bullet a(15, { 1.0,0.0,0.0 }, myjet.MainDot);
                MyBullets.push_back(a);
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
    //DrawOs();
    Start.setColor(0.0, 1.0, 0.0);
    Start.set_text_colors(0.0, 0.0, 0.0);
    Exit_Game.setColor(1.0, 0.0, 0.0);
    Exit_Game.set_text_colors(0.0, 0.0, 0.0);
    Button Health(-900, -980, 175, 80, "HP " + to_string(myjet.HP), 1);
    Button Kills(-700, -980, 250, 80, "Kills: " + to_string(EnemyLvl1count + EnemyLvl2count), 1);
    Health.setColor(0.0, 1.0, 0.0);
    Kills.setColor(0.0, 0.0, 1.0);
    bool flag = false;

    switch (currentMenuState)
    {
    case MAIN:
        Start.drawButton();
        Exit_Game.drawButton();
        myjet.draw();     
        break;
    case GAME_MODE_1:
        for (int i = 0; i < MyBullets.size(); ++i) {
            MyBullets[i].move_bullet(15);
            for (int j = 0; j < Enemies1.size(); ++j) {
                if (MyBullets.size() != 0) {
                    if ((MyBullets[i].center.y > 1010 || MyBullets[i].center.y < -1010)) {
                        MyBullets.erase(MyBullets.begin() + i);
                        glutPostRedisplay();
                    }
                    else if (Enemies1[j].is_hit(MyBullets[i].center)) {
                        cout << "Popadaniye vo vraga" << endl;
                        Enemies1[j].HP--;
                        cout << "Enemy has " << Enemies1[j].HP << " health" << endl;
                        if (Enemies1[j].HP == 0) {
                            Enemies1.erase(Enemies1.begin() + j);
                            MyBullets.erase(MyBullets.begin() + i);
                            EnemyLvl1count++;
                            if (EnemyLvl1count == 5) {
                                currentMenuState = GAME_MODE_2;
                                glutPostRedisplay();
                            }
                            break;
                        }
                        if (MyBullets.size() != 0) {
                            MyBullets.erase(MyBullets.begin() + i);
                        }
                        glutPostRedisplay();


                    }
                }
                else {
                    flag = true;
                    break; 
                }
            }
            if (flag) { break; }
        }
        for (int i = 0; i < EnemyBullets.size(); ++i) {
            EnemyBullets[i].move_bullet(-15);
            if (myjet.is_hit(EnemyBullets[i].center)) {
                myjet.HP--;
                EnemyBullets.erase(EnemyBullets.begin() + i);
                glutPostRedisplay();
            }
            else if ((EnemyBullets[i].center.y > 1000 || EnemyBullets[i].center.y < -1000)) {
                EnemyBullets.erase(EnemyBullets.begin() + i);
            }

        }
        for (int i = 0; i < Enemies1.size(); ++i) {
            if (Enemies1[i].count_to_shot == 60) {
                EnemyBullets.push_back(Bullet(15, { 0.0,0.0,1.0 }, Enemies1[i].MainDot));
                Enemies1[i].count_to_shot = 0;
            }
            if (myjet.is_intersect(Enemies1[i])) {
                cout << "Proizoshlo stolknoveniye, vi proigraly" << endl;
                currentMenuState = LOSE_STATE;
            }
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
        if (myjet.HP == 0) {
            currentMenuState = LOSE_STATE;
            glutPostRedisplay();
        }
        Exit_Game.drawButton();
        Health.drawButton();
        Kills.drawButton();
        myjet.draw();
        glutPostRedisplay();
        break;
    case GAME_MODE_2:
        for (int i = 0; i < MyBullets.size(); ++i) {
            MyBullets[i].move_bullet(15);
            for (int j = 0; j < Enemies1.size(); ++j) {
                if (MyBullets.size() != 0) {
                    /*if ((MyBullets[i].center.y > 1010 || MyBullets[i].center.y < -1010)) {
                        MyBullets.erase(MyBullets.begin() + i);
                        glutPostRedisplay();
                    }*/
                    if (Enemies1[j].is_hit(MyBullets[i].center)) {
                        cout << "Popadaniye vo vraga" << endl;
                        Enemies1[j].HP--;
                        cout << "Enemy has " << Enemies1[j].HP << " health" << endl;
                        if (Enemies1[j].HP == 0) {
                            Enemies1.erase(Enemies1.begin() + j);
                            MyBullets.erase(MyBullets.begin() + i);
                            EnemyLvl1count++;
                            if (EnemyLvl1count == 5) {
                                currentMenuState = GAME_MODE_1;
                                EnemyLvl1count = 0;
                                glutPostRedisplay();
                            }
                            break;
                        }
                        if (MyBullets.size() != 0) {
                            MyBullets.erase(MyBullets.begin() + i);
                        }
                        glutPostRedisplay();
                    }
                }
                else {
                    flag = true;
                    break;
                }
            }
            if (flag) { break; }
            for (int j = 0; j < Enemies2.size(); ++j) {
                if (MyBullets.size() != 0) {
                    /*if ((MyBullets[i].center.y > 2000 || MyBullets[i].center.y < -2000)) {
                        MyBullets.erase(MyBullets.begin() + i);
                        glutPostRedisplay();
                    }*/
                    if (Enemies2[j].is_hit(MyBullets[i].center)) {
                        cout << "Popadaniye vo vraga" << endl;
                        Enemies2[j].HP--;
                        cout << "Enemy has " << Enemies2[j].HP << " health" << endl;
                        if (Enemies2[j].HP == 0) {
                            Enemies2.erase(Enemies2.begin() + j);
                            MyBullets.erase(MyBullets.begin() + i);
                            EnemyLvl2count++;
                            if (EnemyLvl2count == 5) {
                                currentMenuState = GAME_MODE_BOSS;
                                glutPostRedisplay();
                            }
                            break;
                        }
                        if (MyBullets.size() != 0) {
                            MyBullets.erase(MyBullets.begin() + i);
                        }
                        glutPostRedisplay();
                    }
                }
                else {
                    flag = true;
                    break;
                }
            }
            if (flag) { break; }
        }
        for (int i = 0; i < EnemyBullets.size(); ++i) {
            EnemyBullets[i].move_bullet(-15);
            if (myjet.is_hit(EnemyBullets[i].center)) {
                myjet.HP--;
                EnemyBullets.erase(EnemyBullets.begin() + i);
                glutPostRedisplay();
            }
            else if ((EnemyBullets[i].center.y > 1000 || EnemyBullets[i].center.y < -1000)) {
                EnemyBullets.erase(EnemyBullets.begin() + i);
            }

        }
        for (int i = 0; i < Enemies1.size(); ++i) {
            if (Enemies1[i].count_to_shot == 60) {
                EnemyBullets.push_back(Bullet(15, { 0.0,0.0,1.0 }, Enemies1[i].MainDot));
                Enemies1[i].count_to_shot = 0;
            }
            if (myjet.is_intersect(Enemies1[i])) {
                cout << "Proizoshlo stolknoveniye, vi proigraly" << endl;
                currentMenuState = LOSE_STATE;
            }
        }
        
        for (int i = 0; i < Enemies2.size(); ++i) {
            if (Enemies2[i].count_to_shot == 40) {
                EnemyBullets.push_back(Bullet(15, { 0.0,0.0,1.0 }, Enemies2[i].MainDot));
                Enemies2[i].count_to_shot = 0;
            }
            if (myjet.is_intersect(Enemies2[i])) {
                cout << "Proizoshlo stolknoveniye, vi proigraly 2" << endl;
                
            }
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
        for (int i = 0; i < Enemies2.size(); i++) {
            Enemies2[i].draw();
            Button EnemyHP(Enemies2[i].MainDot.x - 40, Enemies2[i].MainDot.y + 210, 80, 60, "HP:" + to_string(Enemies2[i].HP), 2);
            EnemyHP.setColor(0.56, 0.2, 0);
            EnemyHP.set_text_colors(1.0, 1.0, 1.0);
            EnemyHP.drawButton();
        }
        if (myjet.HP == 0) {
            currentMenuState = LOSE_STATE;
            glutPostRedisplay();
        }
        Exit_Game.drawButton();
        Health.drawButton();
        Kills.drawButton();
        myjet.draw();
        glutPostRedisplay();
        break;
    case LOSE_STATE:
        break;
    case WIN_STATE:
        break;
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
                Enemies1[i].MovePoint = Point(rand() % 20 - 10, rand() % 8 - 4);
                Enemies1[i].direction_change = 0;
            }
            if (Enemies1[i].MainDot.x < -900 || Enemies1[i].MainDot.x > 900 || Enemies1[i].MainDot.y > 950) {
                Enemies1[i].MovePoint = Point(rand() % 20 - 10, rand() % 8 - 4);
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
                Enemies1[i].MovePoint = Point(rand() % 20 - 10, rand() % 8 - 4);
                Enemies1[i].direction_change = 0;
            }
            if (Enemies1[i].MainDot.x < -900 || Enemies1[i].MainDot.x > 900 || Enemies1[i].MainDot.y > 950) {
                Enemies1[i].MovePoint = Point(rand() % 20 - 10, rand() % 8 - 4);
                Enemies1[i].direction_change = 0;
            }
        }
        for (int i = 0; i < Enemies2.size(); ++i) {
            Enemies2[i].move_Jet(Enemies2[i].MovePoint);
            Enemies2[i].count_to_shot++;
            Enemies2[i].direction_change++;
            if (Enemies2[i].direction_change == 25) {
                Enemies2[i].MovePoint = Point(rand() % 30 - 15, rand() % 10 - 5);
                Enemies2[i].direction_change = 0;
            }
            if (Enemies2[i].MainDot.x < -900 || Enemies2[i].MainDot.x > 900 || Enemies2[i].MainDot.y > 950) {
                Enemies2[i].MovePoint = Point(rand() % 20 - 10, rand() % 8 - 4);
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