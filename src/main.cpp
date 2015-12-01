
// #include "opencv2/imgproc/imgproc.hpp"
// #include "opencv2/highgui/highgui.hpp"
// #include "OpenCVCommon.hpp"
// #include "AdaptiveSkinDetector.hpp"
// #include "FaceDetection.hpp"
// #include "GetAllBodySkinRatios.hpp"
// #include "/usr/local/Cellar/jsoncpp/0.10.5/include/json/json.h"
#include "ParseJson.hpp"
#include "GetAllBodySkinRatios.hpp"


// Mat src;
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

// void testJsonCPP(){
// 	// Json::Value json_temp;      //临时对象，供如下代码使用
// 	// json_temp["name"] = Json::Value("huchao");
// 	// json_temp["age"] = Json::Value(26);
// 	// Json::Value root;  //表示整个json对象
// 	// root["key_string"] = Json::Value("value_string");         //新建一个Key（名为：key_string），赋予字符串值："value_string"。 
// 	// root["key_number"] = Json::Value(12345);            //新建一个Key（名为：key_number），赋予数值：12345。 
// 	// root["key_boolean"] = Json::Value(false);              //新建一个Key（名为：key_boolean），赋予bool值：false。
// 	// root["key_double"] = Json::Value(12.345);            //新建一个Key（名为：key_double），赋予double值：12.345。
// 	// root["key_object"] = json_temp;                           //新建一个Key（名为：key_object），赋予json::Value对象值。
// 	// root["key_array"].append("array_string");             //新建一个Key（名为：key_array），类型为数组，对第一个元素赋值为字符串："array_string"。
// 	// root["key_array"].append(1234);                           //为数组key_array赋值，对第二个元素赋值为：1234。
// 	// Json::ValueType type = root.type();                       //获得root的类型，此处为objectValue类型。
// }

int main(int argc, char** argv){
	// src = imread(argv[1], 1);
	// GetAllBodySkinRatios a(src);
	// FaceDetection detector;
	// detector.getAllFaces(src, a.allFaces);
	// // vector<double> tmp = a.getAllSkinToBodyRatio(src, a.allFaces);
	// // for(auto x: tmp){
	// // 	cout<<x<<endl;
	// // }
	// a.skinDetecFromFaceSknVal(src, a.allFaces);
	// vector<vector<int> > ans = ParseJson::getBodyPosFromJsonFile("20.json");
	if(argc < 4){
		cout<<"Error: Please input 3 file names"<<endl;
		return 0;
	}
	else{
		GetAllBodySkinRatios::getAllSkinToBodyRatioFromFiles(argv[1], argv[2], argv[3]);
	}
	// GetAllBodySkinRatios::getAllBodyPos("girl.jpg");
	return 0;
}

