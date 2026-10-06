#pragma once
#include <iostream>
class Matrix
{
	double** matrix; //двумерный массив
	int row, col; // строки и столбцы
public:
	void Init(int row, int col); //инициализаци€ матрицы
	void Rand(); //заполнение матрицы случайными значени€ми 
	static void Multi(const Matrix& m, const double* b, int n, double* c); //умножение матрицы на вектор-столбец
	static void Multi_T(const Matrix& m, const double* b, int n, double* c);//”множение транспонированной матрицы
	static void SumVector(double* a, const double* b, int n); //сложение векторов
	double& operator()(int i, int j);
	friend std::ostream& operator << (std::ostream& os, const Matrix& m);
	friend std::istream& operator >> (std::istream& is, Matrix& m);
};

