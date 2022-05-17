#include<opencv2/opencv.hpp>
#include<string>
#include<iostream>
#include<opencv2/features2d.hpp>
#include"Track.hpp"
#include"ImageSet.hpp"

using namespace std;
using namespace cv;

int main(int argc,char*argv[])
{
    if(argc<2)
    {
        cout<<"使用说明："<<endl;
        cout<<"计算特征点：sparse sift [图像目录] [参数文件] [特征点文件]"<<endl;
        cout<<"计算匹配：sparse match [特征点文件] [匹配文件]"<<endl;
        cout<<"计算轨迹：sparse track [图像目录] [参数文件] [特征点文件] [匹配文件] [轨迹文件]"<<endl;
        cout<<"计算全部：sparse all [图像目录] [参数文件] [特征点文件] [匹配文件] [轨迹文件]"<<endl;
        exit(0);
    }
    string command=argv[1];
    if(command=="sift")
    {
        //特征点计算
        string imageDir=argv[2];
        string parFileName=argv[3];
        string keypointsFileName=argv[4];
        ImageSet imageset(imageDir,parFileName);
        KeyPoints keypoints(imageset.images);
        keypoints.saveTo(keypointsFileName);
        cout<<"保存特征点到文件"<<keypointsFileName<<endl;
    }
    else if(command=="match")
    {
        //特征点匹配
        string keypointsFileName=argv[2];
        string matchesFileName=argv[3];
        KeyPoints keypoints(keypointsFileName);
        Matches matches(keypoints);
        matches.saveTo(matchesFileName);
        cout<<"保存匹配到文件"<<matchesFileName<<endl;
    }
    else if(command=="track")
    {
        //轨迹计算
        string imageDir=argv[2];
        string parFileName=argv[3];
        string keypointsFileName=argv[4];
        string matchesFileName=argv[5];
        string tracksFileName=argv[6];
        ImageSet imageset(imageDir,parFileName);
        KeyPoints keypoints(keypointsFileName);
        Matches matches(matchesFileName);
        TrackList tracklist(keypoints,matches);
        tracklist.triangulate(imageset.kpmats);
        tracklist.saveTo(tracksFileName);
        cout<<tracklist<<endl;
        cout<<"保存轨迹到文件"<<tracksFileName<<endl;
    }
    else if(command=="all")
    {
        string imageDir=argv[2];
        string parFileName=argv[3];
        string keypointsFileName=argv[4];
        string matchesFileName=argv[5];
        string tracksFileName=argv[6];
        ImageSet imageset(imageDir,parFileName);
        KeyPoints keypoints(imageset.images);
        keypoints.saveTo(keypointsFileName);
        cout<<"保存特征点到文件"<<keypointsFileName<<endl;
        Matches matches(keypoints);
        matches.saveTo(matchesFileName);
        cout<<"保存匹配到文件"<<matchesFileName<<endl;
        TrackList trackList(keypoints, matches);
        trackList.triangulate(imageset.kpmats);
        trackList.saveTo(tracksFileName);
        cout<<"保存轨迹到文件"<<tracksFileName<<endl;
    }
    return 0;
    
}