#include"KeyPoints.hpp"
#include<fstream>

KeyPoints::KeyPoints(const vector<Mat>&images):
                    keypointsVec(images.size(),
                    vector<KeyPoint>()),DescriptorVec(images.size(),Mat())
{
    SIFT sift;
    for(int i=0;i<images.size();i++)
    {
        cout<<"计算视图"<<i<<"的特征点,"<<endl;
        sift(images[i],Mat(),keypointsVec[i],DescriptorVec[i]);
        cout<<"特征点共有"<<keypointsVec[i].size()<<"个"<<endl;
    }
}

KeyPoints::KeyPoints(const string&fileName)                 
{
    ifstream ifs(fileName);
    int nImages;
    ifs>>nImages;
    keypointsVec.resize(nImages,vector<KeyPoint>());
    DescriptorVec.resize(nImages);
    for(int i=0;i<nImages;i++)
    {  
        int nPoints;
        ifs>>nPoints;
        DescriptorVec[i]=Mat(nPoints,128,CV_32F);
        for(int j=0;j<nPoints;j++)
        {
            KeyPoint point;
            Mat descriptor=DescriptorVec[i].row[j];
            ifs>>point.pt.x>>point.pt.y;
            for(int k=0;k<descriptor.cols;k++)
                ifs>>descriptor.at<float>(0,k);
            keypointsVec[i].push_back(point);
        }
    }
}

void KeyPoints::saveTo(const string&filename)
{
    ofstream ofs(filename);
    ofs<<keypointsVec.size()<<endl;
    for(int i=0;i<keypointsVec.size();i++)
    {
        ofs<<keypointsVec[i].size()<<endl;
        for(int j=0;j<keypointsVec[i].size();j++)
        {
            ofs<<keypointsVec[i][j].pt.x<<" "
                <<keypointsVec[i][j].pt.y<<" ";
            Mat descriptor=DescriptorVec[i].row[j];
            for(int k=0;k<descriptor.cols;k++)
            {
                if(k==descriptor.cols-1)
                    ofs<<descriptor.at<float>(0,k)<<endl;
                else
                {
                    ofs<<descriptor.at<flaot>(0,k)<<" ";
                }
                
                
            }
        }
    }
}

