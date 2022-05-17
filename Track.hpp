#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#include"Feature.hpp"
#include<fstream>
#include"Matches.hpp"
using namespace cv;
using namespace std;
//特征点在所有图像中的轨迹
class Track
{
    public:
    //计算重投影误差
    double reprojectionError(const vector<Mat>&pmats,const Mat&points4D);
    //特征点对应的三维坐标
    double x;
    double y;
    double z;
    //轨迹的重投影误差
    double error;
    //轨迹中的所有二维特征点
    vector<Feature> features;
    //特征点的rgb颜色
    int r;
    int g;
    int b;
    Track();
    //向轨迹中添加特征点
    void addFeature(const Feature &feature);
    //检查轨迹中是否有冲突的特征点
    bool hasConfilct();
    //三角化特征点
    void triangulate(const vector<Mat>&pmats);
    friend ostream&operator<<(ostream&os,const Track&track);
};

//轨迹列表
class TrackList
{
    private:
    //所有轨迹 
    vector<Track> tracks;
    public:
    TrackList(const KeyPoints&keypoints,const Matches&matches);
    TrackList(const string&filename);
    //三角化
    void triangulate(const vector<Mat>&pmats);
    void getColor(const vector<Mat>&images);
    //保存点云文件
    void save2ply(const string&filename);
    void saveTo(const string&filename);
    friend ostream&operator<<(ostream&os,const TrackList&tracklist);
};