#include <iostream>
#include <vector>
#include <algorithm>
#include "CCIPCA.h"

#if defined(_MSC_VER)
#   include <Windows.h>
#   include <io.h>
#else
#   include <dirent.h>
#   include <sys/stat.h>
#   include <sys/types.h>
#   include <unistd.h>
#endif

#include <opencv2/opencv.hpp>

int getAllFiles(const std::string& filePath, std::vector<std::string>& filesVec) {
	filesVec.clear();
#if defined(_MSC_VER)
	long long hFile = 0;
	struct _finddata_t fileInfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(filePath).append("\\*").c_str(), &fileInfo)) != -1) {
		do {
			if ((fileInfo.attrib & _A_SUBDIR)) {
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
					getAllFiles(p.assign(filePath).append("\\").append(fileInfo.name), filesVec);
			}
			else {
				filesVec.push_back(fileInfo.name);
			}
		} while (_findnext(hFile, &fileInfo) == 0); _findclose(hFile);
	}
#else
	struct stat s;
	lstat(filePath.c_str(), &s);
	struct dirent* fileName;
	DIR* dir;
	dir = opendir(filePath.c_str());
	while ((fileName = readdir(dir)) != nullptr) {
		if (strcmp(fileName->d_name, ".") == 0 || strcmp(fileName->d_name, "..") == 0) {
			continue;
		}
		filesVec.push_back(fileName->d_name);
	}
#endif
	std::random_shuffle(filesVec.begin(), filesVec.end());
	return 0;
}

int main() {
	std::string folder = "../data/yale";
	std::vector<std::string> filesVec;
	if (getAllFiles(folder, filesVec) != 0) {
		std::cout << "get folder: " << folder << "failed." << std::endl;
		return -1;
	}

	std::cout << "filesVec size: " << filesVec.size() << std::endl;
	mirror::CCIPCA* ccipca = new mirror::CCIPCA(15, 100 * 100);
	for (int iter = 0; iter < 10; ++iter) {
		for (int i = 0; i < filesVec.size(); ++i) {
			cv::Mat img = cv::imread(folder + "/" + filesVec[i], 0);
			img.convertTo(img, CV_32FC1);
			float* fX = reinterpret_cast<float*>(img.data);
			ccipca->Update(fX);
		}
	}
	auto eigenVecs = ccipca->getEigenVectors();

	cv::Mat result = cv::Mat(100 * 3, 100 * 5, CV_8U);
	float* eigen = new float[100 * 100];
	cv::Mat imgEigen = cv::Mat(100, 100, CV_8U);
	std::cout << "eigenVec size: " << eigenVecs.size() << std::endl;
	for (int i = 0; i < eigenVecs.size(); ++i) {
		auto eigenVec = eigenVecs[i];
		memset(eigen, 0.0, 100 * 100 * sizeof(float));
		memcpy(eigen, eigenVec.data(), 100 * 100 * sizeof(float));
		float minVal = FLT_MAX, maxVal = -FLT_MAX;
		for (int j = 0; j < 100 * 100; ++j) {
			if (eigen[j] < minVal) {
				minVal = eigen[j];
			}
			if (eigen[j] > maxVal) {
				maxVal = eigen[j];
			}
		}
		for (int j = 0; j < 100 * 100; ++j) {
			int x = j % 100;
			int y = j / 100;
			int val = 255 * (eigen[j] - minVal) / (maxVal - minVal);
			imgEigen.at<uchar>(y, x) = val > 255 ? 255 : val;
		}
		int beginX = i % 5;
		int beginY = i / 5;
		imgEigen.copyTo(result(cv::Rect(beginX * 100, beginY * 100, 100, 100)));
		cv::imwrite(std::to_string(i) + ".jpg", imgEigen);
	}
	delete[] eigen;

	cv::imshow("result", result);
	cv::waitKey(0);

	if (ccipca != NULL) {
		delete ccipca;
		ccipca = NULL;
	}
	return 0;
}