#pragma once
#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include "glut.h" //необходимо указть путь к папке с файлом
#include <math.h>

bool equal(double first, double second) {
    return abs(first - second) < 0.001;
}

double deg_to_rad(double angle) {
    return (angle / 180) * M_PI;
}

double rad_to_deg(double angle) {
    return (angle / M_PI) * 180;
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
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(0.0, 0.0, 0.0); // Белый цвет
        glVertex2f(x, y);
        glEnd();
    }

    void drawPoint(int size) {
        // Установка толщины линии
        glPointSize(size);
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

