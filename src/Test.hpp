#ifndef TEST_HPP
#define TEST_HPP

#include "GetAllBodySkinRatios.hpp"
#include "MyLib.hpp"

class Test{
private:
public:
	static void procAllPicInDir(string imgDir);
	
	static void getAllSkinToBodyRatioFromFiles(
    				const string picDir, 
    				const string jsonFilePath, 
    				const string resFileName);

	static void getAllSkinRatiosFromPicDir(string picDir);


};

#endif