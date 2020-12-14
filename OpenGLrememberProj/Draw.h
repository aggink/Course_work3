#pragma once
#include <vector>
#include <Windows.h>
#include <GL\GL.h>

#include"Help.h"

typedef long double ld;

using namespace std;

class Draw : public Help {
public:
	//Перещет координат точек лежащий на плоскости в координаты картинки (высота и ширина картинки 1х1)
	//Инициализация (первичная установка для плоскости) или пересчет координаты точки
	//А - точка новой системы координат (выбирать крайнии точки плоскости, самые далекие от центра) - инициализация
	//А - точка, которую надо пересчитать в новые координаты
	//set - произвести первичную настройку - выбрать при инициализации
	//points - набор точек для обределения границ плоскости - задать при инициализации
	//r - неучненные факторы (расстояние на которое надо увеличить размеры плоскости по ширине и высоте) - задать при инициализации
	static PointXY UpdatePoint(Point A, bool set = false, vector<Point> points = { Point(0, 0, 0) }, double r = 0) {
		static bool installation = false;
		static double width = 0;
		static double height = 0;
		static double min_x, min_y, max_x, max_y;
		static PointXY new_O(0, 0);
		PointXY newCoord(2, 2);
		if (set == true) {
			//определяем границы фигуры без учета выпуклости
			min_x = points[0].x, min_y = points[0].y, max_x = points[0].x, max_y = points[0].y;
			for (int i = 0; i < (int)points.size(); i++) {
				if (min_x > points[i].x) {
					min_x = points[i].x;
				}
				if (max_x < points[i].x) {
					max_x = points[i].x;
				}
				if (min_y > points[i].y) {
					min_y = points[i].y;
				}
				if (max_y < points[i].y) {
					max_y = points[i].y;
				}
			}

			//расчет длины и ширины прямоугольника куда вписана наша фигура
			width = abs(min_x) + abs(max_x) + r;
			height = abs(min_y) + abs(max_y) + r;

			new_O.x = A.x;
			new_O.y = A.y;

			//настройка произведена
			installation = true;

			return newCoord;
		}
		if (installation) {
			//расчет новых координат относительно 
			newCoord.x = (A.x - new_O.x) / width;
			newCoord.y = (A.y - new_O.y) / height;
			return newCoord;
		}
		newCoord.x = 9;
		newCoord.y = 9;
		return newCoord;
	}
public:
	//рисуем линию
	static void DrawLine(Point A, Point B, bool flag = false) {
		if (flag) {
			glBegin(GL_LINES);
		}
		A.DrawPoint();
		B.DrawPoint();
		if (flag)
		{
			glEnd();
		}
	}
	static void DrawQuads(Point A, Point B, Point C, Point D, vector<PointXY> texCoord, int normal = 1) {
		if (normal > 0) {
			SearchNormal(A, B, C).Normal3d();
		}
		else {
			Point Normal = SearchNormal(A, B, C);
			NormalRotation(Normal);
			Normal.Normal3d();
		}
		texCoord[0].TexCoord2d();
		A.DrawPoint();
		texCoord[1].TexCoord2d();
		B.DrawPoint();
		texCoord[2].TexCoord2d();
		C.DrawPoint();
		texCoord[3].TexCoord2d();
		D.DrawPoint();
	}
	static void DrawQuads(Point A, Point B, Point C, Point D, int normal = 1) {
		if (normal > 0) {
			SearchNormal(A, B, C).Normal3d();
		}
		else {
			Point Normal = SearchNormal(A, B, C);
			NormalRotation(Normal);
			Normal.Normal3d();
		}
		A.DrawPoint();
		B.DrawPoint();
		C.DrawPoint();
		D.DrawPoint();
	}
	//рисуем прямоугольник
	static void DrawRectangle(Point A, Point B, double height, vector<PointXY> texCoord, int normal = 1, bool flag = false) {
		if (flag) {
			glBegin(GL_QUADS);
		}

		Point A1 = A + Point(0, 0, height);
		Point B1 = B + Point(0, 0, height);

		if (normal > 0) {
			SearchNormal(A1, A, B).Normal3d();
		}
		else {
			Point Normal = SearchNormal(A1, A, B);
			NormalRotation(Normal);
			Normal.Normal3d();
		}

		texCoord[0].TexCoord2d();
		A.DrawPoint();
		texCoord[1].TexCoord2d();
		B.DrawPoint();
		texCoord[2].TexCoord2d();
		B1.DrawPoint();
		texCoord[3].TexCoord2d();
		A1.DrawPoint();
		if (flag) {
			glEnd();
		}
	}
	//рисуем шестиугольник
	static void DrawPolygon(vector<Point> points, bool flag = false) {
		if (flag) {
			glBegin(GL_POLYGON);
		}
		for (int i = 0; i < (int)points.size(); i++) {
			points[i].DrawPoint();
		}
		if (flag) {
			glEnd();
		}
	}
	//рисуем треугольник
	static void DrawTriangles(vector<Point> points, int normal = 1, bool flag = false) {
		if (flag) {
			glBegin(GL_TRIANGLES);
		}

		if (normal > 0) {
			SearchNormal(points[0], points[1], points[2]).Normal3d();
		}
		else {
			Point Normal = SearchNormal(points[0], points[1], points[2]);
			NormalRotation(Normal);
			Normal.Normal3d();
		}

		for (int i = 0; i < (int)points.size(); i++) {
			UpdatePoint(points[i]).TexCoord2d();
			points[i].DrawPoint();
		}
		if (flag) {
			glEnd();
		}
	}
	static void DrawTriangles(Point A, Point B, Point C, int normal = 1, bool flag = false) {
		if (flag) {
			glBegin(GL_TRIANGLES);
		}

		if (normal > 0) {
			SearchNormal(A, B, C).Normal3d();
		}
		else {
			Point Normal = SearchNormal(A, B, C);
			NormalRotation(Normal);
			Normal.Normal3d();
		}

		UpdatePoint(A).TexCoord2d();
		glVertex3d(A.x, A.y, A.z);

		UpdatePoint(B).TexCoord2d();
		glVertex3d(B.x, B.y, B.z);

		UpdatePoint(C).TexCoord2d();
		glVertex3d(C.x, C.y, C.z);
		if (flag) {
			glEnd();
		}
	}
	static void DrawTriangles(Point A, Point B, Point C, double height, int normal = 1, bool flag = false) {
		if (flag) {
			glBegin(GL_TRIANGLES);
		}

		if (normal > 0) {
			SearchNormal(A, B, C).Normal3d();
		}
		else {
			Point Normal = SearchNormal(A, B, C);
			NormalRotation(Normal);
			Normal.Normal3d();
		}

		UpdatePoint(A).TexCoord2d();
		glVertex3d(A.x, A.y, height);

		UpdatePoint(B).TexCoord2d();
		glVertex3d(B.x, B.y, height);

		UpdatePoint(C).TexCoord2d();
		glVertex3d(C.x, C.y, height);
		if (flag) {
			glEnd();
		}
	}
	static void DrawTriangles_(Point A, Point B, Point C, int normal = 1) {
		if (normal > 0) {
			SearchNormal(A, B, C).Normal3d();
		}
		else {
			Point Normal = SearchNormal(A, B, C);
			NormalRotation(Normal);
			Normal.Normal3d();
		}

		A.DrawPoint();
		B.DrawPoint();
		C.DrawPoint();
	}
	static void  DrawTriangles(Point A, Point B, Point C, vector<PointXY> texCoord, int normal = 1) {
		if (normal > 0) {
			SearchNormal(A, B, C).Normal3d();
		}
		else {
			Point Normal = SearchNormal(A, B, C);
			NormalRotation(Normal);
			Normal.Normal3d();
		}
		texCoord[0].TexCoord2d();
		A.DrawPoint();
		texCoord[1].TexCoord2d();
		B.DrawPoint();
		texCoord[2].TexCoord2d();
		C.DrawPoint();
	}
	//рандомный цвет
	static void RandomColor() {
		glColor3d((ld)((rand() % 999) + 1.0) / 1000, (ld)((rand() % 999) + 1.0) / 1000, (ld)((rand() % 999) + 1.0) / 1000);
	}
	static void Color(int i) {
		switch (i) {
		case 1:
			glColor3d(0.7, 0.6, 0.9);
			break;
		case 2:
			glColor3d(0.8, 0.3, 0.6);
			break;
		case 3:
			glColor3d(0.3, 0.8, 0.3);
			break;
		}
	}
};
