#include <iostream>
#include <opencv2/aruco.hpp>

//using namespace cv;
//using namespace std;

int main(int argc, char *argv[])
{
	//crezione markers
	cv::Mat markerImage;
	cv::Ptr<cv::aruco::Dictionary> dizionario = cv::aruco::getPredefinedDictionary ( cv::aruco::DICT_6X6_250 );
	cv::aruco::drawMarker (dizionario, 23, 200, markerImage, 1); //marker 23 del dizionario DICT_6X6_250.  output avrà una dimensione di 200x200 pixel.
	cv::imwrite ( "marker23.png" , markerImage);

	//marker detection:
	cv::Mat inputImage;
	std::vector<int> markerIds;
	std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
	cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	//L'elenco dei dizionari predefiniti disponibili può essere trovato nella documentazione per l' PREDEFINED_DICTIONARY_NAMEenumerazione.
	//Il dizionario può essere generato automaticamente per regolare il numero desiderato di marker e bit, in modo da ottimizzare la distanza tra i marker:
	//cv::Ptr<cv::aruco::Dictionary> dizionario = cv::aruco::generateCustomDictionary (36, 5);
	//Questo genererà un dizionario personalizzato composto da 36 marcatori di 5x5 bit.


	cv::aruco::detectMarkers(inputImage, dictionary, markerCorners, markerIds, parameters, rejectedCandidates); //One of the parameters of detectMarkers() function is a DetectorParameters object. 
	

	//CHECK MARKERS ARE CORRECTLY DETECTED (VISUALIZATION)
	cv::Mat outputImage = inputImage.clone();
	cv::aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);

	//loop per rilevamento markers dalla camera
	cv::VideoCapture inputVideo;
	inputVideo.open(0);
	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	while (inputVideo.grab()) {
		cv::Mat image, imageCopy;
		inputVideo.retrieve(image);
		image.copyTo(imageCopy);
		std::vector<int> ids;
		std::vector<std::vector<cv::Point2f> > corners;
		cv::aruco::detectMarkers(image, dictionary, corners, ids);
		// if at least one marker detected
		if (ids.size() > 0)
			cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
		cv::imshow("out", imageCopy);
		char key = (char) cv::waitKey(waitTime);
		if (key == 27)
			break;
	}
	////////////////////////////// POSE ESTIMATION

	//// Calibration of the camera

	// --> Look Calibration tutorial of OpenCV. 
	// You can also calibrate your camera using the aruco module as explained in the Calibration with ArUco and ChArUco tutorial.
	//hat you get after the calibration is the camera matrix:
	// a matrix of 3x3 elements with the focal distances and the camera center coordinates (a.k.a intrinsic parameters),
	// and the distortion coefficients: a vector of 5 or more elements that models the distortion produced by your camera.

	//Estimate pose of all detected markers
	cv::Mat cameraMatrix, distCoeffs;
	// You can read camera parameters from tutorial_camera_params.yml
	readCameraParameters(filename, cameraMatrix, distCoeffs); // This function is located in detect_markers.cpp
	std::vector<cv::Vec3d> rvecs, tvecs;
	cv::aruco::estimatePoseSingleMarkers(markerCorners, 0.05, cameraMatrix, distCoeffs, rvecs, tvecs); //The output parameters rvecs and tvecs are the rotation and translation vectors respectively, for each of the markers in markerCorners.
	//cameraMatrix and distCoeffs are the camera calibration parameters that were created during the camera calibration process.

	// draw axis in the image, so pose estimation can be checked:
	inputImage.copyTo(outputImage);
	for (int i = 0; i < rvecs.size(); ++i) {
		auto rvec = rvecs[i];
		auto tvec = tvecs[i];
		cv::drawFrameAxes(outputImage, cameraMatrix, distCoeffs, rvec, tvec, 0.1); //rvec and tvec are the pose parameters for the marker whose axis is to be drawn.
	}

	//full example 
	cv::VideoCapture inputVideo;
	inputVideo.open(0);
	cv::Mat cameraMatrix, distCoeffs;
	// You can read camera parameters from tutorial_camera_params.yml
	readCameraParameters(filename, cameraMatrix, distCoeffs); // This function is located in detect_markers.cpp
	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	while (inputVideo.grab()) {
		cv::Mat image, imageCopy;
		inputVideo.retrieve(image);
		image.copyTo(imageCopy);
		std::vector<int> ids;
		std::vector<std::vector<cv::Point2f>> corners;
		cv::aruco::detectMarkers(image, dictionary, corners, ids);
		// if at least one marker detected
		if (ids.size() > 0) {
			cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
			std::vector<cv::Vec3d> rvecs, tvecs;
			cv::aruco::estimatePoseSingleMarkers(corners, 0.05, cameraMatrix, distCoeffs, rvecs, tvecs);
			// draw axis for each marker
			for(int i=0; i<ids.size(); i++)
				cv::drawFrameAxes(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
		}
		cv::imshow("out", imageCopy);
		char key = (char) cv::waitKey(waitTime);
		if (key == 27)
			break;
	}


}