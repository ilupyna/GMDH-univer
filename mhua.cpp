#include "mhua.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <exception>

using namespace std;



InputData::InputData(	const vector<vector<double>>& X_inp, 
						const vector<double>& Y_inp )
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


const int InputData::variables() const {
	return X.front().size();
}


const int InputData::size() const {
	return X.size();
}



/*
	Method which according to the obtained function fills the system of equations for the "Least squares" 
	and outputs the values of the coefficients using "Gaussian elimination" method. 
	At first it calls the method `ArraySetupForMNK::Setup(function, positions, X, Y)`, 
	and then calculates the corresponding sums of products for each of the system variables.
	Then `ArraySetupForMNK::Solve()` it using the function `gaussa(system_rivnyan)`, 
	and `ArraySetupForMNK::Check()` with "Least squares" method.
*/
void ArraySetupForMNK::SetupAndFind(
	const vector<vector<int>>& function, 
	const Positions& positions,
	const vector<vector<double>>& X, 
	const vector<double>& Y ) 
{
	Setup(function, positions, X, Y);
	Solve();
	Check();
}


const vector<double> ArraySetupForMNK::GetSolution() const {
	return solution;
}

	
const double ArraySetupForMNK::GetDiff() const {
	return diff;
}

	
void ArraySetupForMNK::ArrClear() {
	X_train.clear();
	X_check.clear();
	Y_train.clear();
	Y_check.clear();
}

	
void ArraySetupForMNK::ArrReserve(const int train_size, const int check_size) {
	X_train.reserve(train_size);
	Y_train.reserve(train_size);
	
	X_check.reserve(check_size);
	Y_check.reserve(check_size);
}


/*
	Method that fills in a table of variable functions according to the variables used in the function 
	of products (variable pairs, pairs etc.) to further populate the level system for least squares calculation.
*/
void ArraySetupForMNK::Setup(
	const vector<vector<int>>& function, 
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


void ArraySetupForMNK::Solve() {
//	cout<<"Setting system from array"<<endl;
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


/*
	A private method of the MGUA class that checks the obtained solution value on test data and outputs the standard deviation value.
*/
void ArraySetupForMNK::Check() {
//	cout<<"CheckSolution"<<endl;
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



/*
	Initializing the MGUA class, checks the dimensionality of input data, their storage, as well as performs the division into training and verification samples.
	
	Parameters
	----------
	X_inp : 2d matrix in vector struct
		Variables "x".
	Y_inp : 2d matrix in vector struct
		Functions "y".
	training_part: double
		The coefficient of division into training and test sets.
	train_check_distribution : enum type {DispY, DispX, Pos, EveryN}
		Comparison parameter for distribution.
*/
MGUA::MGUA(	
	const vector<vector<double>>& X_inp, 
	const vector<double>& Y_inp, 
	const double training_part,
	const Positions::Gen train_check_distribution )
	: Data(X_inp, Y_inp)
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


/* 
	Method finding of the GMDH model with threshold self-selection. 
	
	The first functions are created from pairs of variables using the MGUA :: FirstSetup () method.
	Then the following steps are performed cyclically: 
	1. Find possible solutions by calling the `MGUA::GetSolutionFromFunctions` method,	sorting the solutions obtained on the iteration and deriving the best ones.
	2. The `MGUA::GetFunctionsFromBestSolutions` method is called to create functions for consideration on the next iteration.
	3. The `MGUA::NeedAnotherIteration` method is checked to determine if the next iteration of the algorithm is required.
	After stopping the cyclic search, the best found model is displayed: its type, values of coefficients and loss values for verification data.
*/
    
void MGUA::Start() {
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


/*
	Method to verify the obtained models on the entered values.
	
	Waits until the values of all values of X functions and values of B are entered, after which it brings them to the value 
	of the obtained model and estimate of the value of B and the loss, after comparison with the introduced by you.
*/
void MGUA::FreeCheck() const {
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


/* Method that fills the list of functions with all possible input variables. */
void MGUA::FirstFunctionsSetup() {
	// all pairs generation
	for(int i0=0; i0<Data.variables(); i0++) {
		for(int i1=i0; i1<Data.variables(); i1++) {
			functions.push_back({{i0, i1}});
		}
	}
}


/* 
	Method that selects count of top solutions, sorted in descending order of loss, 
	and then fills the function list with pairs of their variables.
*/
void MGUA::SetUpNextFunctionsFromBestSolutions(const int count) {
	vector<vector<vector<int>>> variables;
	for(int i=0; i<count && i<solutions_by_iterations.back().size(); i++) {
		variables.push_back(solutions_by_iterations.back().at(i).func);
	}
	functions.clear();
	
//	sort(variables.begin(), variables.end());
//	variables.erase(unique(variables.begin(), variables.end()), variables.end());
	
//	cout<<"variables: "<< variables <<endl;
	
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
//	cout<<"funcs: "<< functions <<endl;
}


void MGUA::SetTrainCheckPositions(const int train_amount, const Positions::Gen gen) {
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


Positions MGUA::GetPosByDispX(const int train_amount) const {
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

	
Positions MGUA::GetPosByDispY(const int train_amount) const {
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


Positions MGUA::GetPosByPos(const int train_amount) const {
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


Positions MGUA::GetPosByEveryN(const int train_amount) const {
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


/*
	Method that returns the truth value if another iteration is required. 
	
	Alternate verification of conditions: 
	1. The existence of at least 2 solutions on the last iteration.
	2. Conducting at least 2 iterations of finding solutions. 
	3. Reducing the value of the loss on the last iteration, compared with the previous one.
*/
bool MGUA::NeedAnotherIteration() const {
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


/*
	Method that performs a search of functions to calculate their coefficients, obtain loss values and save them. 
	
	For each function `func` from the list calls the methods `ArraySetupForMNK::GetSolution()`, 
	the method `ArraySetupForMNK::GetDiff()` checks the conditions of existence of the solution for the formed system of equations
	and if there is a solution adds it to the array with the value of the function and calculated loss.
*/
void MGUA::GetSolutionFromFunctions() {
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
//		cout<<"func:"<< new_function <<endl 
//			<<"solt:"<< solution <<endl 
//			<<"diff:"<< diff <<endl
//			<<endl;
	}
}
