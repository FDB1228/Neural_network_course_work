#pragma once
#include "ActivateFunction.h"
#include "Matrix.h"
#include <fstream>
using namespace std;
struct data_NetWork {
	int L;
	int* size;
};
class NetWork
{
	int L; //количество слоев нейросети 
	int* size; // нейронов на каждом слое 
	ActivateFunction actFunc; // активационная функция
	Matrix* weights; //матрица весов
	double** bios; //веса смещения
	double** neurons_val, ** neurons_err; //значения нейронов и ошибки нейронов
	double* neurons_bios_val; // значение нейронов смещения
public:
	void Init(data_NetWork data); //инициализация объектов
	void PrintConfig(); // вывод начальной информации о нейросети
	void SetInput(double* values); // преобразование входных данный во входные нейроны

	double ForwardFeed(); // функция прямого распростронения
	int SearchMaxIndex(double* values); // определение индекса максимального значения выходного нейрона
	void PrintValues(int L); // вывод найденого значения

	void BackPropogation(double expect); // функция обратного распространения
	void WeightsUpdater(double lr); // обновляет веса

	void SaveWeights(); //записывает веса в файл
	void Readweights(); //считывает веса из файла
};

