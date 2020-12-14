#pragma once
#define _USE_MATH_DEFINES // for C
#include <math.h>

#include <vector>
#include <Windows.h>

#include"Point.h"

using namespace std;

class Help {
public:
	//найти вектор (A - начало вектора, B - конец вектора)
	static Point SearchVector(Point A, Point B) {
		return Point(B.x - A.x, B.y - A.y, B.z - A.z);
	}
	//векторное произведение
	static Point VectorProduct(Point vectorA, Point vectorB) {
		Point result(0, 0, 0);
		result.x = vectorA.y * vectorB.z - vectorB.y * vectorA.z;
		result.y = -1 * vectorA.x * vectorB.z + vectorB.x * vectorA.z;
		result.z = vectorA.x * vectorB.y - vectorB.x * vectorA.y;
		return result;
	}
	static double ScalarProduct(Point A, Point B) {
		return A.x * B.x + A.y * B.y + A.z * B.z;
	}
	//поиск длины вектора
	static double SearchVectorLength(Point vector) {
		double length = sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
		return length;
	}
	//меняем направление нормали
	static void NormalRotation(Point& Normal) {
		Normal.x *= -1;
		Normal.y *= -1;
		Normal.z *= -1;
	}
	//ищем нормаль к треугольнику и нормализируем ее
	static Point SearchNormal(Point A, Point B, Point C, bool flag = true) {

		//поиск векторов по заданнымточкам
		Point vector_b = SearchVector(B, C);
		Point vector_a = SearchVector(B, A);

		//векторное произведение
		Point NormalVector = VectorProduct(vector_a, vector_b);

		//поиск длины вектора
		double length = SearchVectorLength(NormalVector);

		if (flag) {
			//нормализация вектора
			NormalVector.x = NormalVector.x / length;
			NormalVector.y = NormalVector.y / length;
			NormalVector.z = NormalVector.z / length;
		}
		return NormalVector;
	}
	//находим середину отрезка
	static Point SearchMidpoint(Point A, Point B) {
		Point midpoint(0, 0, 0);
		midpoint.x = (A.x + B.x) / 2;
		midpoint.y = (A.y + B.y) / 2;
		midpoint.z = (A.z + B.z) / 2;
		return midpoint;
	}
	//находим расстояние между точками
	static double SearchDistancePoints(Point A, Point B) {
		double X = B.x - A.x;
		double Y = B.y - A.y;
		double Z = B.z - A.z;
		X = pow(X, 2);
		Y = pow(Y, 2);
		Z = pow(Z, 2);
		double r = sqrt(X + Y + Z);
		return r;
	}
	//найти угол между векторами
	static double SearchAngleVector(Point VectorA, Point VectorB) {
		double number1 = VectorA.x * VectorB.x + VectorA.y * VectorB.y + VectorA.z * VectorB.z;
		double number2 = sqrt(pow(VectorA.x, 2) + pow(VectorA.y, 2) + pow(VectorA.z, 2));
		double number3 = sqrt(pow(VectorB.x, 2) + pow(VectorB.y, 2) + pow(VectorB.z, 2));
		double angle = acos(number1 / (number2 * number3));
		angle = angle * 180 / M_PI;
		return angle;
	}
	//точка пересечения двух прямых в пространстве XY
	static Point SearchIntersectionPoint(Point A, Point B, Point C, Point D) {
		Point IntersectionPoint(0, 0, 0);

		double number1 = (A.x * B.y - A.y * B.x) * (C.x - D.x);
		double number2 = (A.x - B.x) * (C.x * D.y - C.y * D.x);
		double number3 = (A.x - B.x) * (C.y - D.y);
		double number4 = (A.y - B.y) * (C.x - D.x);

		IntersectionPoint.x = (number1 - number2) / (number3 - number4);

		number1 = (A.x * B.y - A.y * B.x) * (C.y - D.y);
		number2 = (A.y - B.y) * (C.x * D.y - C.y * D.x);

		IntersectionPoint.y = (number1 - number2) / (number3 - number4);

		return IntersectionPoint;
	}

	//вершина параболы
	static Point ApexParabola(double a, double b, double c) {
		double x = (-1 * b) / (2 * a);
		double y = pow(x, 2) * a + x * b + c;
		Point O(x, y, 0);
		return O;
	}
	//поиск центра окружности. если известны три точки лежащие на ней
	static void FindCenterCircle(Point A, Point B, Point C, Point& Center, double& r) {
		Center.x = -0.5 * (A.y * (pow(B.x, 2) + pow(B.y, 2) - pow(C.x, 2) - pow(C.y, 2))
			+ B.y * (pow(C.x, 2) + pow(C.y, 2) - pow(A.x, 2) - pow(A.y, 2))
			+ C.y * (pow(A.x, 2) + pow(A.y, 2) - pow(B.x, 2) - pow(B.y, 2))) /
			(A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));

		Center.y = 0.5 * (A.x * (pow(B.x, 2) + pow(B.y, 2) - pow(C.x, 2) - pow(C.y, 2))
			+ B.x * (pow(C.x, 2) + pow(C.y, 2) - pow(A.x, 2) - pow(A.y, 2))
			+ C.x * (pow(A.x, 2) + pow(A.y, 2) - pow(B.x, 2) - pow(B.y, 2))) /
			(A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));

		r = SearchDistancePoints(Center, A);
	}
	//составление уравнения плоскости
	static vector<double> PlaneEquation(Point M2, Point M0, Point M1) {
		Point M0M1 = SearchVector(M0, M1);
		Point M0M2 = SearchVector(M0, M2);

		vector<double> plane;
		plane.push_back(M0M1.y * M0M2.z - M0M1.z * M0M2.y);
		plane.push_back(-1 * (M0M1.x * M0M2.z - M0M1.z * M0M2.x));
		plane.push_back(M0M1.x * M0M2.y - M0M1.y * M0M2.x);

		//Ax + By + Cz + D = 0
		plane.push_back(-1 * (plane[0] * M0.x + plane[1] * M0.y + plane[2] * M0.z));

		return plane;
	}
	//определение положения точки относительно заданной плоскости
	static int  PositionPoint(vector<double> plane, Point O) {
		double result = plane[0] * O.x + plane[1] * O.y + plane[2] * O.z + plane[3];
		if (result > 0) {
			return 1;
		}
		if (result < 0) {
			return -1;
		}
		return 0;
	}
	//нормализация вектора
	static Point VectorNormalization(Point vector) {
		//поиск длины вектора
		double length = SearchVectorLength(vector);

		//нормализация вектора
		vector.x = vector.x / length;
		vector.y = vector.y / length;
		vector.z = vector.z / length;

		return vector;
	}
	//перевод градусов в радианты
	static double ConvertDegreesToRadiants(double angle) {
		return angle * M_PI / 180;
	}
};