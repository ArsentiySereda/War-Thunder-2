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
	FigterJet(const Point& MainDot, int HP) :  MainDot(MainDot), HP(HP) {};
	virtual void draw() = 0;
	virtual bool is_hit(const Point& bullet) = 0;
	void move_Jet(const Point& MoveDot) {
		MainDot = MainDot + MoveDot;
	}
};


struct JetLvl1 : public FigterJet
{
	int count_to_shot;
	Point MovePoint = Point(0, -3);
	int direction_change = 0;
	JetLvl1() : FigterJet(), count_to_shot(25) { };
	JetLvl1(const Point& MainDot) : FigterJet(MainDot), count_to_shot(25) {};
	JetLvl1(const Point& MainPoint, int HP) : FigterJet(MainPoint, HP), count_to_shot(25) {};

	void draw() override {

		glColor3f(0.3, 0.5, 0.0);
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x - 21, MainDot.y + 105);
		glVertex2f(MainDot.x - 147, MainDot.y + 255);
		glVertex2f(MainDot.x - 21, MainDot.y + 255);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x + 21, MainDot.y + 105);
		glVertex2f(MainDot.x + 147, MainDot.y + 255);
		glVertex2f(MainDot.x + 21, MainDot.y + 255);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x - 105, MainDot.y + 330);
		glVertex2f(MainDot.x - 42, MainDot.y + 270);
		glVertex2f(MainDot.x - 42, MainDot.y + 330);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x + 105, MainDot.y + 330);
		glVertex2f(MainDot.x + 42, MainDot.y + 270);
		glVertex2f(MainDot.x + 42, MainDot.y + 330);
		glEnd();

		glColor3f(0.3, 0.3, 0.3);
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x - 42, MainDot.y + 300);
		glVertex2f(MainDot.x - 28, MainDot.y + 340);
		glVertex2f(MainDot.x - 16, MainDot.y + 340);
		glVertex2f(MainDot.x - 0, MainDot.y + 300);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x + 42, MainDot.y + 300);
		glVertex2f(MainDot.x + 28, MainDot.y + 340);
		glVertex2f(MainDot.x + 16, MainDot.y + 340);
		glVertex2f(MainDot.x + 0, MainDot.y + 300);
		glEnd();

		glColor3f(0.15, 0.27, 0);
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x - 0, MainDot.y - 30);
		glVertex2f(MainDot.x - 42, MainDot.y + 156);
		glVertex2f(MainDot.x - 42, MainDot.y + 300);
		glVertex2f(MainDot.x + 42, MainDot.y + 300);
		glVertex2f(MainDot.x + 42, MainDot.y + 156);
		glEnd();

		glColor3f(0.8, 0.8, 0.8);
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x - 0, MainDot.y - 0);
		glVertex2f(MainDot.x - 13, MainDot.y + 60);
		glVertex2f(MainDot.x + 13, MainDot.y + 60);
		glEnd();
	}

	bool is_hit(const Point& bullet) {
		Point A = Point(MainDot.x, MainDot.y - 25);
		Point B = Point(MainDot.x - 147, MainDot.y + 255);
		Point C = Point(MainDot.x + 147, MainDot.y + 255);
		Point LeftSide = A - B;
		Point RightSide = A - C;
		if (bullet.y > MainDot.y - 25 && bullet.x > MainDot.x - 147 && bullet.y < MainDot.y + 255
			&& bullet.x < MainDot.x + 147 && bullet.y < MainDot.y + 255) { // проверяем находится ли точка в "квадрате" около треуголька самолета
			if (bullet.x > A.x - 3 && bullet.x < A.x + 3 && bullet.y > A.y) {
				return true;
			}
			for (int i = 1; i < 30; ++i) { // проверяем близость точки к самому теугольнику
				Point B1 = B + Point(LeftSide.x / 30 * i, LeftSide.y / 30 * i);
				Point C1 = C + Point(RightSide.x / 30 * i, RightSide.y / 30 * i);
				if (bullet.x > B1.x && bullet.x < C1.x && bullet.y > B1.y) {
					return true;
				}
			}

			return false;
		}
		return false;
	}
};

struct JetLvl2 : public FigterJet
{
	int count_to_shot;
	Point MovePoint = Point(0, -5);
	int direction_change = 0;
	JetLvl2() : FigterJet(), count_to_shot(20) { };
	JetLvl2(const Point& MainDot) : FigterJet(MainDot), count_to_shot(20) {};
	JetLvl2(const Point& MainPoint, int HP) : FigterJet(MainPoint, HP), count_to_shot(20) {};

	void draw() override {
		glColor3f(0.9, 0.5, 0); //отрисовка крыльев самолета
		glBegin(GL_POLYGON);//Передние крылья
		glVertex2f(MainDot.x - 20, MainDot.y + 75);
		glVertex2f(MainDot.x - 120, MainDot.y + 120);
		glVertex2f(MainDot.x - 120, MainDot.y + 135);
		glVertex2f(MainDot.x - 45, MainDot.y + 125);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x + 20, MainDot.y + 75);
		glVertex2f(MainDot.x + 120, MainDot.y + 120);
		glVertex2f(MainDot.x + 120, MainDot.y + 135);
		glVertex2f(MainDot.x + 45, MainDot.y + 125);
		glEnd();

		glBegin(GL_POLYGON);//Несущие крылья
		glVertex2f(MainDot.x - 45, MainDot.y + 130);
		glVertex2f(MainDot.x - 180, MainDot.y + 210);
		glVertex2f(MainDot.x - 180, MainDot.y + 255);
		glVertex2f(MainDot.x - 45, MainDot.y + 255);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x + 45, MainDot.y + 130);
		glVertex2f(MainDot.x + 180, MainDot.y + 210);
		glVertex2f(MainDot.x + 180, MainDot.y + 255);
		glVertex2f(MainDot.x + 45, MainDot.y + 255);
		glEnd();

		glBegin(GL_POLYGON);//Задние крылья
		glVertex2f(MainDot.x - 45, MainDot.y + 260);
		glVertex2f(MainDot.x - 150, MainDot.y + 345);
		glVertex2f(MainDot.x - 120, MainDot.y + 375);
		glVertex2f(MainDot.x - 75, MainDot.y + 375);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x + 45, MainDot.y + 260);
		glVertex2f(MainDot.x + 150, MainDot.y + 345);
		glVertex2f(MainDot.x + 120, MainDot.y + 375);
		glVertex2f(MainDot.x + 75, MainDot.y + 375);
		glEnd();

		
		glColor3f(0.56, 0.2, 0); // отрисовка фюзеляжа самолета
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x, MainDot.y - 45);
		glVertex2f(MainDot.x - 20, MainDot.y + 75);
		glVertex2f(MainDot.x - 45, MainDot.y + 90);
		glVertex2f(MainDot.x - 60, MainDot.y + 330);
		glVertex2f(MainDot.x + 60, MainDot.y + 330);
		glVertex2f(MainDot.x + 45, MainDot.y + 90);
		glVertex2f(MainDot.x + 20, MainDot.y + 75);
		glEnd();

		glColor3f(0.2, 0.2, 0.2); //отрисовка турбин самолета
		glBegin(GL_POLYGON);//Передние крылья
		glVertex2f(MainDot.x - 45, MainDot.y + 260);
		glVertex2f(MainDot.x - 10, MainDot.y + 260);
		glVertex2f(MainDot.x - 15, MainDot.y + 400);
		glVertex2f(MainDot.x - 40, MainDot.y + 400);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x + 45, MainDot.y + 260);
		glVertex2f(MainDot.x + 10, MainDot.y + 260);
		glVertex2f(MainDot.x + 15, MainDot.y + 400);
		glVertex2f(MainDot.x + 40, MainDot.y + 400);
		glEnd();

		glColor3f(0.9, 0.9, 0.9);
		glBegin(GL_POLYGON);
		glVertex2f(MainDot.x, MainDot.y);
		glVertex2f(MainDot.x - 20, MainDot.y + 75);
		glVertex2f(MainDot.x + 20, MainDot.y + 75);
		glEnd();
		
	}

	bool is_hit(const Point& bullet) override {
		Point A = Point(MainDot.x, MainDot.y - 30);
		Point B = Point(MainDot.x - 180, MainDot.y + 255);
		Point C = Point(MainDot.x + 180, MainDot.y + 255);
		Point LeftSide = A - B;
		Point RightSide = A - C;
		if (bullet.y > MainDot.y - 25 && bullet.x > MainDot.x - 180 && bullet.y < MainDot.y + 255
			&& bullet.x < MainDot.x + 180 && bullet.y < MainDot.y + 255) { // проверяем находится ли точка в "квадрате" около треуголька самолета
			if (bullet.x > A.x - 3 && bullet.x < A.x + 3 && bullet.y > A.y) {
				return true;
			}
			for (int i = 1; i < 30; ++i) { // проверяем близость точки к самому теугольнику
				Point B1 = B + Point(LeftSide.x / 30 * i, LeftSide.y / 30 * i);
				Point C1 = C + Point(RightSide.x / 30 * i, RightSide.y / 30 * i);
				if (bullet.x > B1.x && bullet.x < C1.x && bullet.y > B1.y) {
					return true;
				}
			}

			return false;
		}
		return false;
	}

	
};

struct Bullet {
	int size;
	vector <double> color;
	Point center;

	Bullet() : size(20), color({ 0.0,0.0,0.0 }), center(Point(0, 0)) {};
	Bullet(int size, vector <double> color, const Point& center) : size(size), color(color), center(center) {};

	void draw() {
		glColor3f(color[0], color[1], color[2]);
		glBegin(GL_POLYGON);
		glVertex2f(center.x + size, center.y);
		glVertex2f(center.x + size * cos(M_PI / 3), center.y + size * sin(M_PI / 3));
		glVertex2f(center.x - size * cos(M_PI / 3), center.y + size * sin(M_PI / 3));
		glVertex2f(center.x - size, center.y);
		glVertex2f(center.x - size * cos(M_PI / 3), center.y - size * sin(M_PI / 3));
		glVertex2f(center.x + size * cos(M_PI / 3), center.y - size * sin(M_PI / 3));
		glEnd();
	}

	void move_bullet(double num) {
		center = center + Point(0, num);
	}

};

struct MyJet : public FigterJet{
	MyJet() : FigterJet() {};
	MyJet(const Point& MainPoint) : FigterJet(MainPoint) {};
	MyJet(const Point& MainPoint, int HP) : FigterJet(MainPoint, HP) {};
	
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

	bool is_hit(const Point& bullet) override{
		Point A = Point(MainDot.x, MainDot.y + 30);
		Point B = Point(MainDot.x - 147, MainDot.y - 255);
		Point C = Point(MainDot.x + 147, MainDot.y - 255);
		Point LeftSide = A - B;
		Point RightSide = A - C;
		if (bullet.y < MainDot.y + 35 && bullet.x > MainDot.x - 147 && bullet.y > MainDot.y - 255 
			&& bullet.x < MainDot.x + 147 && bullet.y > MainDot.y - 255) { // проверяем находится ли точка в "квадрате" около треуголька самолета
			if (bullet.x > A.x - 5 && bullet.x < A.x + 5 && bullet.y < A.y) {
				return true;
			}
			for (int i = 1; i < 20; ++i) { // проверяем близость точки к самому теугольнику
				Point B1 = B + Point(LeftSide.x / 20 * i, LeftSide.y / 20 * i);
				Point C1 = C + Point(RightSide.x / 20 * i, RightSide.y / 20 * i);
				if (bullet.x > B1.x && bullet.x < C1.x && bullet.y < B1.y) {
					return true;
				}
			}
			
			return false;
		}
		return false;
	}

	bool is_intersect(JetLvl1 Enemy) {
		Point A = Point(MainDot.x, MainDot.y + 30);
		Point B = Point(MainDot.x - 147, MainDot.y - 255);
		Point C = Point(MainDot.x + 147, MainDot.y - 255);
		vector<Point> MyJetVerts = { A, B, C };
		Point A1 = Point(Enemy.MainDot.x, Enemy.MainDot.y - 30);
		Point B1 = Point(Enemy.MainDot.x - 147, Enemy.MainDot.y + 255);
		Point C1 = Point(Enemy.MainDot.x + 147, Enemy.MainDot.y + 255);
		vector<Point> EnemyJetVerts = { A1, B1, C1 };
		for (int i = 0; i < 3; ++i) {
			if (this->is_hit(EnemyJetVerts[i])) { return true; }
		}
		for (int i = 0; i < 3; ++i) {
			if (Enemy.is_hit(MyJetVerts[i])) { return true; }
		}
		return false;
	}

	bool is_intersect(JetLvl2 Enemy) {
		Point A = Point(MainDot.x, MainDot.y + 30);
		Point B = Point(MainDot.x - 147, MainDot.y - 255);
		Point C = Point(MainDot.x + 147, MainDot.y - 255);
		vector<Point> MyJetVerts = { A, B, C };
		Point A1 = Point(Enemy.MainDot.x, Enemy.MainDot.y - 30);
		Point B1 = Point(Enemy.MainDot.x - 180, Enemy.MainDot.y + 255);
		Point C1 = Point(Enemy.MainDot.x + 180, Enemy.MainDot.y + 255);
		vector<Point> EnemyJetVerts = { A1, B1, C1 };
		for (int i = 0; i < 3; ++i) {
			if (this->is_hit(EnemyJetVerts[i])) { return true; }
		}
		for (int i = 0; i < 3; ++i) {
			if (Enemy.is_hit(MyJetVerts[i])) { return true; }
		}
		return false;
	}
};

