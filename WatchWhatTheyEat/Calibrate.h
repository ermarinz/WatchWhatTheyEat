// We, Minna Lin, Christopher Lopez, Evan Marinzel, declare that the attached assignment
// is our own work in accordance with the Seneca Academic Policy.  We have not copied any
// part of this assignment, manually or electronically, from any other source including
// web sites, unless specified as references. We have not distributed our work to other students.
//
// Calibrate.h
//
// This file contains the logic used to isolate the food dish from the rest of the image.
// The user is promted to draw a box around the food dish. The box is used later to crop the image.
//

// True while mouse down event
bool drawing_box = false;

// True after mouse up event
bool drawing_complete = false;

// Rectangle and colour to use in drawing
cv::Rect box;
cv::Scalar colour = cv::Scalar(0, 255, 0);

// Mouse callback to handle mouse button event for drawing
void my_mouse_callback(int event, int x, int y, int flags, void* param);

// Subroutine to draw a box onto an image for calibrate function
void draw_box( cv::Mat& img, cv::Rect box, cv::Scalar col ) {
	cv::rectangle(
		img,
		box.tl(),
		box.br(),
		col,
		2
	);
}

// Mouse callback. If left button is pressed, start drawing rectangle.
// When button released, draw rectangle to image. When mouse is dragged
// with button down, resize the rectangle. 
void my_mouse_callback(int event, int x, int y, int flags, void* param) {
	cv::Mat& image = *(cv::Mat*) param;
	switch( event ) {
		case cv::EVENT_MOUSEMOVE: {
			if (drawing_box) {
				box.width  = x-box.x;
				box.height = y-box.y;
			}
		}
		break;
		case cv::EVENT_LBUTTONDOWN: {
			// draw box
			drawing_box = true;
			box = cv::Rect( x, y, 0, 0 );
		}
		break;
		case cv::EVENT_LBUTTONUP: {
			if (drawing_box) {
				drawing_box = false;
				if (box.width < 0) {
					box.x += box.width;
					box.width *= -1;
				}
				if (box.height < 0) {
					box.y += box.height;
					box.height *= -1;
				}
				draw_box(image, box, colour);
				drawing_complete = true;
			}
		}
		break;
	}
}

// Prompts user to draw rectangle around the food dish. Returns the rectangle.
cv::Rect calibrate(cv::Mat img, bool mustCalibrate) {

	// Two images to be used in drawing, cloned from img
	cv::Mat image = img.clone(), temp;

	// Input for detecting ESC to exit, in which case a null rectangle is returned
	char input = ' ';

	// Variables for drawing instruction text on to the image
	char instructions[58] = "Left click and drag to draw a square around the food dish";
	int font = CV_FONT_HERSHEY_PLAIN, thickness = 1;
	double fontScale = 1;
	int baseline = 0;
	cv::Size textSize = cv::getTextSize(instructions, font, fontScale, thickness, &baseline);
	cv::Point textOrg((image.cols - textSize.width) / 2, 40);

	// Initialize temp image, rectangle, and write instruction text to image
	image.copyTo(temp);
	box = cv::Rect(-1, -1, 0, 0);
	cv::putText(image, instructions, textOrg, font, fontScale, cv::Scalar::all(255), thickness, 8);

	// Initialize window 
	cv::namedWindow("Calibrate Food Location");
	cv::moveWindow("Calibrate Food Location", 400, 300);

	// Install mouse callback and pass image as parameters
	cv::setMouseCallback("Calibrate Food Location", my_mouse_callback, (void*)&image);

	for (;;) {

		image.copyTo(temp);
		if (drawing_box) draw_box(temp, box, colour);
		cv::imshow("Calibrate Food Location", temp);
		input = cv::waitKey(15);
		if (input == 27) {
			// ESC key pressed
			if (mustCalibrate = true) {
				std::cout << "First time calibration required. Unable to exit." << std::endl;
			}
			else {
				char confirm = ' ';
				while (confirm != 'y' && confirm != 'Y' && confirm != 'n' && confirm != 'N') {
					std::cout << "Exit without calibration? Previous selection will be used. (Y/N): ";
					std::cin >> confirm;
					if (confirm == 'y' || confirm == 'Y') {
						box = cv::Rect(-1, -1, 0, 0);
						break;
					}
				}
			}
		}
		else if (drawing_complete == true) {
			break;
		}
	}

	cvDestroyWindow("Calibrate Food Location");
	drawing_complete = false;
	drawing_box = false;

	return box;
}