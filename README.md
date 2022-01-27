## Сourse work for last half-year of "Process modeling" course

### Group method of data handling with "threshold self-selection"

The initial data is read from a file that has a specific structure with several "y" values for working with several functions (task specific) and stored in the structure by [``` ReadFromFile ```](main.cpp#L10-L40).

#### The GMDH process itself realised in [``` Start ```](mhua.h#L82) and includes:
1. Separation into training and testing samples by [``` SetTrainCheckPositions ```](mhua.h#L322-L345);
2. Search for the optimal model:
   - Creating possible functions from pairs of variables by [``` FirstFunctionsSetup ```](mhua.cpp#L280-L287);
   - Calculate the values of the function coefficients and the deviation from the test sample for each existing function by [``` GetSolutionFromFunctions ```](mhua.cpp#L473-L491);
   - Sort resulting functions by deviation value;
   - Create new functions from top pairs by [``` SetUpNextFunctionsFromBestSolutions ```](mhua.cpp#L290-L319);
   - Check accuracy increasing to determine if next iteration is needed;
3. Display of the optimal model: used variables and found coefficients;

After executing the GMDH process, the user is given the opportunity to check the found functions on external data by [``` FreeCheck ```](mhua.cpp#L243-L277).


### The values of the coefficients in output should be understood as follows:
- {0, 1} is a pair of coefficients 0 and 1, and therefore the term of the equation X {0, 1} is a multiplication: x<sub>1</sub> * x<sub>2</sub> . 
- {{3,3}, {1, 2}} is a multiplication: x<sub>3</sub> * x<sub>3</sub> * x<sub>1</sub> * x<sub>2</sub> , or: x<sub>1</sub> * x<sub>2</sub> * x<sub>3</sub><sup>2</sup>.
