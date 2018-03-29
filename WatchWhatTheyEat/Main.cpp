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

	// Threshold parameter for canny to use in food dish detection
	int thresh = 130;

	// Contours used in detecting food dish area
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	// Index of the contour with the largest area
	int largest_index;

	// Used in determining dish area
	double largest_area = 0.0;

	// Area of interest (the food dish)
	cv::Rect dishArea = cv::Rect(-1, -1, 0, 0);

	// Initial capture (img) and other preprocessed versions
	cv::Mat img, img_gray, img_canny, img_cropped, img_norm, img_contrast, img_bin, img_closed, img_closed2;

	// Kernal used for closing img_bin
	cv::Mat kernel = 255 * cv::Mat::ones(7, 7, CV_8UC1);

	// Local image storage path. Update IMGFOLDER value in Utility.h to your own project path.
	std::string imgPath = IMGFOLDER;

	// Generate the command used to pull new picture from webcam stream
	std::string command = generateCommand();

	// Use "currentPic.jpeg" when pulling image from webcam stream. Otherwise, set this to an example file name.
	// Try running with exPaper1.jpg - exPaper6.jpg
	imgPath.append("\exPaper3.jpg");

	// Try to open image
	std::cout << imgPath << std::endl;
	img = cv::imread(imgPath);
	if (!img.data) {
		std::cerr << "Failed to open initial image." << std::endl;
		return 1;
	}
	else {
		std::cout << "Image opened successfully." << std::endl << "Detecting dish area." << std::endl;
		// Convert to grayscale
		cv::cvtColor(img, img_gray, CV_RGB2GRAY);
		// Blur
		cv::blur(img_gray, img_gray, cv::Size(3,3) );
		// Detect edges using canny
		Canny(img_gray, img_canny, thresh, thresh * 2, 3);
		// Find contours
		findContours(img_canny, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
		// Retrieve the contour with the largest area and get it's bounding rectangle, assumed to be the food dish outline
		for( int i = 0; i< contours.size(); i++ ) {
			double a = contourArea(contours[i], false);  //  Find the area of contour
			if (a > largest_area) {
				largest_area = a;
				largest_index = i;
				dishArea = boundingRect(contours[i]);
			}
		}
		// Draw largest contour and dishArea (for troubleshooting)
		/*
		cv::Scalar color(255,255,255);
		drawContours(img, contours, largest_index, color, CV_FILLED, 8, hierarchy);
		cv::rectangle(img, dishArea, cv::Scalar(0,255,0), 1, 8, 0);  
		cv::imshow("Detection Results", img);
		*/
		// Shrink area depending on dish walls, then crop
		dishArea -= cv::Size(140, 120);
		dishArea.x += 70;
		dishArea.y += 80;
		img_cropped = img_gray(dishArea);

		// The following process works well for exPaper3.jpg and exPaper4.jpg, but not the others!
		// Displaying step-by-step
		cv::imshow("1. Cropped", img_cropped);
		// Increase contrast
		img_cropped.copyTo(img_contrast);
		img_contrast *= 2;
		cv::imshow("2. Contrast", img_contrast);
		// Normalize
		cv::normalize(img_contrast, img_norm, 0, 255, cv::NORM_MINMAX, CV_8UC1);
		cv::imshow("3. Normalized", img_norm);
		// Convert to binary
		cv::threshold(img_norm, img_bin, 60, 255, CV_THRESH_BINARY);
		cv::imshow("4. Binary", img_bin);
		// Close
		cv::morphologyEx(img_bin, img_closed, CV_MOP_CLOSE, kernel);
		cv::imshow("5. Closed", img_closed);

		cv::waitKey(0);

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