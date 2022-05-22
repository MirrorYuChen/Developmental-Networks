/*
 * @Description: 
 * @Date: 2022-05-22 00:52:42
 * @Author: chenjingyu
 * @LastEditTime: 2022-05-22 15:37:24
 * @FilePath: \Developmental-Networks\MILN\examples\main.cc
 */
#include <assert.h>
#include <algorithm>
#include <iostream>
#include "ClassifierCortex.h"

#ifndef uchar
#   define uchar unsigned char
#endif 

int ReadNumber(FILE* fp, int len) {
	uchar* buffer = new uchar[len];
	fread(buffer, len, 1, fp);
	int result = 0;
	for (int i = 0; i < len; ++i) {
		int temp = buffer[i];
		result += temp << ((len - 1 - i) * 8);
	}
	delete[] buffer;
	return result;
}

void ReadImage(FILE* fp, int len, uchar* buffer) {
	fread(buffer, len, 1, fp);
}

struct Sample {
    int label;
    double image[28 * 28];
};

std::vector<Sample> ReadSample(const char* imgFile, const char* labelFile) {
	std::vector<Sample> result;
	FILE* imgIn = fopen(imgFile, "rb");
	FILE* labelIn = fopen(labelFile, "rb");
	if (imgIn == NULL || labelIn == NULL) {
		if (imgIn) {
			fclose(imgIn);
		}
		if (labelIn) {
			fclose(labelIn);
		}
		std::cout << "open TrianFile failed." << std::endl;
		return std::vector<Sample>();
	}

	int imgMagic = ReadNumber(imgIn, 4);
	int imgNum = ReadNumber(imgIn, 4);
	int imgWidth = ReadNumber(imgIn, 4);
	int imgHeight = ReadNumber(imgIn, 4);

	int labelMagic = ReadNumber(labelIn, 4);
	int labelNum = ReadNumber(labelIn, 4);
	if (imgMagic != 2051 || labelMagic != 2049) {
		if (imgIn) {
			fclose(imgIn);
		}
		if (labelIn) {
			fclose(labelIn);
		}
		std::cout << "error magic number." << std::endl;
		return std::vector<Sample>();
	}
	assert(imgNum == labelNum);
	result.clear();
	result.reserve(imgNum);
	int imgSize = imgWidth * imgHeight;
	uchar* buffer = new uchar[imgSize];
	for (int i = 0; i < imgNum; ++i) {
		Sample sample;
		ReadImage(imgIn, imgSize, buffer);
		sample.label = ReadNumber(labelIn, 1);
		for (int j = 0; j < imgSize; ++j) {
			sample.image[j] = buffer[j];
		}
		result.push_back(sample);
	}
	delete[] buffer;
	fclose(imgIn);
	fclose(labelIn);
	return result;
}

int main(int argc, char* argv[]) {
    const char* trainImgFile = "../../data/mnist/train-images.idx3-ubyte";
	const char* trainLabelFile = "../../data/mnist/train-labels.idx1-ubyte";
	const char* testImgFile = "../../data/mnist/t10k-images.idx3-ubyte";
	const char* testLabelFile = "../../data/mnist/t10k-labels.idx1-ubyte";

	std::vector<Sample> trainSamples = ReadSample(trainImgFile, trainLabelFile);
	std::vector<Sample> testSamples = ReadSample(testImgFile, testLabelFile);

    mirror::ClassifierCortex* classifier = new mirror::ClassifierCortex(28 * 28, 10, 1, 0.3);

////////////////////  train phase  /////////////////////////
    std::cout << "start train." << std::endl;
    for (int currIter = 0; currIter < 1; ++currIter) {
		std::cout << "current iteration is: " << currIter << std::endl;
		std::random_shuffle(trainSamples.begin(), trainSamples.end());
		for (const auto& sample : trainSamples) {
			std::cout << "Train stage -------- current label: " << sample.label << std::endl;
			classifier->Update(sample.image, sample.label);
		}
    }
    std::cout << "end train." << std::endl;
////////////////////  test phase  /////////////////////////
    std::cout << "start test." << std::endl;
	int counts = 0;
    for (const auto& sample : testSamples) {
		std::cout << "Test stage -------- current label: " << sample.label << std::endl;
        if (classifier->Classify(sample.image, sample.label)) {
            counts++;
        }
    }
    std::cout << "end test." << std::endl;
    std::cout << "correct rate: " << (float)(counts) / (float)(testSamples.size()) << std::endl;
    if (classifier) {
        delete classifier;
        classifier = NULL;
    }

    return 0;
}
