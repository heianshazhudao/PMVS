#include<iostream>
#include<opencv2/opencv.hpp>
#include"global.h"
#include<fstream>
using namespace std;
using namespace cv;

class ImageSet
{
    
    public:
    ImageSet(const string&imageDir,const string&parDir);
    vector<Mat> images;
    vector<string> imageNames;
    vector<Mat> kmats;  
    vector<Mat> pmats;
    vector<Mat> kpmats;
};