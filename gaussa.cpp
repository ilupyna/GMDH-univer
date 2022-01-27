#include "gaussa.h"


std::vector<double> gaussa(std::vector<std::vector<double>> arr){
	const int Size = arr.size();
	std::vector<double> output(arr.size());
	
	auto arr_clone = arr;
	
	//Forward elimination (zeroing lower left corner)
	for (int k=0; k<Size; k++)	//k - line number
	{
		for (int i=0; i<Size+1; i++)	//i - column number
			arr_clone[k][i] /= arr[k][k];	//Dividing the k-row by the first member !=0 to convert it to one
		for (int i=k+1; i<Size; i++)	//i - number of the next row after k
		{
			const double koef = arr_clone[i][k] / arr_clone[k][k];	//Coefficient
			for (int j=0; j<Size+1; j++)	//j - column number of the next row after k
				arr_clone[i][j] -= arr_clone[k][j] * koef; //Zeroing matrix elements below the first member converted to one
		}
		for (int i=0; i<Size; i++)	//Update, make changes to the initial matrix
			for (int j=0; j<Size+1; j++)
				arr[i][j] = arr_clone[i][j];
	}
	
	//Back substitution (zeroing of the upper right corner)
	for (int k=Size-1; k>=0; k--)	//k - line number
	{
		for (int i=Size; i>=0; i--)	//i - column number
			arr_clone[k][i] /= arr[k][k];
		for (int i=k-1; i>=0; i--)	//i - number of the next row after k
		{
			const double koef = arr_clone[i][k] / arr_clone[k][k];
			for (int j=Size; j>=0; j--) //j - column number of the next row after k
				arr_clone[i][j] -= arr_clone[k][j] * koef;
		}
	}
	
	//output
	for (int i=0; i<Size; i++)
		output[i] = arr_clone[i][Size];
	
	return output;
}
