
#ifndef GETALLBODYSKINRATIOS_H
#define GETALLBODYSKINRATIOS_H

#include "OpenCVCommon.hpp"
#include "FaceDetection.hpp"
#include "AdaptiveSkinDetector.hpp"
#include "ParseJson.hpp"


class GetAllBodySkinRatios{
private:
	Mat srcImg;
	void printRect(Rect x){
        cout<<x.x<<' '<<x.y<<' '<<x.width<<' '<<x.height<<endl;
    };

public:
  const static double NAKE_SKIN_RATIO = 0.40;
	vector<Rect> allFaces;
	GetAllBodySkinRatios(){};
	GetAllBodySkinRatios(Mat _srcImg);
	static vector<string> getAllFilesFromDir(string dirPath);
	static Mat extractBodyFromFace(Mat _srcImg, Rect face);
  static vector<double> getAllSkinToBodyRatio(Mat _srcImg, const vector<Rect>& facePos);
  static vector<double> getAllSkinToBodyRatio(Mat _srcImg, string jasonPath);
  static void skinDetecFromFaceSkinVal(Mat _srcImg, const vector<Rect>& faces);
  static void getAllBodyPos(string imgPath);
  static void fixRectWithinBoundary(Rect &cur, Mat srcImg);
  static void getAllSkinToBodyRatioFromFiles(
    const string picDir, 
    const string jsonFileName, 
    const string resFileName);
};

#endif