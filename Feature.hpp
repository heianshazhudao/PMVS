#pragma once
#include"global.hpp"

//图像中的特征点
class Feature
{
    public:
    int frameIdx;   //特征点所在图像的编号
    int idx;        //特征点在该图像所有特征点中的序号
    double x;       //特征点在图像中的x坐标
    double y;       //特征点在图像中的y坐标
    Feature(int FrameIdx,int idx,double x,double y);
    Feature();
};