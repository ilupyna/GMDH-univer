#include "gaussa.h"


std::vector<double> gaussa(std::vector<std::vector<double>> arr){
	const int Size = arr.size();
	std::vector<double> output(arr.size());
	
	auto arr_clone = arr;
    
    //������ ��� (��������� ������� ������ ����)
    for (int k=0; k<Size; k++)	//k-����� ������
    {
        for (int i=0; i<Size+1; i++)	//i-����� �������
            arr_clone[k][i] /= arr[k][k];	//������� k-������ �� ������ ���� !=0 ��� �������������� ��� � �������
        for (int i=k+1; i<Size; i++)	//i-����� ��������� ������ ����� k
        {
        const double koef = arr_clone[i][k] / arr_clone[k][k];	//�����������
            for (int j=0; j<Size+1; j++)	//j-����� ������� ��������� ������ ����� k
                arr_clone[i][j] -= arr_clone[k][j] * koef; //��������� ��������� ������� ���� ������� �����, ���������������� � �������
        }
        for (int i=0; i<Size; i++)	//����������, �������� ��������� � ��������� �������
            for (int j=0; j<Size+1; j++)
                arr[i][j] = arr_clone[i][j];
    }
    
    //�������� ��� (��������� �������� ������� ����)
    for (int k=Size-1; k>=0; k--)	//k-����� ������
    {
        for (int i=Size; i>=0; i--)	//i-����� �������
            arr_clone[k][i] /= arr[k][k];
        for (int i=k-1; i>=0; i--)	//i-����� ��������� ������ ����� k
        {
            const double koef = arr_clone[i][k] / arr_clone[k][k];
            for (int j=Size; j>=0; j--) //j-����� ������� ��������� ������ ����� k
                arr_clone[i][j] -= arr_clone[k][j] * koef;
        }
    }
    
    //output
    for (int i=0; i<Size; i++)
        output[i] = arr_clone[i][Size];
    
    return output;
}
