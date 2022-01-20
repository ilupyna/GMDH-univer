#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;


//template <class T>
//bool operator ==(const vector<T>& lhs, const vector<T>& rhs) {
//	if(lhs.size() == rhs.size()) {
//		for(int i=0; i<lhs.size(); i++) {
//			if(lhs.at(i) != rhs.at(i)) {
//				return false;
//			}
//		}
//		return true;
//	}
//	return false;
//}
//
//template <class T>
//bool operator <(const vector<T>& lhs, const vector<T>& rhs) {
//	if(lhs.size() == rhs.size()) {
//		for(int i=0; i<lhs.size(); i++) {
//			if(lhs.at(i) != rhs.at(i)) {
//				return lhs.at(i) < rhs.at(i);
//			}
//		}
//		return false;
//	}
//	return lhs.size() < rhs.size();
//}

template <class T>
ostream& operator << (ostream& os, const vector<T>& v) {
	os << "{";
	bool first = true;
	for (const auto& x : v) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "}";
}

void print(const vector<vector<double>>& v) {
	for(auto ryad:v) {
		for(auto el:ryad) {
			cout<< el <<"\t";
		}
		cout<<endl;
	}
	cout<<endl;
}

vector<double> gaussa(vector<vector<double>> arr){
	const int Size = arr.size();
	vector<double> output(arr.size());
	
	vector<vector<double>> arr_clone = arr;
    
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

struct Solution {
	vector<vector<int>> func;
	vector<double> A;
	double diff;
};

class MGUA {
	const vector<vector<double>> X_origin;
	const vector<double> Y_origin;
	
	vector<vector<double>> X_learn, X_check;
	vector<double> Y_learn, Y_check;
	
	vector<int> learn_pos, check_pos;
	
//	vector<vector<double>> X_add;
//	vector<double> Y_add;
	
	vector<vector<int>> combinations;
	vector<vector<vector<int>>> functions;
	
	vector<Solution> solutions;
	
	const int learn_amount;
	int level = 1;
	int pos = 0;
	
	
	void CreatePolynomCombinations() {
//		cout<<"CreatePolynomCombinations"<<endl;
		/*
		for(int i0=0; i0<X_origin.front().size(); i0++) {
			combinations.push_back({i0});
		}
		*/
		for(int i0=0; i0<X_origin.front().size(); i0++) {
			for(int i1=i0+1; i1<X_origin.front().size(); i1++) {	//i1=i0+1
				combinations.push_back({i0, i1});
			}
		}
		/*
		for(int i0=0; i0<X_origin.front().size(); i0++) {
			combinations.push_back({i0});
			for(int i1=i0+1; i1<X_origin.front().size(); i1++) {
				combinations.push_back({i0, i1});
//				for(int i2=i1+1; i2<X_origin.front().size(); i2++) {
//					combinations.push_back({i0, i1, i2});
//					for(int i3=i2+1; i3<X_origin.front().size(); i3++) {
//						combinations.push_back({i0, i1, i2, i3});
//					}
//				}
			}
		}
		*/
//		combinations.push_back({0, 1, 2, 3, 4});
		
//		cout<<endl;
	}
	
	
	void CreateFunctinsFromCombinations() {
		/*
		for(int i0=0; i0<combinations.size(); i0++) {
			functions.push_back({combinations.at(i0)});
		}
		
		for(int i0=0; i0<combinations.size(); i0++) {
			for(int i1=i0+1; i1<combinations.size(); i1++) {
				functions.push_back({combinations.at(i0), combinations.at(i1)});
			}
		}
		*/
		for(int i0=0; i0<combinations.size(); i0++) {
			functions.push_back({	combinations.at(i0)	});
			/*
			for(int i1=i0+1; i1<combinations.size(); i1++) {
				functions.push_back({	combinations.at(i0), 
										combinations.at(i1)	});
				
				for(int i2=i1+1; i2<combinations.size(); i2++) {
					functions.push_back({	combinations.at(i0), 
											combinations.at(i1), 
											combinations.at(i2)	});
					
					for(int i3=i2+1; i3<combinations.size(); i3++) {
						functions.push_back({	combinations.at(i0), 
												combinations.at(i1), 
												combinations.at(i2), 
												combinations.at(i3)	});
					}
				}
			}
			*/
		}
//		sort(functions.begin(), functions.end());
		
		cout<<"funcs:"<< functions <<endl;
	}
	
	
public:
	MGUA(	const vector<vector<double>>& X_inp, 
			const vector<double>& Y_inp, 
			const double navch_chastka = 0.85	)
		:
			X_origin(X_inp), 
			Y_origin(Y_inp), 
			learn_amount(Y_inp.size()*navch_chastka)
	{
		cout<<"learn:"<< learn_amount <<";\t check:"<< Y_origin.size()-learn_amount <<endl;
		
		for(int i=0; i<X_origin.size(); i++) {
			if(i % (X_origin.size()/(X_origin.size()-learn_amount)) == 0) {
				check_pos.push_back(i);
			}
			else {
				learn_pos.push_back(i);
			}
		}
	}
	
	
	void Start() {
		// y = c0 + c1x
		// y = c0 + c1x + c2x^2
		// y = c0 + c1x + c2x^2 + c3x^3 + c4x^4
		
		CreatePolynomCombinations();
//		cout<<"combs:\n"<< combinations <<endl;
		CreateFunctinsFromCombinations();
//		cout<<"funcs:\n"<< functions <<endl;
//		cout<<"Set for max Kalmogorov-gabor polynom created"<<endl;
//		cout<<endl;
		
		GetNextSolution();
//		cout<<"First solution created"<<endl;
//		cout<<endl;
		do {
			GetNextSolution();
		}
		while(LastSolutionIsBetter() && pos++ < functions.size());
		
		GenerateNextPairs();
		
		const auto best = min_element(solutions.begin(), solutions.end(), [](const auto& lhs, const auto& rhs){
			return lhs.diff < rhs.diff;
		});
		
		cout<<"learn:"<<endl;
		for(const auto i:learn_pos) {
			for(int j=0; j<X_origin.front().size(); j++) {
				cout<< X_origin.at(i).at(j) <<"\t";
			}
			cout<<" |\t"<< Y_origin.at(i) <<endl;
		}
		cout<<endl;
		cout<<"check:"<<endl;
		for(const auto i:check_pos) {
			for(int j=0; j<X_origin.front().size(); j++) {
				cout<< X_origin.at(i).at(j) <<"\t";
			}
			cout<<" |\t"<< Y_origin.at(i) <<endl;
		}
		cout<<endl;
		
		cout<<"Solution:\ny = a0";
		for(int i=0; i<best->func.size(); i++) {
			cout<<" + a"<< i+1 <<"*X"<< best->func.at(i);
		}
		cout<<"}\n"<< best->A <<"\nWith rel_y: "<< best->diff <<endl;
		cout<<endl;
	}
	
	
private:
	bool LastSolutionIsBetter() const {
//		return false;
//		return solutions.size() < 20;
//		return pos < functions.size()-1;
		return solutions.size() < functions.size();
//		return solutions.size() < combinations.size() - X_origin.front().size();
//		return solutions.at(solutions.size()-1).second < solutions.at(solutions.size()-2).second;
	}
	
	
	void GenerateNextPairs() {
		sort(solutions.begin(), solutions.end(), [](auto lhs, auto rhs){
			return lhs.diff < rhs.diff;
		});
		
		for(const auto el:solutions) {
			cout<<": "<< el.func <<endl;
		}
		cout<<endl;
		
		combinations.clear();
		for(int i=0; i<3; i++) {
			//combinations.push_back(solutions.at(i).func);
		}
		cout<<"new combs: "<< combinations <<endl;
	}
	
	
	const vector<vector<int>> GetFunction() const {
//		cout<<"GetFunction:\n"<< functions.at(pos) <<endl;
		return functions.at(pos);
	}
	
	
	void GetNextSolution() {
//		cout<<"\nGetNextSolution"<<endl;
		if(pos == combinations.size()-1) {
			++level;
		}
		const auto new_function = GetFunction();
		const vector<double> solution = GetSolutionForFunction(new_function);
		const double diff = CheckSolution(solution);
		
//		cout<<"func:"<< new_function <<endl 
//			<<"solt:"<< solution <<endl 
//			<<"diff:"<< diff <<endl
//			<<endl; 
		
		solutions.push_back({new_function, solution, diff});
	}
	
	
	void SetArrayForMNK(const vector<vector<int>> function) {
//		cout<<"SetArrayForMNK"<<endl;
//		cout<<"func: "<< function <<endl;
		X_learn.clear();
		X_check.clear();
		Y_learn.clear();
		Y_check.clear();
				
		X_learn.reserve(learn_amount+1);
		X_check.reserve(X_origin.size()-learn_amount+1);
		Y_learn.reserve(learn_amount+1);
		Y_check.reserve(X_origin.size()-learn_amount+1);
		
		for(int i=0; i<X_origin.size(); i++) {
			vector<double> new_ryad = {1};
			new_ryad.reserve(level+1);
			
			for(int tmp_level=0; tmp_level<function.size(); tmp_level++) {
				double tmp = 1;
				for(auto j:function.at(tmp_level)) {
					tmp *= X_origin[i][j];
//					cout<<"i:"<< i <<"; j:"<< j <<"; |"<< X_origin[i][j] <<"| \t";
				}
//				cout<<" = "<< tmp <<endl;
				new_ryad.push_back(tmp);
			}
//			cout<<endl;
			if(i % (X_origin.size()/(X_origin.size()-learn_amount)) == 0) {
				X_check.push_back(new_ryad);
				Y_check.push_back(Y_origin.at(i));
			}
			else {
				X_learn.push_back(new_ryad);
				Y_learn.push_back(Y_origin.at(i));
			}
		}
//		cout<<"learn:"<<endl;
//		print(X_learn);
//		cout<<"check:"<<endl;
//		print(X_check);
//		cout<<"Vubirku podileno na navchalnu i perevirochnu"<<endl;
	}
	
	
	vector<double> GetSolutionForFunction(const vector<vector<int>> function) {
//		cout<<"GetSolutionForCombination"<<endl;
		
//		cout<<"Setting array for function"<<endl;
//		cout<< function <<endl;
		SetArrayForMNK(function);
//		cout<<"learn:"<<endl;
//		print(X_learn);
//		cout<<"check:"<<endl;
//		print(X_check);
		
//		cout<<"Setting system from array"<<endl;
		vector<vector<double>> system_rivnyan;
		for(int k=0; k<X_learn.front().size(); k++) {
			vector<double> tmp_ryad;
			for(int i=0; i<X_learn.front().size(); i++) {
				double tmp = 0;
				for(int j=0; j<X_learn.size(); j++) {
					tmp += X_learn[j][i] * X_learn[j][k];
				}
				tmp_ryad.push_back(tmp);
			}
			system_rivnyan.push_back(tmp_ryad);
		}
		for(int i=0; i<X_learn.front().size(); i++) {
			double tmp = 0;
			for(int j=0; j<X_learn.size(); j++) {
				tmp += X_learn[j][i] * Y_learn[j];
			}
			system_rivnyan[i].push_back(tmp);
		}
//		cout<<"system:"<<endl;
//		print(system_rivnyan);
		
		return gaussa(system_rivnyan);
	}
	
	
	double CheckSolution(const vector<double> solution) const {
//		cout<<"CheckSolution"<<endl;
		double diff = 0;
		for(int i=0; i<X_check.size(); i++) {
			double sum = 0;
			for(int j=0; j<X_check.front().size(); j++) {
				sum += X_check[i][j] * solution[j];
//				cout<< X_check[i][j] <<"\t";
			}
			diff += pow(Y_check[i] - sum, 2);
//			cout<< Y_check[i] <<" | "<< sum <<endl;
		}
//		cout<<"diff = "<< diff <<endl;
		return diff / Y_check.size();
	}
};


int main() {
//	cout<<"start"<<endl;
	cout<<"RHR by I.Lupuna"<<endl;
	cout<<"MGUA method"<<endl;
	cout<<endl;
	const vector<vector<double>> inp = {
		{1, 0.1, 1000, 37.5, 0.6},
		{1, 0.25, 1600, 37.3, 0.1},
		{1, 0.4, 500, 36.8, 0.3},
		
		{2, 0.1, 1000, 36.8, 0.6},
		{2, 0.25, 1600, 37.3, 0.1},
		{2, 0.4, 500, 36.8, 0.3},
		
		{3, 0.1, 1600, 37.5, 0.3},
		{3, 0.1, 1000, 37.5, 0.6},
		{3, 0.4, 500, 36.8, 0.1},
		
		{4, 0.1, 1600, 37.7, 0.1},
		{4, 0.25, 1000, 37.3, 0.6},
		{4, 0.4, 500, 36.8, 0.3},
		
		{5, 0.1, 1600, 36.8, 0.3},
		{5, 0.25, 500, 37.3, 0.6},
		{5, 0.4, 1000, 36.8, 0.1},
		
		{6, 0.1, 1600, 36.8, 0.3},
		{6, 0.25, 500, 37.3, 0.6},
		{6, 0.4, 1000, 36.8, 0.1}
	};
	const vector<double> y1 = {101.8, 71.2, 95.2, 119.3, 58.1, 1023.2, 183.9, 166, 71.6, 38.9, 262.6, 823.1, 327.2, 223.7, 89.1, 246.4, 211, 66.8};
	const vector<double> y2 = {81.3, 129.3, 177.4, 81.3, 118.3, 432.4, 136.7, 133, 73.9, 33.3, 280.9, 291.9, 85, 251.3, 85, 70.2, 299.3, 70.2};
	const vector<double> y3 = {67.3, 127.1, 145.2, 205.5, 78.9, 375.9, 151.3, 128.1, 113.6, 140.7, 145.7, 111.6, 93, 131.7, 136.7, 205.5, 96.5, 119.6};
	
	cout<<"Data:"<<endl;
	cout<<"Type\tt(mm)\tn(1/hv)\tD(mm)\ts(mm/1)\t |\tPx(H)\tPy(H)\tPz(H)"<<endl;
	for(int i=0; i<inp.size(); i++) {
		for(auto el:inp.at(i)) {
			cout<< el <<"\t";
		}
		cout<<" |\t"<< y1[i] <<"\t"<< y2[i] <<"\t"<< y3[i] <<endl;
	}
	cout<<endl;
	
	cout<<"First y"<<endl;
	MGUA s1(inp, y1);
	s1.Start();
	/*
	cout<<"Second y"<<endl;
	MGUA s2(inp, y2);
	s2.Start();
	
	cout<<"Third y"<<endl;
	MGUA s3(inp, y3);
	s3.Start();
	*/
//	cout<<"fin"<<endl;
		
	return 0;
}
