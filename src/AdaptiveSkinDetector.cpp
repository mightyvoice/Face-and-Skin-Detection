#include "AdaptiveSkinDetector.hpp"

AdaptiveSkinDetector::AdaptiveSkinDetector()
{
    initSkinDetectParams();
}

void AdaptiveSkinDetector::initSkinDetectParams(){
     _hueLower=3;
    _hueUpper=33;

    //initialising the global skin color thresholds
    lower=Scalar(3,50,50);
    higher=Scalar(33,255,255);

    //the global histogram is given 0.95% weightage
    _mergeFactor=0.95;

    //setting the historgram computation parameters
    channels.resize(1);
    channels[0]=0;
    h.setChannel(channels);

    histSize.resize(1);
    histSize[0]=30;
    h.setHistSize(histSize);

    ranges.resize(2*channels.size());
    ranges[0]=0;
    ranges[1]=30;
    h.setRange(ranges);

    //////////////Init bad color set//////////////
    ////bad color: 3, 6, 7, 8, 9, 13, 14, 18, 19, 21, 22, 
    ////good color: 10, 11, 12, 13, 14, 15, 16, 17
    memset(badColorSet, 0, sizeof(badColorSet));
    badColorSet[3] = 1;
    badColorSet[4] = 1;
    // badColorSet[5] = 1;
    // badColorSet[6] = 1;
    // badColorSet[7] = 1;
    // badColorSet[8] = 1;
    // badColorSet[9] = 1;
    // badColorSet[13] = 1;
    // badColorSet[14] = 1;
    // badColorSet[18] = 1;
    // badColorSet[19] = 1;
    // badColorSet[21] = 1;
    badColorSet[22] = 1;
    badColorSet[23] = 1;
}


void AdaptiveSkinDetector::run(Mat image, Mat &mask)
{
    cv::cvtColor(image,image,CV_BGR2HSV);
    cv::inRange(image,lower,higher,mask);
    vector<Mat> ch;
    Mat hue;
    cv::split(image,ch);
    ch[0].copyTo(hue);
    //setting the mask for histogram
    h.setMask(mask);

   //build histogram based on global skin color threshols
    Mat hist=h.BuildHistogram(hue,false);

    //normalize the histogram
    cv::normalize(hist,hist,0,1,NORM_MINMAX);
    //update the histograms
    h.setHist(hist);

    //get the histogram thresholds
    vector<int> range1=h.getThreshHist(hist,0.05,0.05);

    _hueLower=range1[0];
    _hueUpper=range1[1];
    // log(__FILE__,__FUNCTION__,__LINE__,DEBUG,"Modified hue threshold 1 %d:%d",_hueLower,_hueUpper);

    //obseve the pixels encountering motion
    Mat mmask=Mat();
    if(!p1.empty())
    {
        Mat motion;
        cv::absdiff(p1,ch[2],motion);
        cv::inRange(motion,Scalar(8,0,0),Scalar(255,0,0),mmask);
        cv::erode(mmask,mmask,Mat());
        cv::dilate(mmask,mmask,Mat());
    }


    //compute a combined mask,representing motion of skin colored pixels
    if(!mmask.empty())
    cv::bitwise_and(mask,mmask,mmask);

    //set the new histogram mask
    h.setMask(mmask);

    //compute the histogram based on updated mask
    Mat shist=h.BuildHistogram(hue,false);
    //normalize the histogram
    cv::normalize(shist,shist,0,1,NORM_MINMAX);

    //merge both the histograms
    h.mergeHistogram(shist,0.02);

    //get the final histogram
    hist=h.getHist();

    //get the histogram thresholds
    h.getThreshHist(hist,0.05,0.05);

    //update the histogram thresholds
    _hueLower=range1[0];
    _hueUpper=range1[1];

    // log(__FILE__,__FUNCTION__,__LINE__,DEBUG,"Modified hue threshold 2 %d:%d",_hueLower,_hueUpper);
    //Mat hist=h.BuildHistogram(hue,false);

    //comptute the new mask
    // cv::cvtColor(image,image,CV_HSV2BGR);
    // mask = image.clone();
    MatIterator_<uchar> it = mask.begin<uchar>(), it_end = mask.end<uchar>();
    MatIterator_<uchar> it1 = hue.begin<uchar>();
    // namedWindow("sg", WINDOW_AUTOSIZE);
    // imshow("sg", mask);
    // waitKey();
    int pixCount = 0;
    int total = 0;
    int badColorCount[30] = {0};
    int totalPix = 0;
    int skinPix = 0;
    for(; it != it_end; ++it,++it1++)
    {
        if(*it)
        {
            // if(!((*it1)>=_hueLower && (*it1)<=_hueUpper) && abs((int)*it1-9) > 3)
            ////bad color: 3, 6, 7, 8, 9, 18, 19, 21, 22,
            ///good color: 7-17, 4-13, 8-22, 7-13, 
            if(!((*it1)>=_hueLower && (*it1)<=_hueUpper) || badColorSet[*it1])
            // if(!((*it1)>=_hueLower && (*it1)<=_hueUpper))
            {
                (*it) = 0;
            } 
            else{
                total += *it1;
                pixCount++;
                // cout<<(int)(*it1)<<' ';
                badColorCount[(int)(*it1)]++;
                skinPix++;
            }
        }
        totalPix++;

    }
    // cout<<"Mask size: "<<mask.rows<<' '<<mask.cols<<' '<<mask.channels()<<endl;
    // cout<<"Pix Count: "<<pixCount<<endl;
    for(int i = 1; i < 30; i++){
        // if(badColorCount[i] > 0) cout<<i<<": "<<badColorCount[i]<<endl;
    }
    ch[2].copyTo(p1);
    cv::cvtColor(image,image,CV_HSV2BGR);
    // cout<<_hueLower<<' '<<_hueUpper<<endl;
    // cout<<pixCount<<endl;
    cout<<"Skin ratio: "<<skinPix*1.0/totalPix<<endl;
}

pair<int, int> AdaptiveSkinDetector::getSkinColorValueRange(Mat image, Mat &mask){
    cv::cvtColor(image,image,CV_BGR2HSV);
    cv::inRange(image,lower,higher,mask);
    vector<Mat> ch;
    Mat hue;
    cv::split(image,ch);
    ch[0].copyTo(hue);
    //setting the mask for histogram
    h.setMask(mask);

   //build histogram based on global skin color threshols
    Mat hist=h.BuildHistogram(hue,false);

    //normalize the histogram
    cv::normalize(hist,hist,0,1,NORM_MINMAX);
    //update the histograms
    h.setHist(hist);

    //get the histogram thresholds
    vector<int> range1=h.getThreshHist(hist,0.05,0.05);

    _hueLower=range1[0];
    _hueUpper=range1[1];
    // log(__FILE__,__FUNCTION__,__LINE__,DEBUG,"Modified hue threshold 1 %d:%d",_hueLower,_hueUpper);

    //obseve the pixels encountering motion
    Mat mmask=Mat();
    if(!p1.empty())
    {
        Mat motion;
        cv::absdiff(p1,ch[2],motion);
        cv::inRange(motion,Scalar(8,0,0),Scalar(255,0,0),mmask);
        cv::erode(mmask,mmask,Mat());
        cv::dilate(mmask,mmask,Mat());
    }

    //compute a combined mask,representing motion of skin colored pixels
    if(!mmask.empty())
    cv::bitwise_and(mask,mmask,mmask);

    //set the new histogram mask
    h.setMask(mmask);

    //compute the histogram based on updated mask
    Mat shist=h.BuildHistogram(hue,false);
    //normalize the histogram
    cv::normalize(shist,shist,0,1,NORM_MINMAX);

    //merge both the histograms
    h.mergeHistogram(shist,0.02);

    //get the final histogram
    hist=h.getHist();

    //get the histogram thresholds
    h.getThreshHist(hist,0.05,0.05);

    //update the histogram thresholds
    _hueLower=range1[0];
    _hueUpper=range1[1];

    // log(__FILE__,__FUNCTION__,__LINE__,DEBUG,"Modified hue threshold 2 %d:%d",_hueLower,_hueUpper);
    //Mat hist=h.BuildHistogram(hue,false);

    //comptute the new mask
    // cv::cvtColor(image,image,CV_HSV2BGR);
    // mask = image.clone();
    MatIterator_<uchar> it = mask.begin<uchar>(), it_end = mask.end<uchar>();
    MatIterator_<uchar> it1 = hue.begin<uchar>();
    // namedWindow("sg", WINDOW_AUTOSIZE);
    // imshow("sg", mask);
    // waitKey();
    int minVal = 2147483647;
    int maxVal = -1;
    for(; it != it_end; ++it,++it1++)
    {
        if(*it)
        {
            // if(!((*it1)>=_hueLower && (*it1)<=_hueUpper) && abs((int)*it1-9) > 3)
            ////bad color: 3, 6, 7, 8, 9, 18, 19, 21, 22,
            ///good color: 7-17, 4-13, 8-22, 7-13, 
            if(!((*it1)>=_hueLower && (*it1)<=_hueUpper))
            {
                (*it) = 0;
            } 
            else{
                minVal = std::min(minVal, (int)(*it1));
                maxVal = std::max(maxVal, (int)(*it1));
            }
        }

    }
    // cout<<"Mask size: "<<mask.rows<<' '<<mask.cols<<' '<<mask.channels()<<endl;
    // cout<<"Pix Count: "<<pixCount<<endl;
    ch[2].copyTo(p1);
    cv::cvtColor(image,image,CV_HSV2BGR);
    return make_pair(minVal, maxVal);
}

void AdaptiveSkinDetector::skinDetectFromBodyImg(Mat image){
    FaceDetection faceDetection;
    vector<Rect> faceRects;
    cout<<image.rows<<endl;
    faceDetection.getAllFaces(image, faceRects);
    cout<<faceRects.size()<<endl;
    Mat bodyImg = image.clone();
    if(faceRects.size() > 0){
        Rect faceRect = faceRects[0];
        cout<<faceRect.width<<' '<<faceRect.height<<endl;
        Rect bodyRect;
        bodyRect.x = faceRect.x - (int)(faceRect.width*0.6);
        bodyRect.y = faceRect.y + faceRect.height/2*3;
        bodyRect.width = (int)(faceRect.width*2);
        bodyRect.height = faceRect.height*3;
        MyLib::limitToBoundary(bodyRect, image.rows, image.cols);
        bodyImg = image(bodyRect);
    }
    Mat mask = bodyImg.clone();
    initSkinDetectParams();
    run(bodyImg, mask);
    namedWindow("original", WINDOW_AUTOSIZE);
    imshow("original", bodyImg);
    namedWindow("Processed", WINDOW_AUTOSIZE);
    imshow("Processed", mask);
    waitKey();
}

double AdaptiveSkinDetector::skinDetectFromImgShrink(Mat image){
    int width = image.cols;
    int height = image.rows;
    Rect bodyRect;
    bodyRect.x = width * 0.2;
    bodyRect.y = (int)(height * 0.25);
    bodyRect.width = (int)(width * 0.65);
    bodyRect.height = (int)(height * 0.6);
    // MyLib::limitToBoundary(bodyRect, width, height);
    Mat bodyImg = image(bodyRect);
    Mat mask = bodyImg.clone();
    initSkinDetectParams();
    return skinToBodyRatio(bodyImg, mask);
    // MyLib::writeMatToJpgFile(mask, "./pic/people-res/"+picName);
    // MyLib::writeMatToJpgFile(bodyImg, "./pic/people-res/frame-"+picName);
    // namedWindow("original", WINDOW_AUTOSIZE);
    // imshow("original", image);
    // namedWindow("original1", WINDOW_AUTOSIZE);
    // imshow("original1", bodyImg);
    // namedWindow("Processed", WINDOW_AUTOSIZE);
    // imshow("Processed", mask);
    // waitKey();
}

double AdaptiveSkinDetector::skinToBodyRatio(Mat image, Mat &mask)
{
    cv::cvtColor(image,image,CV_BGR2HSV);
    cv::inRange(image,lower,higher,mask);
    vector<Mat> ch;
    Mat hue;
    cv::split(image,ch);
    ch[0].copyTo(hue);
    //setting the mask for histogram
    h.setMask(mask);

   //build histogram based on global skin color threshols
    Mat hist=h.BuildHistogram(hue,false);

    //normalize the histogram
    cv::normalize(hist,hist,0,1,NORM_MINMAX);
    //update the histograms
    h.setHist(hist);

    //get the histogram thresholds
    vector<int> range1=h.getThreshHist(hist,0.05,0.05);

    _hueLower=range1[0];
    _hueUpper=range1[1];
    // log(__FILE__,__FUNCTION__,__LINE__,DEBUG,"Modified hue threshold 1 %d:%d",_hueLower,_hueUpper);

    //obseve the pixels encountering motion
    Mat mmask=Mat();
    if(!p1.empty())
    {

        Mat motion;
        cv::absdiff(p1,ch[2],motion);
        cv::inRange(motion,Scalar(8,0,0),Scalar(255,0,0),mmask);
        cv::erode(mmask,mmask,Mat());
        cv::dilate(mmask,mmask,Mat());
    }

    //compute a combined mask,representing motion of skin colored pixels
    if(!mmask.empty())
    cv::bitwise_and(mask,mmask,mmask);

    //set the new histogram mask
    h.setMask(mmask);

    //compute the histogram based on updated mask
    Mat shist=h.BuildHistogram(hue,false);
    //normalize the histogram
    cv::normalize(shist,shist,0,1,NORM_MINMAX);

    //merge both the histograms
    h.mergeHistogram(shist,0.02);

    //get the final histogram
    hist=h.getHist();

    //get the histogram thresholds
    h.getThreshHist(hist,0.05,0.05);

    //update the histogram thresholds
    _hueLower=range1[0];
    _hueUpper=range1[1];

    // log(__FILE__,__FUNCTION__,__LINE__,DEBUG,"Modified hue threshold 2 %d:%d",_hueLower,_hueUpper);
    //Mat hist=h.BuildHistogram(hue,false);

    //comptute the new mask
    // cv::cvtColor(image,image,CV_HSV2BGR);
    // mask = image.clone();
    MatIterator_<uchar> it = mask.begin<uchar>(), it_end = mask.end<uchar>();
    MatIterator_<uchar> it1 = hue.begin<uchar>();
    // namedWindow("sg", WINDOW_AUTOSIZE);
    // imshow("sg", mask);
    // waitKey();
    int totalPix = 0;
    int skinPix = 0;
    for(; it != it_end; ++it,++it1++)
    {
        if(*it)
        {
            if(!((*it1)>=_hueLower && (*it1)<=_hueUpper) || badColorSet[*it1])
            {
                (*it) = 0;
            }
            else{
                skinPix++;
            }
        }
        totalPix++;

    }
    // cout<<"Total Pix: "<<totalPix<<endl;
    // cout<<"Skin Pix: "<<skinPix<<endl;
    // cout<<"Skin Pix ratio: "<<1.0*skinPix/totalPix<<endl;
    // cout<<"Mask size: "<<mask.rows<<' '<<mask.cols<<' '<<mask.channels()<<endl;
    // cout<<"Pix Count: "<<pixCount<<endl;
    ch[2].copyTo(p1);
    cv::cvtColor(image, image, CV_HSV2BGR);
    double res = skinPix*1.0/totalPix;
    // res /= BODY_TO_FRAME_RATIO;
    // if(res >= 1.0) res = 1.0;
    return res;
}

int AdaptiveSkinDetector::faceSkinColorValue(Mat image, Mat &mask)
{
    cv::cvtColor(image,image,CV_BGR2HSV);
    cv::inRange(image,lower,higher,mask);
    vector<Mat> ch;
    Mat hue;
    cv::split(image,ch);
    ch[0].copyTo(hue);
    //setting the mask for histogram
    h.setMask(mask);

   //build histogram based on global skin color threshols
    Mat hist=h.BuildHistogram(hue,false);

    //normalize the histogram
    cv::normalize(hist,hist,0,1,NORM_MINMAX);
    //update the histograms
    h.setHist(hist);

    //get the histogram thresholds
    vector<int> range1=h.getThreshHist(hist,0.05,0.05);

    _hueLower=range1[0];
    _hueUpper=range1[1];
    // log(__FILE__,__FUNCTION__,__LINE__,DEBUG,"Modified hue threshold 1 %d:%d",_hueLower,_hueUpper);

    //obseve the pixels encountering motion
    Mat mmask=Mat();
    if(!p1.empty())
    {

        Mat motion;
        cv::absdiff(p1,ch[2],motion);
        cv::inRange(motion,Scalar(8,0,0),Scalar(255,0,0),mmask);
        cv::erode(mmask,mmask,Mat());
        cv::dilate(mmask,mmask,Mat());
    }

    //compute a combined mask,representing motion of skin colored pixels
    if(!mmask.empty())
    cv::bitwise_and(mask,mmask,mmask);

    //set the new histogram mask
    h.setMask(mmask);

    //compute the histogram based on updated mask
    Mat shist=h.BuildHistogram(hue,false);
    //normalize the histogram
    cv::normalize(shist,shist,0,1,NORM_MINMAX);

    //merge both the histograms
    h.mergeHistogram(shist,0.02);

    //get the final histogram
    hist=h.getHist();

    //get the histogram thresholds
    h.getThreshHist(hist,0.05,0.05);

    //update the histogram thresholds
    _hueLower=range1[0];
    _hueUpper=range1[1];

    // log(__FILE__,__FUNCTION__,__LINE__,DEBUG,"Modified hue threshold 2 %d:%d",_hueLower,_hueUpper);
    //Mat hist=h.BuildHistogram(hue,false);

    //comptute the new mask
    // cv::cvtColor(image,image,CV_HSV2BGR);
    // mask = image.clone();
    MatIterator_<uchar> it = mask.begin<uchar>(), it_end = mask.end<uchar>();
    MatIterator_<uchar> it1 = hue.begin<uchar>();
    // namedWindow("sg", WINDOW_AUTOSIZE);
    // imshow("sg", mask);
    // waitKey();
    int totalSkinPixVal = 0;
    int skinPixCount = 0;
    cout<<_hueLower<<' '<<_hueUpper<<endl;
    for(; it != it_end; ++it,++it1++)
    {
        if(*it)
        {
            if(!(*it1>=_hueLower && *it1<=_hueUpper))
            {
                (*it) = 0;
            }
            else{
                totalSkinPixVal += *it1;
                skinPixCount++;
            }
        }
    }
    // cout<<"Total Pix: "<<totalPix<<endl;
    // cout<<"Skin Pix: "<<skinPix<<endl;
    // cout<<"Skin Pix ratio: "<<1.0*skinPix/totalPix<<endl;
    // cout<<"Mask size: "<<mask.rows<<' '<<mask.cols<<' '<<mask.channels()<<endl;
    // cout<<"Pix Count: "<<pixCount<<endl;
    ch[2].copyTo(p1);
    cv::cvtColor(image, image, CV_HSV2BGR);
    cout<<totalSkinPixVal<<' '<<skinPixCount<<endl;
    return (totalSkinPixVal/skinPixCount);
}

void AdaptiveSkinDetector::skinDetecFromFaceSkinVal(Mat image, Mat &mask, int faceSkinVal)
{
    cv::cvtColor(image,image,CV_BGR2HSV);
    cv::inRange(image,lower,higher,mask);
    vector<Mat> ch;
    Mat hue;
    cv::split(image,ch);
    ch[0].copyTo(hue);
    //setting the mask for histogram
    h.setMask(mask);

   //build histogram based on global skin color threshols
    Mat hist=h.BuildHistogram(hue,false);

    //normalize the histogram
    cv::normalize(hist,hist,0,1,NORM_MINMAX);
    //update the histograms
    h.setHist(hist);

    //get the histogram thresholds
    vector<int> range1=h.getThreshHist(hist,0.05,0.05);

    _hueLower=range1[0];
    _hueUpper=range1[1];
    // log(__FILE__,__FUNCTION__,__LINE__,DEBUG,"Modified hue threshold 1 %d:%d",_hueLower,_hueUpper);

    //obseve the pixels encountering motion
    Mat mmask=Mat();
    if(!p1.empty())
    {

        Mat motion;
        cv::absdiff(p1,ch[2],motion);
        cv::inRange(motion,Scalar(8,0,0),Scalar(255,0,0),mmask);
        cv::erode(mmask,mmask,Mat());
        cv::dilate(mmask,mmask,Mat());
    }

    //compute a combined mask,representing motion of skin colored pixels
    if(!mmask.empty())
    cv::bitwise_and(mask,mmask,mmask);

    //set the new histogram mask
    h.setMask(mmask);

    //compute the histogram based on updated mask
    Mat shist=h.BuildHistogram(hue,false);
    //normalize the histogram
    cv::normalize(shist,shist,0,1,NORM_MINMAX);

    //merge both the histograms
    h.mergeHistogram(shist,0.02);

    //get the final histogram
    hist=h.getHist();

    //get the histogram thresholds
    h.getThreshHist(hist,0.05,0.05);

    //update the histogram thresholds
    _hueLower=range1[0];
    _hueUpper=range1[1];

    // log(__FILE__,__FUNCTION__,__LINE__,DEBUG,"Modified hue threshold 2 %d:%d",_hueLower,_hueUpper);
    //Mat hist=h.BuildHistogram(hue,false);

    //comptute the new mask
    // cv::cvtColor(image,image,CV_HSV2BGR);
    // mask = image.clone();
    MatIterator_<uchar> it = mask.begin<uchar>(), it_end = mask.end<uchar>();
    MatIterator_<uchar> it1 = hue.begin<uchar>();
    // namedWindow("sg", WINDOW_AUTOSIZE);
    // imshow("sg", mask);
    // waitKey();
    for(; it != it_end; ++it,++it1++)
    {
        if(*it)
        {
            if( !( abs(*it1-faceSkinVal) <= FACE_SKIN_EPS))
            {
                (*it) = 0;
            }
        }
    }
    cout<<mask.rows<<' '<<mask.cols<<endl;
    // cout<<"Total Pix: "<<totalPix<<endl;
    // cout<<"Skin Pix: "<<skinPix<<endl;
    // cout<<"Skin Pix ratio: "<<1.0*skinPix/totalPix<<endl;
    // cout<<"Mask size: "<<mask.rows<<' '<<mask.cols<<' '<<mask.channels()<<endl;
    // cout<<"Pix Count: "<<pixCount<<endl;
    ch[2].copyTo(p1);
    cv::cvtColor(image, image, CV_HSV2BGR);
}


