#pragma once

#include <vector>

/*
    A function that solves System of equations arr by the "Gaussian elimination" method and outputs a solution in the form of a linear array of values.
    
    Parameters
    ----------
    arr : 2d array in vector struct
    	Array to solve, in which every line contains "y" value on last position of vector
    
    Returns
    -------
    output : vector
    	Array of coefficients
*/
std::vector<double> gaussa(std::vector<std::vector<double>> arr);
