#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;


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
	
	vector<int> learn_pos, check_pos;
	
	vector<vector<double>> X_learn, X_check;
	vector<double> Y_learn, Y_check;
	
	vector<vector<vector<int>>> functions;
	
	vector<vector<Solution>> solutions_by_iterations;
	Solution best_solution;
	
	
	void FirstSetup() {
		for(int i0=0; i0<X_origin.front().size(); i0++) {
			for(int i1=i0; i1<X_origin.front().size(); i1++) {	//i1=i0+1
				functions.push_back({{i0, i1}});
			}
		}
	}
	
	
	void GetFunctionsFromBestSolutions(const int count=8) {
		vector<vector<vector<int>>> variables;
		for(int i=0; i<count && i<solutions_by_iterations.back().size(); i++) {
			variables.push_back(solutions_by_iterations.back().at(i).func);
		}
		functions.clear();
		
//		cout<<"variables: "<< variables <<endl;
		
		for(int i0=0; i0<variables.size(); i0++) {
			for(int i1=i0; i1<variables.size(); i1++) {
				vector<vector<int>> tmp;
				
				for(auto el:variables.at(i0)) {
					tmp.push_back(el);
				}
				for(auto el:variables.at(i1)) {
					tmp.push_back(el);
				}
//				sort(tmp.begin(), tmp.end());
//				if(unique(tmp.begin(), tmp.end()) == tmp.end()) {
					functions.push_back(tmp);
//				}
			}
		}
//		cout<<"funcs:"<< functions <<endl;
	}
	
	
	void SortByDispX(const int learn_amount) {
		vector<double> Ser;
		vector<pair<int, double>> Disp;
		
		{
			for(int i=0; i<X_origin.front().size(); i++) {
				double sum = 0;
				for(int j=0; j<X_origin.size(); j++) {
					sum += X_origin.at(j).at(i);
				}
				Ser.push_back(sum/X_origin.size());
			}
		}
		{
			for(int i=0; i<X_origin.size(); i++) {
				double sum = 0;
				for(int j=0; j<X_origin.front().size(); j++) {
					sum += pow(X_origin.at(i).at(j) - Ser.at(j), 2);
				}
				Disp.push_back({i, sum});
			}
		}
		
		sort(Disp.begin(), Disp.end(), [](const auto& lhs, const auto& rhs){
			return lhs.second > rhs.second;
		});
		
		for(int i=0; i<learn_amount; i++) {
			learn_pos.push_back(Disp.at(i).first);
		}
		for(int i=learn_amount; i<X_origin.size(); i++) {
			check_pos.push_back(Disp.at(i).first);
		}
	}
	
	void SortByDispY(const int learn_amount) {
		vector<double> Ser;
		vector<pair<int, double>> Disp;
		
		double ser = 0;
		{
			for(int i=0; i<Y_origin.size(); i++) {
				ser += Y_origin.at(i);
			}
			ser /= Y_origin.size();
		}
		{
			for(int i=0; i<Y_origin.size(); i++) {
				Disp.push_back({i, pow(Y_origin.at(i) - ser, 2)});
			}
		}
		
		sort(Disp.begin(), Disp.end(), [](const auto& lhs, const auto& rhs){
			return lhs.second > rhs.second;
		});
		
		for(int i=0; i<learn_amount; i++) {
			learn_pos.push_back(Disp.at(i).first);
		}
		for(int i=learn_amount; i<X_origin.size(); i++) {
			check_pos.push_back(Disp.at(i).first);
		}
	}
	
	void SortByPos(const int learn_amount) {
		const int every_n_for_check = X_origin.size()/(X_origin.size() - learn_amount);
		
		for(int i=0; i<X_origin.size(); i++) {
			if(i % every_n_for_check == 0) {
				check_pos.push_back(i);
			}
			else {
				learn_pos.push_back(i);
			}
		}
	}
	
	
public:
	MGUA(	const vector<vector<double>>& X_inp, 
			const vector<double>& Y_inp, 
			const double navch_chastka = 0.85	)
		:
			X_origin(X_inp), 
			Y_origin(Y_inp)
	{
		if(X_inp.size() != Y_inp.size()) {
			cout<<"Wrong data size"<<endl;
			throw(1);
		}
		for(int i=1; i<X_inp.size(); i++) {
			if(X_inp.at(i).size() != X_inp.at(i-1).size()) {
				cout<<"Wrong data size"<<endl;
				throw(2);
			}
		}
		
		const int learn_amount = X_origin.size()*navch_chastka;
		
		cout<<"learn:"<< learn_amount <<";\t check:"<< X_origin.size()-learn_amount <<endl;
		cout<<endl;
		
//		SortByDispX(learn_amount);
		SortByDispY(learn_amount);
//		SortByPos(learn_amount);
		
		cout<<"Learn data:"<<endl;
		for(const auto i:learn_pos) {
			for(int j=0; j<X_origin.at(i).size(); j++) {
				cout<< X_origin.at(i).at(j) <<"\t";
			}
			cout<<"\t|\t"<< Y_origin.at(i)<<endl;
		}
		cout<<endl;
		cout<<"Check data:"<<endl;
		for(const auto i:check_pos) {
			for(int j=0; j<X_origin.at(i).size(); j++) {
				cout<< X_origin.at(i).at(j) <<"\t";
			}
			cout<<"\t|\t"<< Y_origin.at(i)<<endl;
		}
		cout<<endl;
	}
	
	
	void Start() {
		// y = c0 + f(x1,x2) 	// f(x_n-1, x_n)
		// z = c0 + f(y1, y2)
		
		FirstSetup();
		
		do {
			solutions_by_iterations.push_back({});
			GetSolutionFromFunctions();
			
			if(solutions_by_iterations.back().size() == 0) {
				cout<<"there is no good solutions on "<< solutions_by_iterations.size() <<" iteration"<<endl;
				cout<<endl;
				solutions_by_iterations.pop_back();
				break;
			}
			
			sort(solutions_by_iterations.back().begin(), solutions_by_iterations.back().end(), [](const auto& lhs, const auto& rhs){
				return lhs.diff < rhs.diff;
			});
			cout<<"Top3 solutions on "<< solutions_by_iterations.size() <<" iteration:"<<endl;
			for(int i=0; i<solutions_by_iterations.back().size() && i<3; i++) {
				cout<< solutions_by_iterations.back().at(i).func <<"\nwith standart deviation: "
					<< solutions_by_iterations.back().at(i).diff <<endl;
			}
			cout<<endl;
			
			GetFunctionsFromBestSolutions();
		}
		while(NeedAnotherIteration());
		
		
		cout<<"So best solution is:\ny = a0";
		best_solution = min_element(solutions_by_iterations.begin(), solutions_by_iterations.end(), [](const auto& lhs, const auto& rhs){
			return lhs.front().diff < rhs.front().diff;
		})->front();
		for(int i=0; i<best_solution.func.size(); i++) {
			cout<<" + a"<< i+1 <<"*X"<< best_solution.func.at(i);
		}
		cout<<"\n"<< best_solution.A <<"\nWith standart deviation: "<< best_solution.diff <<endl;
		cout<<endl;
	}
	
	
	void FreeCheck() const {
		cout<<"Vvedit znachennya X: "<<endl;
		vector<double> X_input;
		double Y_input;
		for(int i=0; i<X_origin.front().size(); i++) {
			double tmp;
			cin >> tmp;
			X_input.push_back(tmp);
		}
		cout<<"Vvedit znachennya Y: ";
		cin >> Y_input;
		cout<<endl;
		
		vector<double> equation = {1};
		for(int tmp_level=0; tmp_level<best_solution.func.size(); tmp_level++) {
			double tmp = 1;
			for(auto i:best_solution.func.at(tmp_level)) {
				tmp *= X_input.at(i);
			}
			equation.push_back(tmp);
		}
		/*
		vector<double> check = {1, 1, 0.2, 1400, 37.4, 0.2};
		const double y_check = 80;
		*/
		double sum = 0;
		for(int i=0; i<best_solution.A.size(); i++) {
			sum += equation[i] * best_solution.A.at(i);
		}
		cout<<"y origin: "<< Y_input <<endl
			<<"y rozrah: "<< sum <<endl
			<<"standart deviation: "<< pow(Y_input - sum, 2) <<endl;
		cout<<endl;
	}
	
	
private:
	bool NeedAnotherIteration() const {
		if(solutions_by_iterations.back().size() < 2) {
			return false;
		}
		if(solutions_by_iterations.size() < 2) {
			return true;
		}
//		return true;
		return solutions_by_iterations.at(solutions_by_iterations.size()-1).front().diff < solutions_by_iterations.at(solutions_by_iterations.size()-2).front().diff;
	}
	
	
	void GetSolutionFromFunctions() {
//		cout<<"\nGetsolutions_by_iterationsFromFunctions"<<endl;
		for(const auto& func:functions) {
			const vector<double> solution = GetSolutionForFunction(func);
			const double diff = CheckSolution(solution);
			if(!isnan(diff)) {
//				cout<<"NOT isNaN"<<endl;
				solutions_by_iterations.back().push_back({func, solution, diff});
			}
//			cout<<"func:"<< new_function <<endl 
//				<<"solt:"<< solution <<endl 
//				<<"diff:"<< diff <<endl
//				<<endl;
		}
	}
	
	
	void SetArrayForMNK(const vector<vector<int>> function) {
//		cout<<"SetArrayForMNK"<<endl;
		X_learn.clear();
		X_check.clear();
		Y_learn.clear();
		Y_check.clear();
		
//		X_learn.reserve(learn_amount+1);
//		X_check.reserve(X_origin.size()-learn_amount+1);
//		Y_learn.reserve(learn_amount+1);
//		Y_check.reserve(X_origin.size()-learn_amount+1);
		
		for(const int i:check_pos) {
			vector<double> new_ryad = {1};
			for(int tmp_level=0; tmp_level<function.size(); tmp_level++) {
				double tmp = 1;
				for(auto j:function.at(tmp_level)) {
					tmp *= X_origin[i][j];
				}
				new_ryad.push_back(tmp);
			}
			X_check.push_back(new_ryad);
			Y_check.push_back(Y_origin.at(i));
		}
		for(const int i:learn_pos) {
			vector<double> new_ryad = {1};
			for(int tmp_level=0; tmp_level<function.size(); tmp_level++) {
				double tmp = 1;
				for(auto j:function.at(tmp_level)) {
					tmp *= X_origin[i][j];
				}
				new_ryad.push_back(tmp);
			}
			X_learn.push_back(new_ryad);
			Y_learn.push_back(Y_origin.at(i));
		}
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
			}
			diff += pow(Y_check[i] - sum, 2);
		}
		return diff / Y_check.size();
	}
};


const vector<vector<double>> Read_X_FromFile(const string FILENAME = "input x.txt") {
	cout<<"Read_X_FromFile"<<endl;
	ifstream in(FILENAME);
	if(in.is_open()){
		vector<vector<double>> X;
		for(string line; getline(in, line); ) {
			istringstream iss(line);
			X.push_back({});
			for(double tmp; iss>>tmp; ) {
				X.back().push_back(tmp);
			}
		}
		cout<<"Data readed"<<endl;
		in.close();
		return X;
	}
	else cout<<"Can't open input file"<<endl;
}
const vector<double> Read_Y_FromFile(const string FILENAME = "input y1.txt") {
	cout<<"Read_Y_FromFile"<<endl;
	ifstream in(FILENAME);
	if(in.is_open()){
		vector<double> Y;
		for(double tmp; in>>tmp; ) {
			Y.push_back(tmp);
		}
		cout<<"Data readed"<<endl;
		in.close();
		return Y;
	}
	else cout<<"Can't open input file"<<endl;
}
const pair<vector<vector<double>>, vector<vector<double>>> ReadFromFile(const string FILENAME = "input.txt") {
//	cout<<"ReadFromFile"<<endl;
	ifstream in(FILENAME);
	if(in.is_open()){
		int n_for_x, n_for_y;
		in >> n_for_x >> n_for_y;
		
		vector<vector<double>> X, Y(n_for_y);
		while(!in.eof()) {
			X.push_back({});
			double tmp;
			for(int i=0; i<n_for_x; i++) {
				in >> tmp;
				X.back().push_back(tmp);
			}
			for(int i=0; i<n_for_y; i++) {
				in >> tmp;
				Y.at(i).push_back(tmp);
			}
		}
//		cout<<"Data readed"<<endl;
		in.close();
		return {X, Y};
	}
	else {
		cout<<"Can't open input file"<<endl;
	}
}


int main() {
	cout<<"RHR by I.Lupuna"<<endl;
	cout<<"MGUA method"<<endl;
	cout<<endl;
	
	cout<<"Write filename for input data (first line is 'x y', where 'x' is count of Xn in every line and 'y' is count of Yn in every line"<<endl;
	cout<<"Filename: ";
	string filename;
	cin>> filename;
	const auto [X, Y] = ReadFromFile(filename);
	
	cout<<"Data:"<<endl;
	cout<<"Type\tt(mm)\tn(1/hv)\tD(mm)\ts(mm/1)\t |\tPx(H)\tPy(H)\tPz(H)"<<endl;
	for(int i=0; i<X.size(); i++) {
		for(const auto el:X.at(i)) {
			cout<< el <<"\t";
		}
		cout<<" |\t";
		for(int j=0; j<Y.size(); j++) {
			cout<< Y.at(j).at(i) <<"\t";
		}
		cout<<endl;
	}
	cout<<endl;
	
	cout<<"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<endl;
	cout<<"First y"<<endl;
	MGUA s1(X, Y[0]);
	s1.Start();
	
	cout<<"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<endl;
	cout<<"Second y"<<endl;
	MGUA s2(X, Y[1]);
	s2.Start();
	
	cout<<"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<endl;
	cout<<"Third y"<<endl;
	MGUA s3(X, Y[2]);
	s3.Start();
	
	cout<<"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<endl;
	cout<<"Free Check for First y"<<endl;
	s1.FreeCheck();
	cout<<endl;
	
	cout<<"Free Check for Second y"<<endl;
	s2.FreeCheck();
	cout<<endl;
	
	cout<<"Free Check for Third y"<<endl;
	s3.FreeCheck();
	cout<<endl;
	/**/
//	cout<<"fin"<<endl;
		
	return 0;
}
