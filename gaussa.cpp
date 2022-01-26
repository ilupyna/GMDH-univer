#include "gaussa.h"


std::vector<double> gaussa(std::vector<std::vector<double>> arr){
	const int Size = arr.size();
	std::vector<double> output(arr.size());
	
	auto arr_clone = arr;
    
    //Прямой ход (Зануление нижнего левого угла)
    for (int k=0; k<Size; k++)	//k-номер строки
    {
        for (int i=0; i<Size+1; i++)	//i-номер столбца
            arr_clone[k][i] /= arr[k][k];	//Деление k-строки на первый член !=0 для преобразования его в единицу
        for (int i=k+1; i<Size; i++)	//i-номер следующей строки после k
        {
        const double koef = arr_clone[i][k] / arr_clone[k][k];	//Коэффициент
            for (int j=0; j<Size+1; j++)	//j-номер столбца следующей строки после k
                arr_clone[i][j] -= arr_clone[k][j] * koef; //Зануление элементов матрицы ниже первого члена, преобразованного в единицу
        }
        for (int i=0; i<Size; i++)	//Обновление, внесение изменений в начальную матрицу
            for (int j=0; j<Size+1; j++)
                arr[i][j] = arr_clone[i][j];
    }
    
    //Обратный ход (Зануление верхнего правого угла)
    for (int k=Size-1; k>=0; k--)	//k-номер строки
    {
        for (int i=Size; i>=0; i--)	//i-номер столбца
            arr_clone[k][i] /= arr[k][k];
        for (int i=k-1; i>=0; i--)	//i-номер следующей строки после k
        {
            const double koef = arr_clone[i][k] / arr_clone[k][k];
            for (int j=Size; j>=0; j--) //j-номер столбца следующей строки после k
                arr_clone[i][j] -= arr_clone[k][j] * koef;
        }
    }
    
    //output
    for (int i=0; i<Size; i++)
        output[i] = arr_clone[i][Size];
    
    return output;
}
