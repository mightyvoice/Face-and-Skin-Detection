
#include "ParseJson.hpp"

vector<vector<int> > ParseJson::getBodyPosFromJsonFile(string filePath){
	ifstream ifs;
    ifs.open(filePath.c_str());
    //assert(ifs.is_open());
 
    Json::Reader reader;
    Json::Value root;
    vector<vector<int> > ans;
    if (!reader.parse(ifs, root, false))
    {
        return ans;
    }
    int itemSize = root["items"].size();
    if(itemSize > 0){
    	for(int i = 0; i < itemSize; i++){
	    	string name = root["items"][i]["item_name"].asString();
	    	if(name == "person"){
	    		std::vector<int> tmp;
	    		tmp.push_back(root["items"][i]["bbx"]["x1"].asInt());
	    		tmp.push_back(root["items"][i]["bbx"]["y1"].asInt());
	    		tmp.push_back(root["items"][i]["bbx"]["x2"].asInt());
	    		tmp.push_back(root["items"][i]["bbx"]["y2"].asInt());
	    		ans.push_back(tmp);
	    	}
    	}
    }
    // for(int i = 0; i < ans.size(); i++){
    // 	for(int j = 0; j < ans[i].size(); j++){
    // 		cout<<ans[i][j]<<' ';
    // 	}
    // 	cout<<endl;
    // }
    return ans;
}
