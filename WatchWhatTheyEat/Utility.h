// We, Minna Lin, Christopher Lopez, Evan Marinzel, declare that the attached assignment
// is our own work in accordance with the Seneca Academic Policy.  We have not copied any
// part of this assignment, manually or electronically, from any other source including
// web sites, unless specified as references. We have not distributed our work to other students.
//
// Utility.h

// Update IMGPATH to your local img_raw folder path, ie: "C:\\Project2\\WatchWhatTheyEat\\WatchWhatTheyEat\\img_raw\\"
// Update imgSource to URL of your camera stream, ie: "http://my.ip.address:port/?action=snapshot"
#define IMGFOLDER " "
#define IMGSOURCE " "

//#include <curl/curl.h>


// This function returns the command line text used to call the curl executable 
// It would be better to use curl library, or some other method
// instead of using a system() call, if we have time.
//
std::string generateCommand() {

	std::string imgSavePath = IMGFOLDER;
	std::string imgSource = IMGSOURCE;
	// Un-comment to append time (to the second) instead of over-writing the image.
	//std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());	
	std::string filename = "currentPic.jpeg"; // + std::to_string(time) + ".jpeg";
	std::string command = "curl -o " + imgSavePath + filename + " " + imgSource;

	return command;

}