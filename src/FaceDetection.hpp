
#ifndef FACE_DETECTION_HPP
#define FACE_DETECTION_HPP

#include "OpenCVCommon.hpp"

class FaceDetection{

private:

    const string window_name = "Face detection";
	// String face_cascade_name = "/usr/local/Cellar/opencv/2.4.9/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml";
	const string face_cascade_name = "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml";
	const string eyes_cascade_name = "/usr/local/share/OpenCV/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

	CascadeClassifier face_cascade;
	CascadeClassifier eyes_cascade;

    Mat srcImg;

public:

	void run();
	void detectAndDisplay( Mat frame );
	void getAllFaces( Mat frame, vector<Rect>& faces);
};
#endif