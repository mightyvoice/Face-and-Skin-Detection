
进入SkinDetection文件夹

1. 安装jsoncpp
步骤：

解压当前目录下的jsoncpp-master.zip得到文件夹jsoncpp-master

打开终端进入当前目录，然后输入以下命令安装jsoncpp:

cd jsoncpp-master
mkdir -p build/debug
cd build/debug
cmake -DCMAKE_BUILD_TYPE=debug -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF -DARCHIVE_INSTALL_DIR=. -G "Unix Makefiles" ../..
make
sudo make install

2. 生成执行文件 SkinDetection
步骤：

回到文件夹SkinDetection

如果没有安装cmake, 如下命令安装cmake
sudo apt-get install cmake

安装完成后输入命令
cmake .
然后输入
make
最后就会生成可执行文件 SkinDetection

3. 怎样运行SkinDetection
输入命令
./SkinDetection 图片文件夹路径 json文件路径 结果文件路径


#############代码文档##################
主要函数:

GetAllBodySkinRatios.cpp文件里的

	getAllSkinToBodyRatioFromFiles(
	    const string picDir, 
	    const string jsonFileName, 
	    const string resFileName);
	输入：picture的文件夹路径，json文件路径和计算结果要存的文件的路径
	输出：把计算结果输出到要存的文件里面
	
	fixRectWithinBoundary(Rect &cur, Mat srcImg)
	输入：人身体的Rect框，原始的图片
	输出：把输入的Rect的边界限定在srcImg的范围内

AdaptiveSkinDetector.cpp里的

	double skinToBodyRatio(Mat image, Mat &mask)
	输入：原始图片，原始图片的一个copy
	输出：输入图片中皮肤占整个图片的比例



