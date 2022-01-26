#pragma once

#include <vector>

#include "adds.h"
#include "gaussa.h"


struct Solution {
	std::vector<std::vector<int>> func;
	std::vector<double> A;
	double diff;
};


struct Positions {
	std::vector<int> train, check;
	enum Gen:int { DispY, DispX, Pos, EveryN };
};


struct InputData {
	const std::vector<std::vector<double>> X;
	const std::vector<double> Y;
	
	InputData(	const std::vector<std::vector<double>>& X_inp, 
			const std::vector<double>& Y_inp);
	
	const int variables() const;
	const int size() const;
};


class ArraySetupForMNK {
	std::vector<std::vector<double>> X_train, X_check;
	std::vector<double> Y_train, Y_check;
	
	std::vector<double> solution;
	double diff;
	
public:
	void SetupAndFind(	const std::vector<std::vector<int>>& function, 
				const Positions& positions,
				const std::vector<std::vector<double>>& X, 
				const std::vector<double>& Y );
	
	const std::vector<double> GetSolution() const;
	const double GetDiff() const;
	
private:
	void ArrClear();
	void ArrReserve(const int train_size, const int check_size);
	void Setup(	const std::vector<std::vector<int>>& function, 
			const Positions& positions,
			const std::vector<std::vector<double>>& X, 
			const std::vector<double>& Y);
	void Solve();
	void Check();
};


class MGUA {
	const InputData Data;
	
	Positions positions;
	ArraySetupForMNK solution_by_mnk;
	std::vector<std::vector<std::vector<int>>> functions;
	std::vector<std::vector<Solution>> solutions_by_iterations;
	Solution best_solution;
	
public:
	MGUA(	const std::vector<std::vector<double>>& X_inp, 
		const std::vector<double>& Y_inp, 
		const double training_part = 0.85,
		const Positions::Gen train_check_distribution = Positions::Gen::DispY
	);
	
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
