/**
 * MaxFile name: GaussianElimination
 * Summary:
 *   CPU code for solving linear equation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Maxfiles.h" 			// Includes .max files
#include <MaxSLiCInterface.h>	// Simple Live CPU interface 

int numberOfUnknownInEqn = 3;
int eliminatedEqns = 0;
const int NX = 16;
const int NY = 16;

void transformInputData(int eqnNum, int numOfUnknowns, float eqnToSolve[numberOfUnknownInEqn][(numberOfUnknownInEqn+1)], float *inVec)
{
    for (int i = 0; i < (numOfUnknowns + 1); i++)
    {
        inVec[i] = eqnToSolve[eqnNum][i];
    }
}

void print_2d_data(float *data, char* name, int nx, int ny)
{
	printf("\n%s\n", name);
	for (int y = 0; y < ny; y++) {
		for (int x = 0; x < nx; x++) {
			printf("%7.5f", data[y * nx + x]);
		}
		printf("\n");
	}
}

void substituteToSolution(int numOfEqn, int substitueLevel,float *solutionVector,float *eqnVector)
{
    float sumOfSolved = 0;
    for (int itr = 0; itr < numOfEqn+1; itr++)
    {
        sumOfSolved = sumOfSolved + (solutionVector[itr]*eqnVector[itr]);
        
    }
    
	if(substitueLevel==1)
	{
        solutionVector[numberOfUnknownInEqn-numOfEqn] = ((eqnVector[numOfEqn+1]) - sumOfSolved)/eqnVector[substitueLevel];
	}
	else
	{
	    solutionVector[substitueLevel] = ((eqnVector[numOfEqn+1]) - sumOfSolved)/eqnVector[substitueLevel];
	}
}


void solve(float eqnToSolve[numberOfUnknownInEqn-eliminatedEqns][(numberOfUnknownInEqn-eliminatedEqns+1)],float *solutionVector)
{
    size_t sizeBytes = NX * NY * sizeof(float);
	float *inVector = malloc(sizeBytes);
	float *inVector1 = malloc(sizeBytes);
	float *reducedVec = malloc(sizeBytes);
	
	float reducedEqns[numberOfUnknownInEqn-eliminatedEqns][(numberOfUnknownInEqn-eliminatedEqns+1)];
	
	transformInputData(0, numberOfUnknownInEqn, eqnToSolve, inVector); 
	for (int i=1; i < (numberOfUnknownInEqn-eliminatedEqns); i++)
	{
	    transformInputData(i, numberOfUnknownInEqn, eqnToSolve, inVector1);
	    
	    GaussianElimination(16, inVector, inVector1, reducedVec);
	    
	    
	    for(int j = 0; j < (numberOfUnknownInEqn-eliminatedEqns); j++)
	    {
	        reducedEqns[i-1][j] = reducedVec[j+1];
	    }
	}
	
	eliminatedEqns = eliminatedEqns + 1;
	
	if((numberOfUnknownInEqn-eliminatedEqns) > 1)
	{
	    //call solve
	    solve(reducedEqns,solutionVector);
	}
	
	
	substituteToSolution(numberOfUnknownInEqn-eliminatedEqns,1,solutionVector,reducedVec);
	
	eliminatedEqns = eliminatedEqns - 1;
	
	if(eliminatedEqns == 0)
	{
	    substituteToSolution(numberOfUnknownInEqn-1,0,solutionVector,inVector);
	}
    
}


int main()
{
	
	size_t sizeBytes = NX * NY * sizeof(float);
	float *inVector = malloc(sizeBytes);
	float *inVector1 = malloc(sizeBytes);
	
	float *expectedVector = malloc(sizeBytes);
	float *solutionVector = malloc(sizeBytes);

	
	//float eqnToSolve[2][3] = {{1,1,2},{2,3,5}};//ans 1,1
	//float eqnToSolve[3][4] = {{1,1,-1,4},{1,-2,3,-6},{2,3,1,7}};//ans 1,2,-1
	float eqnToSolve[3][4] = {{1,-2,3,7},{2,1,1,4},{-3,2,-2,-10}};//ans 2,-1,1
	
	
	printf("Running DFE.\n");
	solve(eqnToSolve,solutionVector);

	
    print_2d_data(solutionVector, "OUTPUT DATA", numberOfUnknownInEqn, 1);
	

	return 0;
}
