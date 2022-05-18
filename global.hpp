#pragma once
#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<opencv2/xfeatures2d/nonfree.hpp>
using namespace std;
using namespace cv;

//从指定路径读取所有图片
vector<Mat> getAllImages(const string&imageDir);
