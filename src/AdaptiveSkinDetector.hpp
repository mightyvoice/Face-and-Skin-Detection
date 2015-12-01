#ifndef ADAPTIVESKINDETECTOR_H
#define ADAPTIVESKINDETECTOR_H
// #include "Common/OpenCVCommon.hpp"
// #include "opencv2/gpu/device/common.hpp"

#include "OpenCVCommon.hpp"
#include "Histogram.hpp"
// #include "FaceDetection.hpp"
/**
 * @brief The AdaptiveSkinColor class : Class incapsulating adaptive skin color detector
 * based on the paper An adaptive real-time skin detector based on Hue thresholding:
A comparison on two motion tracking methods by Farhad Dadgostar *, Abdolhossein Sarrafzadeh
 */

class AdaptiveSkinDetector //: public Logger
{
    Mat image;

    //adaptive hue thresholds for skin color detection
    int _hueLower;
    int _hueUpper;

    //global lower and upper thresholds for skin color detection
    Scalar lower;
    Scalar higher;

    Mat hist;

    //histogram merge factor for weighted average
    float _mergeFactor;

    //histogram paramters
    vector<int> histSize;
    vector<float> ranges;
    vector<int> channels;

    //object for histogram computation
    ocv::Histogram h;

    //image required for image motion histogram
    Mat p1;
    Mat srcImg;

    const static int FACE_SKIN_EPS = 8;
    // const static double SKIN_AREA_RATIO_TO_BE_NAKED = 0.4;

    // bool inRect(int x, int y, vector<Rect> faces);
    // void getAllFaces();

public:
    /**
     * @brief AdaptiveSkinColor : constructor
     */
    AdaptiveSkinDetector();
    /**
     * @brief run : main function that performs adaptive skin color detection
     * @param image : input BGR image
     * @param mask : output mask ,1 are skin color pixels
     */
    vector<Rect> allFaces;

    void run(Mat image, Mat &mask);
    // void run(Mat image, Mat &mask, const vector<Rect>& facePos);
    double skinToBodyRatio(Mat image, Mat &mask);
    int faceSkinColorValue(Mat image, Mat &mask);
    void skinDetecFromFaceSkinVal(Mat image, Mat &mask, int faceSkinVal);

};

#endif // ADAPTIVESKINDETECTOR_H
