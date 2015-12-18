
#include "Test.hpp"


void Test::procAllPicInDir(string imgDir){
    vector<string> fileNames = GetAllBodySkinRatios::getAllFilesFromDir(imgDir);
    int n = fileNames.size();
    for(int i = 0; i < n; i++){
    	string imgName = MyLib::getNameFromFileName(fileNames[i]);
    	// cout<<imgName<<endl;
    	Mat peopleImg = imread(imgDir+"/"+fileNames[i]);
        Mat mask = peopleImg.clone();
        AdaptiveSkinDetector detector;
        detector.run(peopleImg, mask);
        MyLib::writeMatToJpgFile(mask, "./npeople-res/"+imgName+"-res.jpg");
    }
}

void Test::getAllSkinToBodyRatioFromFiles(
    const string picDir, 
    const string jsonDir, 
    const string resFileName)
{

	int nakePicNum = 0;
	int nonNakePicNum = 0;

    if(picDir == "" || jsonDir == "" || resFileName == ""){
        cout<<"Please input three file names"<<endl;
        return;
    }
	ofstream resFile(resFileName.c_str(), std::ios::out|std::ios::ate);
	if(!resFile){
	    cout<<"Cannot open the result file: "+resFileName<<endl;
	    return;
	}

    vector<string> picFileNames = MyLib::getAllFileNamesFromDir(picDir);
    // vector<string> jsonFileNames = MyLib::getAllFileNamesFromDir(jsonDir);
    int n = picFileNames.size();
    for(int ii = 0; ii < picFileNames.size(); ii++){
        string picFileName = picFileNames[ii];
    	cout<<picFileName<<endl;
    	string picFilePath = picDir;
    	if(picFilePath[picDir.length()-1] != '/'){
    	    picFilePath += '/';
    	}
    	picFilePath += picFileName;
	
    	// cout<<picFilePath<<' '<<jsonFilePath<<endl;
    	Mat srcImg = imread(picFilePath, 1);
    	if(srcImg.rows == 0 || srcImg.cols == 0){
    	    cout<<"The picture: " + picFilePath + " does not exist"<<endl;
    	    resFile<<0.0<<endl;
    	    break;
    	}
	
		string jsonFilePath = jsonDir;
		if(jsonFilePath[jsonDir.length()-1] != '/'){
			jsonFilePath += '/';
		}
		jsonFilePath += MyLib::getNameFromFileName(picFileName) + ".json";

    	vector<vector<int> > bodyPos = ParseJson::getBodyPosFromJsonFile(jsonFilePath);
    	double res;
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
    	        GetAllBodySkinRatios::fixRectWithinBoundary(bodyRect, srcImg);
    	        // cout<<bodyRect.x<<' '<<bodyRect.y<<' '<<bodyRect.width<<' '<<bodyRect.height<<endl;
    	        Mat bodyImg = srcImg(bodyRect);
    	        // Mat bodyImgCopy = srcImg(bodyRect);
    	        // Mat mask = bodyImg.clone();
    	        AdaptiveSkinDetector detector;
                string bodyImgName = "./pic/bad-pic/"
                                     + MyLib::getNameFromFileName(picFileName) 
                                     + "-"+MyLib::int2String(i);

    	        ratios.push_back(detector.skinDetectFromImgShrink(bodyImg, bodyImgName));
    	    	// MyLib::writeMatToJpgFile(bodyImg, bodyImgName + "-frame-0.jpg");

    	    }
    	    res = *max_element(ratios.begin(), ratios.end());
    	    if(res >= AdaptiveSkinDetector::NAKE_SKIN_RATIO){
    	    	nakePicNum++;
    	    	resFile<<picFileName+": "<<res<<endl;
    	    }
    	    else{
    	    	nonNakePicNum++;
    	    	// resFile<<picFileName+": "<<res<<endl;
    	    }
    	}
    	else{
    	    cout<<"There are no bodies in the picture: " + picFilePath<<endl;
    	    res = 0.0;
    	}
    	// cout<<res<<endl;
    	// resFile<<picFileName+": "<<res<<endl;
    }
	resFile.close();
	const int NAKE_PIC_NUM = 67;
	const int NON_NAKE_PIC_NUM = 71;
	// cout<<"Naked recognition ratio: "<<nakePicNum*100.0/NAKE_PIC_NUM<<"%"<<endl;
	cout<<"None naked recognition ratio: "<<nakePicNum*100.0/NON_NAKE_PIC_NUM<<"%"<<endl;
}

void Test::getAllSkinRatiosFromPicDir(string picDir){
    cout<<picDir<<endl;
    vector<string> picFileNames = MyLib::getAllFileNamesFromDir(picDir);
    for(auto picName: picFileNames){
        string picPath = picDir+"/"+picName;
        // cout<<picPath<<endl;
        if(picPath[picPath.length()-1] == 'g'){
            cout<<picName<<": ";
            Mat srcImg = imread(picPath, 1);
            AdaptiveSkinDetector detector;
            detector.skinDetectFromImgShrink(srcImg);
        }
    }
}