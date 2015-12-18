
// #include "opencv2/imgproc/imgproc.hpp"
// #include "opencv2/highgui/highgui.hpp"
// #include "OpenCVCommon.hpp"
// #include "AdaptiveSkinDetector.hpp"
// #include "FaceDetection.hpp"
// #include "GetAllBodySkinRatios.hpp"
// #include "/usr/local/Cellar/jsoncpp/0.10.5/include/json/json.h"
#include "ParseJson.hpp"
#include "GetAllBodySkinRatios.hpp"
#include "Test.hpp"


// Mat dst;

// char window_name1[] = "Unprocessed Body";
// char window_name2[] = "Processed Body";

// vector<Rect> facePos;

// Rect faceToBody(Rect face, int oriRow, int oriCol){
// 	cout<<oriRow<<' '<<oriCol<<endl;
// 	cout<<face.x<<' '<<face.y<<' '<<face.width<<' '<<face.height<<endl;
// 	Rect body = face;
// 	body.x = max(0, face.x-face.width/2);
// 	body.y = face.y;
// 	body.width = min(oriCol-body.x, face.width * 2);
// 	body.height = min(oriRow-body.y, face.height * 7);
// 	return body;
// }

// void skinDetection(const vector<Rect>& faces){
// 	// src = imread(argv[1], 1);
// 	namedWindow("original", WINDOW_AUTOSIZE);
// 	imshow("original", src);
// 	// dst = src.clone();
// 	// Mat faceImg = src(face);
// 	// namedWindow("faceImg", WINDOW_AUTOSIZE);
// 	// imshow("faceImg", faceImg);
// 	// GaussianBlur(src, dst, Size(15,15), 0, 0);
// 	// AdaptiveSkinDetector detector;
// 	// vector<double> res = detector.getAllSkinToBodyRatio(src, faces);

// 	// src = src(faceToBody(face, src.rows, src.cols));
// 	// namedWindow(window_name1, WINDOW_AUTOSIZE);
// 	// imshow(window_name1, src);
// 	// cout<<detector.skinToBodyRatio(src, dst)<<endl;
// 	// // detector.run(src, dst, facePos);
// 	// namedWindow(window_name2, WINDOW_AUTOSIZE);
// 	// imshow(window_name2, dst);
// 	// waitKey();
// }

void skinDetection(Mat src){
	// src = imread(argv[1], 1);
	namedWindow("original", WINDOW_AUTOSIZE);
	imshow("original", src);
	Mat dst = src.clone();
	AdaptiveSkinDetector detector;
	detector.run(src, dst);
	// detector.skinDetectFromFaceColor(src, dst);

	namedWindow("Processed", WINDOW_AUTOSIZE);
	imshow("Processed", dst);
	waitKey();
}


int main(int argc, char** argv){
	Mat src;
	// src = imread(argv[1], 1);
	// skinDetection(src);
	// AdaptiveSkinDetector detector;
	// detector.skinDetectFromImgShrink(src);
	// Test::getAllSkinRatiosFromPicDir(argv[1]);



	if(argc < 4){
		cout<<"Error: Please input 3 file names"<<endl;
		return 0;
	}
	else{
		Test::getAllSkinToBodyRatioFromFiles(argv[1], argv[2], argv[3]);
	}


	return 0;
}

