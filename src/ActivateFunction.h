#pragma once
#include <iostream>
enum activateFunc {sigmoid = 1, ReLU, thx}; //перечисляемый тиа
class ActivateFunction
{
	activateFunc actFunc;
public:
	void set(); //выбор активационной функции
	void use(double* value, int n); //задание функций
	void useDer(double* value, int n); //задание производной функции
	double useDer(double value);
};

