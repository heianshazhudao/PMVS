#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class KeyPoints
{

    private:
    vector<vector<KeyPoint>> keypointsVec;
    vector<Mat> DescriptorVec;

    public:
    KeyPoints(const vector<Mat>&images);
    KeyPoints(const string&filename);
    int getFrameNum() const;
    int getKeyPointNum(int i) const;
    KeyPoint getKeyPoint(int i,int j) const;
    const vector<KeyPoint>& getKeypoints(int i) const;
    const Mat& getDescriptors(int i) const;
    void saveTo(const string &filename);
};