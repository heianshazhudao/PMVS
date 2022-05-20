
#include"global.hpp"
using namespace std;
using namespace cv;

class ImageSet
{
    public:

    ImageSet(){};
    ImageSet(const string&imageDir,const string&parDir);
    vector<Mat> images;
    vector<string> imageNames;
    vector<Mat> kmats;  
    vector<Mat> pmats;
    vector<Mat> kpmats;
};