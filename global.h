#include<iostream>
#include<opencv2/opencv.hpp>
#include<io.h>
using namespace std;
using namespace cv;

//从指定路径读取所有图片
vector<Mat> getAllImages(const string&imageDir);
