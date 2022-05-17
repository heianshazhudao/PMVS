#include"ImageSet.hpp"
#include"io.h"

using namespace std;
using namespace cv;

ImageSet::ImageSet(const string&imageDir,const string&parDir)
{
    vector<Mat> Images;
    images=getAllImages(imageDir);
    if(images.size()==0)
    cout<<"无图像"<<endl;
    fstream parfile;
    parfile.open(parDir+"\\temple_par.txt");
    int count=0;
    parfile>>count;
    string name;
    for(int i=0;i<count;i++)
    {
        parfile>>name;
        //导入内参矩阵
        parfile>>kmats[i].at<float>(0,0);
        parfile>>kmats[i].at<float>(0,1);
        parfile>>kmats[i].at<float>(0,2);
        parfile>>kmats[i].at<float>(1,0);
        parfile>>kmats[i].at<float>(1,1);
        parfile>>kmats[i].at<float>(1,2);
        parfile>>kmats[i].at<float>(2,0);
        parfile>>kmats[i].at<float>(2,1);
        parfile>>kmats[i].at<float>(2,2);
        //导入外参矩阵
        parfile>>pmats[i].at<float>(0,0);
        parfile>>pmats[i].at<float>(0,1);
        parfile>>pmats[i].at<float>(0,2);
        parfile>>pmats[i].at<float>(0,3);
        parfile>>pmats[i].at<float>(1,0);
        parfile>>pmats[i].at<float>(1,1);
        parfile>>pmats[i].at<float>(1,2);
        parfile>>pmats[i].at<float>(1,3);
        parfile>>pmats[i].at<float>(2,0);
        parfile>>pmats[i].at<float>(2,1);
        parfile>>pmats[i].at<float>(2,2);
        parfile>>pmats[i].at<float>(2,3);

        kpmats[i]=kmats[i]*pmats[i];

        cout<<kmats[i]<<endl;
        cout<<pmats[i]<<endl;
    }
}