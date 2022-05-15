#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "IHDR.h"

#define XDIM 16
#define YDIM 1

int main(int argc, char* argv[])
{
	//initial dimension of the input and outpt vector
	int xdim = XDIM;
	int ydim = YDIM;
	// name of training file and testing file
	const char* trainFile = "../data/letter.tra";
	const char* testFile = "../data/letter.tes";
	const char* resultFile = "./result.txt";
	const char* treeFile = "./sample.tree";
	IHDRTree* MyTree = new IHDRTree(0, xdim, ydim, 4, 80);

	//initialize output vector
	float* Y = new float[ydim];
	memset(Y, 0, sizeof(float) * ydim);
	float* X1 = new float[xdim];
	memset(X1, 0, sizeof(float) * xdim);

	int Label = 0;
	int tempValue = 0;
	int nCorrect = 0;
	
	printf("Training.....\n");
	FILE* fp = fopen(trainFile, "r");
	if (fp == NULL) {
		std::cout << "open file: " << trainFile << "failed." << std::endl;
		return -1;
	}
	int no_data = 0;
	while (feof(fp) == 0) {
		for (int InputDim = 0; InputDim < xdim; InputDim++) {
			(void)fscanf(fp, "%d ", &tempValue);
			X1[InputDim] = tempValue;
		}
		(void)fscanf(fp, "%d \n", &Label);

		Y[0] = Label;
		MyTree->UpdateTree(X1, Y);
		no_data++;
	}
	fclose(fp);

	//save the tree
	MyTree->SaveTree(treeFile);

	//training ends#############################

	//testing starts############################
	//load the save tree
	MyTree->LoadTree(treeFile);
	fp = fopen(testFile, "r");
	if (fp == NULL) {
		std::cout << "open file: " << testFile << "failed." << std::endl;
		return -1;
	}
	no_data = 0;
	nCorrect = 0;
	printf("Testing.....\n");

	FILE* f_result = fopen(resultFile, "w");
	while (feof(fp) == 0)
	{
		for (int InputDim = 0; InputDim < xdim; InputDim++) {
			(void)fscanf(fp, "%d ", &tempValue);
			X1[InputDim] = tempValue;
		}
		(void)fscanf(fp, "%d \n", &Label);
		float dist = 0.0;
		MyTree->RetriveTree(X1, Y, dist);
		std::cout << "dist: " << dist << std::endl;
		if (int(Label) == int(Y[0]))
			nCorrect++;
		fprintf(f_result, "%d %f\n", Label, Y[0]);
		no_data++;
	}

	fclose(fp);

	fprintf(f_result, "Number of sample %d correct number %d\n", no_data, nCorrect);
	fclose(f_result);
	delete[] X1;
	delete[] Y;
	if (MyTree) {
		delete MyTree;
		MyTree = NULL;
	}

	return 0;
}
