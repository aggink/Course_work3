#pragma once
#define _USE_MATH_DEFINES // for C
#include <chrono>
#include <vector>

#include"Draw.h"

//включение/отключение рисование линий
bool line = false;
double sharpness_t = 0.01;


//Рассчет одной точки для кривой Эрмита
Point HermitCurve(Point P1, Point R1, Point P2, Point R2, double t) {
	double t_2 = t * t;
	double t_3 = t * t * t;

	double x = P1.x * (2 * t_3 - 3 * t_2 + 1) + P2.x * (-2 * t_3 + 3 * t_2) + R1.x * (t_3 - 2 * t_2 + t) + R2.x * (t_3 - t_2);
	double y = P1.y * (2 * t_3 - 3 * t_2 + 1) + P2.y * (-2 * t_3 + 3 * t_2) + R1.y * (t_3 - 2 * t_2 + t) + R2.y * (t_3 - t_2);
	double z = P1.z * (2 * t_3 - 3 * t_2 + 1) + P2.z * (-2 * t_3 + 3 * t_2) + R1.z * (t_3 - 2 * t_2 + t) + R2.z * (t_3 - t_2);

	return Point(x, y, z);
}

//Рассчитать точки кривой Эрмита
//P1 - точка начала кривой, P2 - точка конца кривой
//R1 - вектор произвольной кривой при t = 0, R2 - вектор проивольной кривой при t = 1
//t - шаг
//t_max - максимальное значение t
vector<Point> PointHermitCurve(Point P1, Point R1, Point P2, Point R2, double t_max = 1) {
	vector<Point> points;
	t_max = 1 + sharpness_t * 0.001;

	for (double i = 0; i <= t_max; i += sharpness_t) {
		points.push_back(HermitCurve(P1, R1, P2, R2, i));
	}
	return points;
}

//Рассет одной точки для кривой Безье второго порядка
Point BezierCurve2(Point P1, Point P2, Point P3, double t) {
	double t_2 = t * t;
	double var = 1 - t;
	double var_2 = var * var;

	double x = P1.x * var_2 + 2 * P2.x * t * var + P3.x * t_2;
	double y = P1.y * var_2 + 2 * P2.y * t * var + P3.y * t_2;
	double z = P1.z * var_2 + 2 * P2.z * t * var + P3.z * t_2;

	return Point(x, y, z);
}

//Рассчитать точки кривой Безье второго порядка
//P1, P2, P3 - точки, t - шаг, t_max - максимальное значение t 
vector<Point> PointBezierCurve2(Point P1, Point P2, Point P3, double t_max = 1) {
	vector<Point> points;
	t_max = 1 + sharpness_t * 0.001;

	for (double i = 0; i <= t_max; i += sharpness_t) {
		points.push_back(BezierCurve2(P1, P2, P3, i));
	}
	return points;
}

//Рассчитать точки кривой Безье третьего порядка
Point BezierCurve3(Point P0, Point P1, Point P2, Point P3, double t) {
	double t_2 = t * t;
	double t_3 = t_2 * t;
	double var = 1 - t;
	double var_2 = var * var;
	double var_3 = var_2 * var;

	double x = var_3 * P0.x + 3 * t * var_2 * P1.x + 3 * t_2 * var * P2.x + t_3 * P3.x;
	double y = var_3 * P0.y + 3 * t * var_2 * P1.y + 3 * t_2 * var * P2.y + t_3 * P3.y;
	double z = var_3 * P0.z + 3 * t * var_2 * P1.z + 3 * t_2 * var * P2.z + t_3 * P3.z;

	return Point(x, y, z);
}

//Рассчитать точки кривой Безье третьего порядка
//Начинается в точке P0, стремится приблизится к P1, затем к P2 и заканчивается в точке P3
//t - шаг, t_max - максимальное значение t
vector<Point> PointBezierCurve3(Point P0, Point P1, Point P2, Point P3, double t_max = 1) {
	vector<Point> points;
	t_max = 1 + sharpness_t * 0.001;

	for (double i = 0; i <= t_max; i += sharpness_t) {
		points.push_back(BezierCurve3(P0, P1, P2, P3, i));
	}
	return points;
}

//Рисуем кривую Эрмита
void DrawHermitCurve(Point P1, Point R1, Point P2, Point R2) {
	if (line) {
		//размер точки
		glPointSize(10);

		//рисуем точки
		glColor3d(1, 0, 0);
		glBegin(GL_POINTS);
		P1.DrawPoint();
		P2.DrawPoint();
		R1.DrawPoint();
		R2.DrawPoint();
		glEnd();

		//размер точки
		glPointSize(1);


		//рисуем вектора
		glColor3d(0.6, 0.2, 0.3);
		glBegin(GL_LINES);
		Draw::DrawLine(P1, R1);
		Draw::DrawLine(P2, R2);
		glEnd();

	}
	//ширина линии
	glLineWidth(3);

	R1 = Draw::SearchVector(P1, R1);
	R2 = Draw::SearchVector(P2, R2);

	//Рисуем кривую Эрмита
	glColor3d(0.4, 0.5, 0.6);
	glBegin(GL_LINE_STRIP);
	vector<Point> points = PointHermitCurve(P1, R1, P2, R2);
	for (int i = 0; i < (int)points.size(); i++) {
		points[i].DrawPoint();
	}
	glEnd();

	//ширина линии
	glLineWidth(1);

}

//Рисуем кривую Безье второго порядка
void DrawBezierCurve2(Point A, Point B, Point C) {

	if (line) {
		//размер точки
		glPointSize(10);

		//рисуем точки
		glColor3d(1, 0, 1);
		glBegin(GL_POINTS);
		A.DrawPoint();
		B.DrawPoint();
		C.DrawPoint();
		glEnd();

		//размер точки
		glPointSize(1);

		//рисуем ломанную ABC
		glColor3d(0.8, 1, 0.3);
		glBegin(GL_LINE_STRIP);
		A.DrawPoint();
		B.DrawPoint();
		C.DrawPoint();
		glEnd();

	}

	//ширина линии
	glLineWidth(3);

	//рисуем кривую Безье второго порядка
	glColor3d(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	vector<Point> points = PointBezierCurve2(A, B, C);
	for (int i = 0; i < (int)points.size(); i++) {
		points[i].DrawPoint();
	}
	glEnd();

	//ширина линии
	glLineWidth(1);

}

//Рисуем кривую Безье третьего порядка
void DrawBezierCurve3(Point A, Point B, Point C, Point D) {
	if (line) {
		//размер точки
		glPointSize(10);

		//рисуем точки
		glColor3d(1, 0, 1);
		glBegin(GL_POINTS);
		A.DrawPoint();
		B.DrawPoint();
		C.DrawPoint();
		D.DrawPoint();
		glEnd();

		//размер точки
		glPointSize(1);

		//рисуем ломанную ABCD
		glColor3d(0.7, 0.2, 0.6);
		glBegin(GL_LINE_STRIP);
		A.DrawPoint();
		B.DrawPoint();
		C.DrawPoint();
		D.DrawPoint();
		glEnd();

	}
	// ширина линии
	glLineWidth(3);

	//рисуем кривую Безье второго порядка
	glColor3d(0, 0, 1);
	glBegin(GL_LINE_STRIP);
	vector<Point> points = PointBezierCurve3(A, B, C, D);
	for (int i = 0; i < (int)points.size(); i++) {
		points[i].DrawPoint();
	}
	glEnd();

	//ширина линии
	glLineWidth(1);

}

//найти определитель матрицы 3 на 3
double SearchDeterminantMatrix3(double m[3][3]) {
	return m[0][0] * m[1][1] * m[2][2] +
		m[2][0] * m[0][1] * m[1][2] +
		m[1][0] * m[2][1] * m[0][2] -
		m[2][0] * m[1][1] * m[0][2] -
		m[0][0] * m[2][1] * m[1][2] -
		m[1][0] * m[0][1] * m[2][2];
}

//рисуем куб
void DrawSquare() {
	static Point XYZ = { 0, 0, 0 };

	double l = 0.5;
	Point A(l, l, -1 * l), B(l, -1 * l, -1 * l), C(-1 * l, -1 * l, -1 * l), D(-1 * l, l, -1 * l);
	Point AA(l, l, l), BB(l, -1 * l, l), CC(-1 * l, -1 * l, l), DD(-1 * l, l, l);

	Point O(2 * l, 0, 0);
	Point X(2, 0, 0);
	Point Y(0, 2, 0);
	Point Z(0, 0, 2);

	glColor3d(0.1, 0.3, 0.3);
	glBegin(GL_QUADS);
	//низ
	Draw::DrawQuads(A, B, C, D);
	//верх
	Draw::DrawQuads(AA, BB, CC, DD);

	//стены
	glColor3d(0.4, 0.5, 0.3);
	Draw::DrawQuads(BB, B, A, AA, -1);
	glColor3d(0.5, 0.3, 0.6);
	Draw::DrawQuads(CC, C, B, BB, -1);
	glColor3d(0.7, 0.5, 0.1);
	Draw::DrawQuads(DD, D, C, CC, -1);
	glColor3d(0.8, 0.4, 0.8);
	Draw::DrawQuads(AA, A, D, DD, -1);

	glEnd();

	glColor3d(0.7, 0.3, 0.2);
	glBegin(GL_TRIANGLES);
	Draw::DrawTriangles_(O, AA, A);
	Draw::DrawTriangles_(O, BB, AA);
	Draw::DrawTriangles_(O, BB, B);
	Draw::DrawTriangles_(O, A, B);
	glEnd();

	if (line) {
		glLineWidth(3);
		glBegin(GL_LINES);
		glColor3d(1, 0, 0);
		Draw::DrawLine(XYZ, X);
		glColor3d(0, 1, 0);
		Draw::DrawLine(XYZ, Y);
		glColor3d(0, 0, 1);
		Draw::DrawLine(XYZ, Z);
		glEnd();
		glLineWidth(1);
	}

}

enum Method {
	Hermit,
	Bezier2,
	Bezier3
};

//поворот фигуры
void MoveAxis(Point point, Point next_point) {
	Point dir = Draw::SearchVector(point, next_point);
	dir = Draw::VectorNormalization(dir);

	Point orig(1, 0, 0);
	Point rotX(dir.x, dir.y, 0);
	rotX = Draw::VectorNormalization(rotX);
	double cosU = Draw::ScalarProduct(orig, rotX);
	Point vecpr = Draw::VectorProduct(orig, rotX);

	double sinSign = vecpr.z / abs(vecpr.z);
	double U = acos(cosU) * 180.0 / M_PI * sinSign;
	double ZU = acos(dir.z) * 180.0 / M_PI - 90;

	glRotated(U, 0, 0, 1);
	glRotated(ZU, 0, 1, 0);

}

//объект двигается по кривой туда сюда
void AnimationObjectCurve(vector<Point> points, vector<double> t, Method method) {
	vector<Point> PointsCurve;
	Point point(0, 0, 0);
	Point next_point(0, 0, 0);
	Point last_point(0, 0, 0);

	double t_max = t[0], t_max_next = t[1];

	switch (method) {
	case Hermit:
		if (line) {
			DrawHermitCurve(points[0], points[1], points[2], points[3]);
		}

		glPushMatrix();

		point = HermitCurve(points[0], Draw::SearchVector(points[0], points[1]), points[2], Draw::SearchVector(points[2], points[3]), t_max);
		next_point = HermitCurve(points[0], Draw::SearchVector(points[0], points[1]), points[2], Draw::SearchVector(points[2], points[3]), t_max_next);
		point.Translated();
		MoveAxis(point, next_point);
		DrawSquare();

		glPopMatrix();
		break;

	case Bezier2:
		if (line) {
			DrawBezierCurve2(points[0], points[1], points[2]);
		}

		glPushMatrix();
		point = BezierCurve2(points[0], points[1], points[2], t_max);
		next_point = BezierCurve2(points[0], points[1], points[2], t_max_next);

		point.Translated();
		MoveAxis(point, next_point);
		DrawSquare();

		glPopMatrix();
		break;

	case Bezier3:
		if (line) {
			DrawBezierCurve3(points[0], points[1], points[2], points[3]);
		}

		glPushMatrix();

		point = BezierCurve3(points[0], points[1], points[2], points[3], t_max);
		next_point = BezierCurve3(points[0], points[1], points[2], points[3], t_max_next);

		point.Translated();
		MoveAxis(point, next_point);
		DrawSquare();

		glPopMatrix();
		break;
	}
}

//факториал
//Начало функции нахождения факториала
int factorial(int N)
{
	if (N < 0)
		return 0;
	if (N == 0)
		return 1;
	else
		return N * factorial(N - 1);
}

//функция базиса Бернштейна
double FunctionBernsteinBasis(int n, int i, double t) {
	return (factorial(n) / (factorial(n - i) * factorial(i))) * pow(t, i) * pow(1 - t, n - i);
}

//определение радиус вектора
Point FunctionBezierSurface(vector<vector<Point>> mas, double u, double v) {
	Point R(0, 0, 0);
	int row = (int)mas.size();
	for (int i = 0; i < row; i++) {
		int col = (int)mas[i].size();
		for (int j = 0; j < col; j++) {
			double Bi = FunctionBernsteinBasis(row - 1, i, u);
			double Bj = FunctionBernsteinBasis(col - 1, j, v);
			R.x += Bj * Bi * mas[i][j].x;
			R.y += Bj * Bi * mas[i][j].y;
			R.z += Bj * Bi * mas[i][j].z;
		}
	}
	return R;
}

//нарисовать поверхность Безье из точек
void DrawBezierSurfacePoint(vector<vector<Point>> mas) {
	double t_max = 1 + sharpness_t * 0.001;
	vector<Point> points;
	vector<vector<Point>> mas_points;
	for (double u = 0; u <= t_max; u += sharpness_t) {
		for (double v = 0; v <= t_max; v += sharpness_t) {
			points.push_back(FunctionBezierSurface(mas, u, v));
		}
		mas_points.push_back(points);
		points.clear();
	}

	//рисуем поверхность Безье
	glColor3d(0.7, 0.3, 0.6);
	glBegin(GL_TRIANGLES);
	int row = (int)mas_points.size();
	for (int i = 0; i < row - 1; i++) {
		int col = (int)mas_points[i].size();
		for (int j = 0; j < col - 1; j++) {
			//берем четырехугольник и рисуем его двумя трегольниками
			Point A = mas_points[i][j];
			Point B = mas_points[i + 1][j];
			Point C = mas_points[i][j + 1];
			Point D = mas_points[i + 1][j + 1];

			//первый треугольник
			vector<PointXY> texCoordA = { PointXY((j + 0.0) / col, (i + 0.0) / row), PointXY((j + 0.0) / col, (i + 1.0) / row), PointXY((j + 1.0) / col, (i + 0.0) / row) };
			Draw::DrawTriangles(A, B, C, texCoordA, -1);

			//второй треугольник
			vector<PointXY> texCoordB = { PointXY((j + 0.0) / col, (i + 1.0) / row), PointXY((j + 1.0) / col, (i + 0.0) / row), PointXY((j + 1.0) / col, (i + 1.0) / row) };
			Draw::DrawTriangles(B, C, D, texCoordB);
		}
	}
	glEnd();


	if (line) {
		//отключаем текстуру
		glDisable(GL_TEXTURE_2D);

		//рисуем точки
		//размер точки
		glPointSize(10);
		glColor3d(1, 0, 0);

		glBegin(GL_POINTS);
		for (int i = 0; i < (int)mas.size(); i++) {
			for (int j = 0; j < (int)mas[i].size(); j++) {
				mas[i][j].DrawPoint();
			}
		}

		glEnd();
		//размер точки
		glPointSize(1);


		//рисуем сетку
		glLineWidth(2);
		glColor3d(0.4, 0.3, 0.4);

		glBegin(GL_LINES);
		for (int i = 0; i < (int)mas.size(); i++) {
			for (int j = 0; j < (int)mas[i].size(); j++) {

				if (j < (int)mas[i].size() - 1) {
					mas[i][j].DrawPoint();
					mas[i][j + 1].DrawPoint();
				}

				if (i < (int)mas.size() - 1) {
					mas[i][j].DrawPoint();
					mas[i + 1][j].DrawPoint();
				}

			}
		}

		glEnd();
		glLineWidth(1);

		//включаем текстуру
		glEnable(GL_TEXTURE_2D);
	}
}

//измеряем промежутки времени между отрисовкой
double Search_delta_time() {
	static auto end_render = std::chrono::steady_clock::now();
	auto cur_time = std::chrono::steady_clock::now();
	auto deltatime = cur_time - end_render;
	double delta = 1.0 * std::chrono::duration_cast<std::chrono::microseconds>(deltatime).count() / 1000000;
	end_render = cur_time;
	return delta;
}


void Draw() {

	static double t_max = 0;
	static double t_max_next = 0;
	static bool flag_tmax = true;

	//настройка времени
	double delta_time = Search_delta_time();
	double go = delta_time / 5; //t_max становится = 1 за 5 секунд
	//t_max сама по себе изменяется от 0 до 1 постепенно от кадра к кадру
	if (flag_tmax) {
		t_max += go;
		t_max_next = t_max + go;
		if (t_max > 1) {
			t_max = 1;
			flag_tmax = false;
		}
		if (t_max_next > 1) {
			t_max_next = 1;
		}
	}
	else {
		t_max -= go;
		t_max_next = t_max - go;
		if (t_max < 0) {
			t_max = 0;
			flag_tmax = true;
		}
		if (t_max_next < 0) {
			t_max_next = 0;
		}
	}

	vector<double> t = { t_max, t_max_next };

	//Анимация, движение объекта по кривой туда сюда
	//Рисуем кривые Эрмита
	//AnimationObjectCurve(vector<Point> {PointHB[0], PointHB[1], PointHB[2], PointHB[3]}, t, Hermit);

	//Рисуем кривые Безье 2 порядка
	//AnimationObjectCurve(vector<Point> {PointHB[8], PointHB[9], PointHB[10]}, t, Bezier2)

	//Рисуем кривую Безье 3 порядка
	//AnimationObjectCurve(vector<Point> {PointHB[18], PointHB[19], PointHB[20], PointHB[21]}, t, Bezier3);

}