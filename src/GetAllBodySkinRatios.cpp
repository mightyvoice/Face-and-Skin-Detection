
#include "GetAllBodySkinRatios.hpp"

GetAllBodySkinRatios::GetAllBodySkinRatios(Mat _srcImg){
    srcImg = _srcImg.clone();
    // FaceDetection dec;
    // dec.getAllFaces(srcImg, allFaces);
    // for(auto x: allFaces){
    //     printRect(x);
    // }
}

Mat GetAllBodySkinRatios::extractBodyFromFace(Mat _srcImg, Rect face){
    int oriRow = _srcImg.rows;
    int oriCol = _srcImg.cols;
    // cout<<face.x<<' '<<face.y<<' '<<face.width<<' '<<face.height<<endl;
    Rect body = face;
    body.x = max(0, face.x-face.width/2);
    body.y = max(0, face.y);
    body.width = min(oriCol-body.x-1, face.width * 2);
    body.height = min(oriRow-body.y-1, face.height * 7);
    // cout<<oriRow<<' '<<oriCol<<endl;
    cout<<body.x<<' '<<body.y<<' '<<body.width+body.x<<' '<<body.height+body.y<<endl;
    return _srcImg(body);
}

vector<double> GetAllBodySkinRatios::getAllSkinToBodyRatio(Mat _srcImg, const vector<Rect>& faces){
    vector<double> res;
    // string windowNameSuffix = "a";
    for(int i = 0; i < faces.size(); i++){
        // Mat faceImg = _srcImg(faces[i]);
        Mat body = extractBodyFromFace(_srcImg, faces[i]);
        Mat mask = body.clone();
        AdaptiveSkinDetector detector;
        res.push_back(detector.skinToBodyRatio(body, mask));

        // namedWindow("faceImg"+windowNameSuffix, WINDOW_AUTOSIZE);
        // imshow("faceImg"+windowNameSuffix, faceImg);
        // namedWindow("bodyImg"+windowNameSuffix, WINDOW_AUTOSIZE);
        // imshow("bodyImg"+windowNameSuffix, body);
        // namedWindow("proBodyImg"+windowNameSuffix, WINDOW_AUTOSIZE);
        // imshow("proBodyImg"+windowNameSuffix, mask);
        // windowNameSuffix[0]++;
    }
    // waitKey();
    return res;
}

void GetAllBodySkinRatios::getAllBodyPos(string imgPath){
    Mat srcImg = imread(imgPath, 1);
    vector<Rect> faces;
    FaceDetection faceDetection;
    faceDetection.getAllFaces(srcImg, faces);

    string windowNameSuffix = "a";
    for(int i = 0; i < faces.size(); i++){
        Mat faceImg = srcImg(faces[i]);
        Mat body = extractBodyFromFace(srcImg, faces[i]);
        Mat mask = body.clone();
        AdaptiveSkinDetector detector;
        detector.run(body, mask);

        namedWindow("faceImg"+windowNameSuffix, WINDOW_AUTOSIZE);
        imshow("faceImg"+windowNameSuffix, faceImg);
        namedWindow("bodyImg"+windowNameSuffix, WINDOW_AUTOSIZE);
        imshow("bodyImg"+windowNameSuffix, body);
        namedWindow("proBodyImg"+windowNameSuffix, WINDOW_AUTOSIZE);
        imshow("proBodyImg"+windowNameSuffix, mask);
        windowNameSuffix[0]++;
    }
    waitKey();
}

vector<double> GetAllBodySkinRatios::getAllSkinToBodyRatio(Mat _srcImg, string jsonPath){
    vector<double> res;
    vector<vector<int> > bodyPos = ParseJson::getBodyPosFromJsonFile(jsonPath);
    for(int i = 0; i < bodyPos.size(); i++){
        Rect bodyRect;
        bodyRect.x = bodyPos[i][0];
        bodyRect.y = bodyPos[i][1];
        bodyRect.width = bodyPos[i][2] - bodyRect.x;
        bodyRect.height = bodyPos[i][3] - bodyRect.y;
        Mat bodyImg = _srcImg(bodyRect);
        Mat mask = bodyImg.clone();
        AdaptiveSkinDetector detector;
        res.push_back(detector.skinToBodyRatio(bodyImg, mask));
    }
    return res;
}

void GetAllBodySkinRatios::skinDetecFromFaceSkinVal(Mat _srcImg, const vector<Rect>& faces){
    string windowNameSuffix = "b";
    for(int i = 0; i < faces.size(); i++){
        // Mat faceImg = _srcImg(faces[i]);
        Mat bodyImg = extractBodyFromFace(_srcImg, faces[i]);
        // Mat bodyMask = bodyImg.clone();
        // AdaptiveSkinDetector d;
        // int faceSkinVal = d.faceSkinColorValue(bodyImg, bodyMask);
        // cout<<"face skin val: "<<faceSkinVal<<endl;
        Mat mask = bodyImg.clone();
        AdaptiveSkinDetector dd;
        // dd.skinDetecFromFaceSkinVal(bodyImg, mask, faceSkinVal);
        // dd.runOnEllipseModel(bodyImg, mask);
        Mat mask1 = bodyImg.clone();
        AdaptiveSkinDetector ddd;
        ddd.run(bodyImg, mask1);

        // namedWindow("faceImg"+windowNameSuffix, WINDOW_AUTOSIZE);
        // imshow("faceImg"+windowNameSuffix, faceImg);

        namedWindow("bodyImg"+windowNameSuffix, WINDOW_AUTOSIZE);
        imshow("bodyImg"+windowNameSuffix, bodyImg);

        namedWindow("proBodyImgFromSkinColor"+windowNameSuffix, WINDOW_AUTOSIZE);
        imshow("proBodyImgFromSkinColor"+windowNameSuffix, mask);

        namedWindow("proBodyImg"+windowNameSuffix, WINDOW_AUTOSIZE);
        imshow("proBodyImg"+windowNameSuffix, mask1);
        windowNameSuffix[0]++;
    }
    waitKey();
}

vector<string> GetAllBodySkinRatios::getAllFilesFromDir(string dirPath){
    vector<string> res;
    DIR* dir;
    struct dirent* file;

    if( (dir = opendir(dirPath.c_str())) == NULL){
        cout<<"Can't open the directory: "<<dirPath<<endl;
    }

    while( (file = readdir(dir)) != NULL){
        if(file->d_type == 8 && file->d_name[0] != '.'){
            res.push_back(file->d_name);
            // cout<<file->d_name<<endl;
        }
    }
    closedir(dir);
    return res;
}


void GetAllBodySkinRatios::fixRectWithinBoundary(Rect &cur, Mat srcImg){
    int row = srcImg.rows;
    int col = srcImg.cols;
    if(cur.x + cur.width >= col){
        cur.width = col-cur.x;
    }
    if(cur.y + cur.height >= row){
        cur.height = row-cur.y;
    }
}



void GetAllBodySkinRatios::getAllSkinToBodyRatioFromFiles(
    const string picDir, 
    const string jsonFilePath, 
    const string resFileName)
{

    if(picDir == "" || jsonFilePath == "" || resFileName == ""){
        cout<<"Please input three file names"<<endl;
        return;
    }

    ofstream file(resFileName.c_str(), std::ios::out|std::ios::ate);
    if(!file){
        cout<<"Cannot open the result file: "+resFileName<<endl;
        return;
    }

    string picFileName = "";
    int nameStartIndex;
    for(nameStartIndex = jsonFilePath.length()-1;  nameStartIndex >= 0; nameStartIndex--){
        if(jsonFilePath[nameStartIndex] == '/'){
            break;
        }
    }
    nameStartIndex++;
    for(int i = nameStartIndex; i < jsonFilePath.length(); i++){
        if(jsonFilePath[i] != '.'){
            picFileName += jsonFilePath[i];
        }
        else{
            break;
        }
    }

    double res;
    string picFilePath = picDir;
    if(picFilePath[picDir.length()-1] != '/'){
        picFilePath += '/';
    }
    picFilePath += picFileName + ".jpg";

    // cout<<picFilePath<<' '<<jsonFilePath<<endl;
    Mat srcImg = imread(picFilePath, 1);
    if(srcImg.rows == 0 || srcImg.cols == 0){
        cout<<"The picture: " + picFilePath + " does not exist"<<endl;
        file<<0.0<<endl;
        file.close();
        return;
    }

    vector<vector<int> > bodyPos = ParseJson::getBodyPosFromJsonFile(jsonFilePath);
    if(bodyPos.size() > 0){
        vector<double> ratios;
        for(int i = 0; i < bodyPos.size(); i++){
            Rect bodyRect;
            // cout<<bodyPos[j][0]<<' '<<bodyPos[j][1]<<' '<<bodyPos[j][2]<<' '<<bodyPos[j][3]<<endl;
            bodyRect.x = bodyPos[i][0];
            bodyRect.y = bodyPos[i][1];
            bodyRect.width = bodyPos[i][2] - bodyRect.x;
            bodyRect.height = bodyPos[i][3] - bodyRect.y;
            // cout<<srcImg.cols<<' '<<srcImg.rows<<endl;
            // cout<<bodyRect.x<<' '<<bodyRect.y<<' '<<bodyRect.width<<' '<<bodyRect.height<<endl;
            fixRectWithinBoundary(bodyRect, srcImg);
            // cout<<bodyRect.x<<' '<<bodyRect.y<<' '<<bodyRect.width<<' '<<bodyRect.height<<endl;
            Mat bodyImg = srcImg(bodyRect);
            // Mat mask = bodyImg.clone();
            AdaptiveSkinDetector detector;
            ratios.push_back(detector.skinDetectFromImgShrink(bodyImg));
        }
        res = *max_element(ratios.begin(), ratios.end());
    }
    else{
        // cout<<"There are no bodies in the picture: " + picFilePath<<endl;
        res = 0.0;
    }
    file<<res<<endl;
    file.close();
}


