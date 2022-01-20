#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <exception>

#include "adds.h"
#include "gaussa.h"

using namespace std;

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
			const Positions::Gen train_check_distribution = Positions::Gen::DispY	);
	
	void Start();
	void FreeCheck() const;
	
private:
	void FirstFunctionsSetup();
	
	void SetUpNextFunctionsFromBestSolutions(const int count=8);
	
	void SetTrainCheckPositions(const int train_amount, const Positions::Gen gen = Positions::Gen::DispY);
	
	Positions GetPosByDispX(const int train_amount) const;
	Positions GetPosByDispY(const int train_amount) const;
	Positions GetPosByPos(const int train_amount) const;
	Positions GetPosByEveryN(const int train_amount) const;
	
	
	bool NeedAnotherIteration() const;
	
	void GetSolutionFromFunctions();
};