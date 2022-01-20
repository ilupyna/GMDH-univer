//#include "mhua.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <exception>

#include "adds.cpp"
#include "gaussa.cpp"

using namespace std;

struct Solution {
	vector<vector<int>> func;
	vector<double> A;
	double diff;
};


struct Positions {
	vector<int> train, check;
	
	enum Gen:int { DispY, DispX, Pos, EveryN };
};


struct InputData {
	const vector<vector<double>> X;
	const vector<double> Y;
	
	InputData(const vector<vector<double>>& X_inp, const vector<double>& Y_inp)
		:X(X_inp), Y(Y_inp) 
	{
		if(X_inp.size() != Y_inp.size()) {
			cout<<"Wrong data size"<<endl;
			throw invalid_argument("Dimensions X and Y are not the same");
		}
		if(X_inp.size() < 3) {
			cout<<"There is not enough data to define check and train data"<<endl;
			throw invalid_argument("There is no X data");
		}
		for(int i=1; i<X_inp.size(); i++) {
			if(X_inp.at(i).size() != X_inp.at(i-1).size()) {
				cout<<"Wrong data size"<<endl;
				throw invalid_argument("Dimensions X in every line are not the same");
			}
		}
	}
	
	const int variables() const {
		return X.front().size();
	}
	
	const int size() const {
		return X.size();
	}
};


class ArraySetupForMNK {
	vector<vector<double>> X_train, X_check;
	vector<double> Y_train, Y_check;
	
	vector<double> solution;
	double diff;
	
public:
	void SetupAndFind(	const vector<vector<int>>& function, 
						const Positions& positions,
						const vector<vector<double>>& X, 
						const vector<double>& Y ) 
	{
		Setup(function, positions, X, Y);
		Solve();
		Check();
	}
	
	const vector<double> GetSolution() const {
		return solution;
	}
	
	const double GetDiff() const {
		return diff;
	}
	
private:
	void ArrClear() {
		X_train.clear();
		X_check.clear();
		Y_train.clear();
		Y_check.clear();
	}
	
	void ArrReserve(const int train_size, const int check_size) {
		X_train.reserve(train_size);
		Y_train.reserve(train_size);
		
		X_check.reserve(check_size);
		Y_check.reserve(check_size);
	}
	
	void Setup(	const vector<vector<int>>& function, 
				const Positions& positions,
				const vector<vector<double>>& X, 
				const vector<double>& Y) 
	{
		ArrClear();
		ArrReserve(positions.train.size(), positions.check.size());
		
		for(const int i:positions.train) {
			vector<double> new_ryad = {1};
			for(int tmp_level=0; tmp_level<function.size(); tmp_level++) {
				double tmp = 1;
				for(auto j:function.at(tmp_level)) {
					tmp *= X.at(i).at(j);
				}
				new_ryad.push_back(tmp);
			}
			X_train.push_back(new_ryad);
			Y_train.push_back(Y.at(i));
		}
		for(const int i:positions.check) {
			vector<double> new_ryad = {1};
			for(int tmp_level=0; tmp_level<function.size(); tmp_level++) {
				double tmp = 1;
				for(auto j:function.at(tmp_level)) {
					tmp *= X.at(i).at(j);
				}
				new_ryad.push_back(tmp);
			}
			X_check.push_back(new_ryad);
			Y_check.push_back(Y.at(i));
		}
	}
	
	void Solve() {
//		cout<<"Setting system from array"<<endl;
		vector<vector<double>> system_rivnyan;
		
		for(int k=0; k<X_train.front().size(); k++) {
			vector<double> tmp_ryad;
			for(int i=0; i<X_train.front().size(); i++) {
				double tmp = 0;
				for(int j=0; j<X_train.size(); j++) {
					tmp += X_train[j][i] * X_train[j][k];
				}
				tmp_ryad.push_back(tmp);
			}
			system_rivnyan.push_back(tmp_ryad);
		}
		for(int i=0; i<X_train.front().size(); i++) {
			double tmp = 0;
			for(int j=0; j<X_train.size(); j++) {
				tmp += X_train[j][i] * Y_train[j];
			}
			system_rivnyan[i].push_back(tmp);
		}
		
		solution = gaussa(system_rivnyan);
	}
	
	void Check() {
//		cout<<"CheckSolution"<<endl;
		double tmp = 0;
		for(int i=0; i<X_check.size(); i++) {
			double sum = 0;
			for(int j=0; j<X_check.front().size(); j++) {
				sum += X_check[i][j] * solution[j];
			}
			tmp += pow(Y_check[i] - sum, 2);
		}
		diff = tmp / Y_check.size();
	}
};


class MGUA {
	const InputData Data;
	
	Positions positions;
	
	ArraySetupForMNK solution_by_mnk;
	
	vector<vector<vector<int>>> functions;
	
	vector<vector<Solution>> solutions_by_iterations;
	Solution best_solution;
	
public:
	MGUA(	const vector<vector<double>>& X_inp, 
			const vector<double>& Y_inp, 
			const double training_part = 0.85,
			const Positions::Gen train_check_distribution = Positions::Gen::DispY	)
		:
			Data(X_inp, Y_inp)
	{
		const int train_amount = Data.size()*training_part;
		
		cout<<"There will be "<< train_amount <<" train and "<< Data.size()-train_amount <<" check data"<<endl;
		cout<<endl;
		
		SetTrainCheckPositions(train_amount, train_check_distribution);
		
		cout<<"train data:"<<endl;
		for(const auto i:positions.train) {
			for(int j=0; j<Data.variables(); j++) {
				cout<< Data.X.at(i).at(j) <<"\t";
			}
			cout<<"\t|\t"<< Data.Y.at(i) <<endl;
		}
		cout<<endl;
		cout<<"Check data:"<<endl;
		for(const auto i:positions.check) {
			for(int j=0; j<Data.variables(); j++) {
				cout<< Data.X.at(i).at(j) <<"\t";
			}
			cout<<"\t|\t"<< Data.Y.at(i) <<endl;
		}
		cout<<endl;
	}
	
	
	void Start() {
		// Start all the solution finding process
		
		// y = c0 + f(x1,x2) 	// f(x_n-1, x_n)
		// z = c0 + f(y1, y2)
		
		FirstFunctionsSetup();
		
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
			
			SetUpNextFunctionsFromBestSolutions();
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
		// Opportunity to check finded solution on inputed data
		cout<<"Enter values of X: "<<endl;
		vector<double> X_input;
		double Y_input;
		for(int i=0; i<Data.variables(); i++) {
			double tmp;
			cin >> tmp;
			X_input.push_back(tmp);
		}
		cout<<"Enter value of Y: ";
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
		cout<<"Y origin: "<< Y_input <<endl
			<<"Y calc: "<< sum <<endl
			<<"Standart deviation: "<< pow(Y_input - sum, 2) <<endl;
		cout<<endl;
	}
	
	
private:
	void FirstFunctionsSetup() {
		// all pairs generation
		for(int i0=0; i0<Data.variables(); i0++) {
			for(int i1=i0; i1<Data.variables(); i1++) {
				functions.push_back({{i0, i1}});
			}
		}
	}
	
	
	void SetUpNextFunctionsFromBestSolutions(const int count=8) {
		vector<vector<vector<int>>> variables;
		for(int i=0; i<count && i<solutions_by_iterations.back().size(); i++) {
			variables.push_back(solutions_by_iterations.back().at(i).func);
		}
		functions.clear();
		
//		sort(variables.begin(), variables.end());
//		variables.erase(unique(variables.begin(), variables.end()), variables.end());
		
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
				sort(tmp.begin(), tmp.end());
				if(unique(tmp.begin(), tmp.end()) == tmp.end()) {
					functions.push_back(tmp);
				}
			}
		}
//		cout<<"funcs: "<< functions <<endl;
	}
	
	
	void SetTrainCheckPositions(const int train_amount, const Positions::Gen gen = Positions::Gen::DispY) {
		switch (gen) {
			case Positions::Gen::DispX: {
				positions = GetPosByDispX(train_amount);
				break;
			}
			case Positions::Gen::DispY: {
				positions = GetPosByDispY(train_amount);
				break;
			}
			case Positions::Gen::EveryN: {
				positions = GetPosByEveryN(train_amount);
				break;
			}
			case Positions::Gen::Pos: {
				positions = GetPosByPos(train_amount);
				break;
			}
			default: {
				positions = GetPosByDispY(train_amount);
				break;
			}	
		}
	}
	
	Positions GetPosByDispX(const int train_amount) const {
		// Find check and train data by calculation X dispersion for every measurement by sum of every param dispersion
		// and sorting to place measures with bigger dispersion in train data
		vector<int> train_pos, check_pos;
		
		vector<double> Ser;
		vector<pair<int, double>> Disp;
		
		{
			for(int i=0; i<Data.variables(); i++) {
				double sum = 0;
				for(int j=0; j<Data.size(); j++) {
					sum += Data.X.at(j).at(i);
				}
				Ser.push_back(sum/Data.size());
			}
		}
		{
			for(int i=0; i<Data.size(); i++) {
				double sum = 0;
				for(int j=0; j<Data.size(); j++) {
					sum += pow(Data.X.at(i).at(j) - Ser.at(j), 2);
				}
				Disp.push_back({i, sum});
			}
		}
		
		sort(Disp.begin(), Disp.end(), [](const auto& lhs, const auto& rhs){
			return lhs.second > rhs.second;
		});
		
		for(int i=0; i<train_amount; i++) {
			train_pos.push_back(Disp.at(i).first);
		}
		for(int i=train_amount; i<Data.size(); i++) {
			check_pos.push_back(Disp.at(i).first);
		}
		
		return {train_pos, check_pos};
	}
	
	Positions GetPosByDispY(const int train_amount) const {
		// Find check and train data by calculation Y dispersion
		// and sorting to place measures with bigger dispersion in train data
		vector<int> train_pos, check_pos;
		
		vector<double> Ser;
		vector<pair<int, double>> Disp;
		
		double ser = 0;
		{
			for(int i=0; i<Data.size(); i++) {
				ser += Data.Y.at(i);
			}
			ser /= Data.Y.size();
		}
		{
			for(int i=0; i<Data.Y.size(); i++) {
				Disp.push_back({i, pow(Data.Y.at(i) - ser, 2)});
			}
		}
		
		sort(Disp.begin(), Disp.end(), [](const auto& lhs, const auto& rhs){
			return lhs.second > rhs.second;
		});
		
		for(int i=0; i<train_amount; i++) {
			train_pos.push_back(Disp.at(i).first);
		}
		for(int i=train_amount; i<Data.size(); i++) {
			check_pos.push_back(Disp.at(i).first);
		}
		
		return {train_pos, check_pos};
	}
	
	Positions GetPosByPos(const int train_amount) const {
		// Find check and train data by choosing every N as check data
		const int every_n_for_check = Data.size()/(Data.size() - train_amount);
		vector<int> train_pos, check_pos;
		
		for(int i=0; i<Data.size(); i++) {
			if(i % every_n_for_check == 0) {
				check_pos.push_back(i);
			}
			else {
				train_pos.push_back(i);
			}
		}
		return {train_pos, check_pos};
	}
	
	Positions GetPosByEveryN(const int train_amount) const {
		// Find check and train data by choosing every N as check data
		vector<int> train_pos, check_pos;
		
		for(int i=0; i<Data.size(); i++) {
			if(i < train_amount) {
				train_pos.push_back(i);
			}
			else {
				check_pos.push_back(i);
			}
		}
		return {train_pos, check_pos};
	}
	
	
	bool NeedAnotherIteration() const {
		if(solutions_by_iterations.back().size() < 2) {
			// We can't check enything else in this case
			return false;
		}
		if(solutions_by_iterations.size() < 2) {
			// We can't find new combinations in this case
			return true;
		}
		// Check if best if previous solutions was better than best than best of theese
		return solutions_by_iterations.at(solutions_by_iterations.size()-1).front().diff < solutions_by_iterations.at(solutions_by_iterations.size()-2).front().diff;
	}
	
	
	void GetSolutionFromFunctions() {
		// Setting up and solving system by Least squares method
		// if ok thed add to solution array
		for(const auto& func:functions) {
			solution_by_mnk.SetupAndFind(func, positions, Data.X, Data.Y);
			
			const auto solution = solution_by_mnk.GetSolution();
			const double diff = solution_by_mnk.GetDiff();
			
			if(!isnan(diff)) {
				// Sometimes there are no solution by Gaussian elimination method
				solutions_by_iterations.back().push_back({func, solution, diff});
			}
//			cout<<"func:"<< new_function <<endl 
//				<<"solt:"<< solution <<endl 
//				<<"diff:"<< diff <<endl
//				<<endl;
		}
	}
};
