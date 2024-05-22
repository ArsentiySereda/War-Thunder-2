#pragma once
#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include "glut.h" //необходимо указть путь к папке с файлом
#include <math.h>
#include "Point.h"
using namespace std;

struct FigterJet {
	int HP;
	Point MainDot;

	FigterJet() : HP(5), MainDot(Point(0, -500)) {};
	FigterJet(const Point& MainDot) : HP(5), MainDot(MainDot) {};
	virtual void draw() = 0;
};

struct MyJet : public FigterJet{
	MyJet() : FigterJet() {};
	
	void draw() override {

		glColor3f(0.26, 0.66, 1.0);
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x - 21, MainDot.y - 105);
		glVertex2f(MainDot.x - 147, MainDot.y - 255);
		glVertex2f(MainDot.x - 21, MainDot.y - 255);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x + 21, MainDot.y - 105);
		glVertex2f(MainDot.x + 147, MainDot.y - 255);
		glVertex2f(MainDot.x + 21, MainDot.y - 255);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x -105, MainDot.y - 330);
		glVertex2f(MainDot.x -42, MainDot.y - 270);
		glVertex2f(MainDot.x -42, MainDot.y - 330);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x + 105, MainDot.y - 330);
		glVertex2f(MainDot.x + 42, MainDot.y - 270);
		glVertex2f(MainDot.x + 42, MainDot.y - 330);
		glEnd();

		glColor3f(0.6, 0.6, 0.6);
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x - 42, MainDot.y - 300);
		glVertex2f(MainDot.x - 28, MainDot.y - 340);
		glVertex2f(MainDot.x - 16, MainDot.y - 340);
		glVertex2f(MainDot.x - 0, MainDot.y - 300);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x + 42, MainDot.y - 300);
		glVertex2f(MainDot.x + 28, MainDot.y - 340);
		glVertex2f(MainDot.x + 16, MainDot.y - 340);
		glVertex2f(MainDot.x + 0, MainDot.y - 300);
		glEnd();

		glColor3f(0.0, 0.20, 0.6);
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x - 0, MainDot.y + 30);
		glVertex2f(MainDot.x - 42, MainDot.y - 156);
		glVertex2f(MainDot.x - 42, MainDot.y - 300);
		glVertex2f(MainDot.x + 42, MainDot.y - 300);
		glVertex2f(MainDot.x + 42, MainDot.y - 156);
		glEnd();

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x - 0, MainDot.y + 0);
		glVertex2f(MainDot.x - 13, MainDot.y - 60);
		glVertex2f(MainDot.x + 13, MainDot.y - 60);
		glEnd();
	}

	void move_MyJet(double dx, double dy) {
		MainDot = MainDot + Point(dx, dy);
	}

	void move_MyJet(const Point& MoveDot) {
		MainDot = MainDot + MoveDot;
	}
};