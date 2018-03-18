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
#include "Calibrate.h"
#include "DataHandler.h"

#pragma comment(lib, "vfw32.lib")
#pragma comment(lib, "comctl32.lib" )


int main(int argc, char** argv) {

	// Interval between calculating new food amount and writing result to DB.
	int waitMinutes = 10;

	// Area of interest
	cv::Rect dishArea = cv::Rect(-1, -1, 0, 0);

	// Initial capture (img) and after applying crop to food area (img_cropped).
	cv::Mat img, img_cropped;

	// Local image storage path. Update IMGFOLDER value in Utility.h to your own project path.
	std::string imgPath = IMGFOLDER;

	// Use "currentPic.jpeg" once running the completed solution. Otherwise, set this to an example file name.
	imgPath.append("\exDarkEmptyMat2.jpeg");

	// Generate the command used to pull new picture from webcam stream
	std::string command = generateCommand();

	// Perform first time image read get area of interest
	img = cv::imread(imgPath);
	if (!img.data) {
		std::cerr << "Failed to open initial image." << std::endl;
		return 1;
	}
	else {
		std::cout << "Image opened successfully." << std::endl << "Performing first time calibration." << std::endl;
		char confirm = ' ';
		while (confirm != 'y' && confirm != 'Y') {
			confirm = ' ';
			// Launch calibration to get area of interest
			dishArea = calibrate(img, true);
			if (dishArea.area() != 0) {
				// Calibrate success
				std::cout << "Selected area: " << dishArea.area() << "px." << std::endl;
				// Crop and show image
				img_cropped = img(dishArea);
				cv::namedWindow("Confirm Selection", cv::WINDOW_AUTOSIZE);
				cv::imshow("Confirm Selection", img_cropped);
				while (confirm != 'y' && confirm != 'Y' && confirm != 'n' && confirm != 'N') {
					// Confirm result, or re-do calibration
					std::cout << "Confirm result? (Y) or re-do? (N): ";
					confirm = cv::waitKey(0);
					if (confirm == 'y' || confirm == 'Y') {
						cv::destroyWindow("Confirm Selection");
						std::cout << "Yes" << std::endl << "Selection confirmed." << std::endl;
					}
					else if (confirm == 'n' || confirm == 'N') {
						cv::destroyWindow("Confirm Selection");
						std::cout << "No" << std::endl << "Re-doing calibrate selection." << std::endl;
					}
					else {
						std::cout << std::endl;
					}
				}
			}
			else {
				std::cerr << "Calibrate function failed." << std::endl;
				return 2;
			}
		}
	}

	// 

	// Main loop. Will leave this commented out until getting a single full run to work first.
	/*
	for (;;) {

		// Wait specified amount of time before repeating the process
		std::this_thread::sleep_for(std::chrono::minutes(waitMinutes));

		// Pull new .jpeg from webcam stream
		system(command.c_str());

		// Read image
		img = cv::imread(imgPath);
		if (!img.data) {
			std::cerr << "Failed to open image." << std::endl;
		}
		else {
			// Crop to food dish using existing area of interest
			img_cropped = img(dishArea);
		}

		// Calculate % food remaining

		// Write to DB

		// Allow for user input to re-calibrate area of interest

	}	
	*/

	return 0;

}