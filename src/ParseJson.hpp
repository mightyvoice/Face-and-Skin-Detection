#ifndef PARSEJSON_H
#define PARSEJSON_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <json/json.h>

using namespace std;

class ParseJson{
private:

public:

	//each vector<int> include a person's x1, y1, x2, y2
	static vector<vector<int> > getBodyPosFromJsonFile(string filePath);
};

#endif