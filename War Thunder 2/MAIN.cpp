#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include "glut.h" //необходимо указть путь к папке с файлом
#include <math.h>
using namespace std;

GLfloat windowWidth = 10;
GLfloat windowHeight = 10;
bool equal(double first, double second) {
    return abs(first - second) < 0.001;
}

double deg_to_rad(double angle) {
    return (angle / 180) * M_PI;
}

double rad_to_deg(double angle) {
    return (angle / M_PI) * 180;
}

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

struct Point {
    double x;
    double y;

    Point() : x(0.), y(0.) {}

    Point(double x, double y) : x(x), y(y) {}

    Point(Point first, Point second) {
        x = first.x + second.x;
        x /= 2;
        y = first.y + second.y;
        y /= 2;
    }

    void rotate(const Point& center, double rad) {
        double x_2 = center.x + cos(rad) * (x - center.x) - sin(rad) * (y - center.y);
        double y_2 = center.y + sin(rad) * (x - center.x) + cos(rad) * (y - center.y);
        x = x_2;
        y = y_2;
    }

    void scale(const Point& center, double coe) {
        x = (x - center.x) * coe + center.x;
        y = (y - center.y) * coe + center.y;
    }

    void reflect(const Point& center) {
        scale(center, -1);
    }

    void drawPoint() {
        // Установка толщины линии
        glPointSize(3);
        glBegin(GL_POINTS);
        glColor3f(0.0, 0.0, 0.0); // Белый цвет
        glVertex2f(x, y);
        glEnd();
    }
};

double operator*(const Point first, const Point second) {
    return first.x * second.x + first.y * second.y;
}

Point operator+(const Point first, const Point second) {
    return { first.x + second.x, first.y + second.y };
}

Point operator-(const Point first, const Point second) {
    return { first.x - second.x, first.y - second.y };
}

bool operator==(const Point& first, const Point& second) {
    return equal(first.x, second.x) && equal(first.y, second.y);
}

bool operator!=(const Point& first, const Point& second) { return !(first == second); }

double dist(const Point& first, const Point& second) {
    return sqrt(pow((first.x - second.x), 2) + pow((first.y - second.y), 2));
}


class Button {
private:
    double x;
    double y;
    double width;
    double height;
    string text;
public:
    Button() : x(0), y(0), width(0), height(0), text("") {};
    Button(double x, double y, double width, double height, string text)
        : x(x), y(y), width(width), height(height), text(text) {};

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
        int textLength = text.length();
        float textX = x + 20;
        float textY = y + 25;
        glRasterPos2f(textX, textY);
        for (int i = 0; i < textLength; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
        }
    }

    bool isButtonHovered(Point& najatie) {
        return najatie.x >= this->x && najatie.x <= this->x + this->width &&
            najatie.y >= this->y && najatie.y <= this->y + this->height;
    }

};

enum MenuState
{
    MAIN,
    GAME_MODE_1
};
MenuState currentMenuState = MAIN;

Button Start(-100, 10, 200, 80, "START");
Button Exit_Game(-150, -120, 300, 80, "EXIT GAME");

void mouseClickHandler(int button, int state, int x, int y) {
    // Проверка нажатия левой кнопки мыши
    setlocale(LC_ALL, "rus");
    double X = (x - 500) * 2, Y = 1000 - (y * 2);

    Point najatie(X, Y);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
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

            break;
        }
    }
}


//**********************************************************
// Рисование сцены
void RenderScene(void) {
    setlocale(LC_ALL, "rus");
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // Цвет фона окна
    glClear(GL_COLOR_BUFFER_BIT);
    DrawOs();
    Start.setColor(0.0, 1.0, 0.0);
    Start.set_text_colors(0.0, 0.0, 0.0);
    Exit_Game.setColor(1.0, 0.0, 0.0);
    Exit_Game.set_text_colors(0.0, 0.0, 0.0);

    switch (currentMenuState)
    {
    case MAIN:
        Start.drawButton();
        Start.drawButton();
        Exit_Game.drawButton();
        break;
    case GAME_MODE_1:
        break;
    default:
        break;
    }
    glutSwapBuffers();

}

//**********************************************************
// Вызывается по таймеру библиотекой GLUT в спокойном состоянии,
// когда не меняются размеры окна приложения и нет перемещений параметр - номер таймера
//void TimerFunction(int value) {
//
//	//Перерисовываем сцену с новыми координатами
//	glutPostRedisplay();
//	glutTimerFunc(10, TimerFunction, 1);
//}
void Timer(int value) {

    //glutTimerFunc(0.00001, TimerFunction, 1);
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
    glutCreateWindow("Template"); // Создание окна, его название
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutMouseFunc(mouseClickHandler);
    srand(time(NULL));

    SetupRC();
    glutMainLoop();

}


///* Обрабатывает сообщения от клавиатуры */
//void keyboardFunc(unsigned char key, int x, int y)
//{
//   
//  if( key == 27  )
//	exit(0);
//	
//}
///* Обрабатывает сообщения от клавиатуры функциональные клавиши */
//void SkeyboardFunc(int key, int x, int y)
//{
//
//	 if( key == GLUT_KEY_UP  ){ xstep = 0;  ystep = 3;}
//	 if( key == GLUT_KEY_LEFT  ){ xstep = -3;  ystep = 0;}
//	if( key == GLUT_KEY_DOWN  ){ xstep = 0;  ystep = -3;}
//	if( key == GLUT_KEY_RIGHT  ){ xstep = 3;  ystep = 0;}
//	
//}
///* Обрабатывает сообщения от мыши */
//void Mouse(int button, int state, int x, int y)
//{
//  if( state == GLUT_DOWN ) {
//    switch( button ) {
//      case GLUT_LEFT_BUTTON:
//        Seed = random(RAND_MAX);
//        break;
//      case GLUT_RIGHT_BUTTON:
//        FillFlag = !FillFlag;
//        break;
//    }
//    glutPostRedisplay();
//  }
//}