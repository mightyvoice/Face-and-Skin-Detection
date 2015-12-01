
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

