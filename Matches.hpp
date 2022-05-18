#pragma once
#include"global.hpp"
#include"KeyPoints.hpp"
using namespace std;
using namespace cv;

class Matches
{
    
    private:
    void pairwiseMatch(const vector<KeyPoint>&keypoints1,const vector<KeyPoint>&keypoints2,const Mat&descriptors1,const Mat&descriptors2,vector<DMatch> matches);
    vector<vector<DMatch>> matchesTable;
    
    public:
    Matches(const KeyPoints&keypoints);
    Matches(const string&filename);
    const vector<DMatch>& getMatches(int i,int j) const;
    void saveTo(const string&filename);

};