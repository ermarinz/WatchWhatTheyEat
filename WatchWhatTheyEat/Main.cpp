// We, Minna Lin, Christopher Lopez, Evan Marinzel, declare that the attached assignment
// is our own work in accordance with the Seneca Academic Policy.  We have not copied any
// part of this assignment, manually or electronically, from any other source including
// web sites, unless specified as references. We have not distributed our work to other students.
//
// Main.cpp

#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
#include <ctime>
#include "Utility.h"
#include "DataHandler.h"

#pragma comment(lib, "vfw32.lib")
#pragma comment(lib, "comctl32.lib" )


int main(int argc, char** argv) {

	int waitMinutes = 10;
	std::string imgPath = IMGFOLDER;
	imgPath.append("\exDarkEmptyMat2.jpeg");
	std::string command = generateCommand();

	for (;;) {

		// Pull new .jpeg from webcam stream
		//system(command.c_str());

		// Read initial image
		cv::Mat img = cv::imread(imgPath);
		if (!img.data) {
			std::cerr << "Failed to open initial image." << std::endl;
		}
		else {
			cv::namedWindow("Initial Image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Initial Image", img);
		}
		cv::waitKey(0);

		// Detect food dish


		// Crop to food dish


		// Additional pre-processing?


		// Calculate % food remaining


		// Write result with timestamp to database


		// Wait specified amount of time before repeating the process
		std::this_thread::sleep_for(std::chrono::minutes(waitMinutes));

	}	

	return 0;

}