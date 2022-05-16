#include<Track.hpp>

void addFeature(const Feature &feature)
{

}

TrackList::TrackList(const vector<Mat>&images)
{ 
    int nFrames=images.size();

    //keypointVec[i][j]:第i张图的第j个特征点
    vector<vector<KeyPoint>> keypointVec(nFrames,vector<KeyPoint>());
    vector<Mat> descriptorsVec(nFrames,Mat());
    cout<<"开始计算特征点..."<<endl;
    computeKeypoints(images,keypointVec,descriptorsVec);
    cout<<"特征点计算结束"<<endl;
    vector<vector<vector<DMatch>>> matcheTables(nFrames,vector<vector<DMatch>> (nFrames,vector<DMatch>()));
    cout<<"开始计算特征点匹配..."<<endl;
    computeMatches(keypointVec,descriptorsVec,matcheTables);
    cout<<"特征点匹配计算结束"<<endl;
    couputeTracks(keypointVec,matcheTables,tracks);
}

void computeTracks(const vector<vector<KeyPoint>> &keypointsVec,
                   const vector<vector<vector<DMatch>>> &matchesTable,
                   vector<Track>& tracks)
{
    int nFrame=(int)keypointsVec.size();
    
    //trackIds[i][j]: 第i张图的第j个特征点对应的track的ID
    vector<vector<int>> trackIds(nFrame,vector<int>());

    for(int i=0;i<keypointsVec.size();i++)
        for(int j=0;j<keypointsVec.size();j++)
        {
            trackIds[i].push_back(-1);
        }
    for(int i=0;i<nFrame;i++)
    {
        cout<<"正在计算视图..."<<i<<endl;
        for(int j=i+1;j<nFrame;j++)
        {
            vector<DMatch> const & matches=matchesTable[i][j];
            if(matches.size()>200)
            {
                for(vector<DMatch>::const_iterator it=matches.begin();it!=matches.end();it++)
                {
                    int trackId1=trackIds[i][it->queryIdx];
                    int trackId2=trackIds[j][it->trainIdx];
                    Point2d p1=keypointsVec[i][it->queryIdx].pt;
                    Point2d p2=keypointsVec[j][it->trainIdx].pt;
                    if(trackId1==-1&&trackId2==-1)
                    {
                        //新增一个track 新track的ID为当前tracks.size()
                        trackIds[i][it->queryIdx]=(int)tracks.size();
                        trackIds[j][it->trainIdx]=(int)tracks.size();
                        Track track;
                        track.addFeature(Feature(i,it->queryIdx,p1.x,p1.y));
                        track.addFeature(Feature(j,it->trainIdx,p2.x,p2.y));
                        tracks.push_back(track);
                    }
                    else if(trackId1!=-1&&trackId2==-1)
                    {
                        trackIds[j][it->trainIdx]=trackId1;
                        Track &track=tracks[trackId1];
                        track.addFeature(Feature(j,it->trainIdx,p2.x,p2.y));
                    }
                    else if(trackId1==-1&&trackId2!=-1)
                    {
                        trackIds[i][it->queryIdx]=trackId2;
                        Track &track=tracks[trackId2];
                        track.addFeature(Feature(j,it->queryIdx,p1.x,p1.y));
                    }
                    else if(trackId1!=trackId2)
                    {
                        //合并track
                        trackIds[j][it->trainIdx]=trackId1;
                        Track &track1=tracks[trackId1];
                        Track &track2=tracks[trackId2];

                        //把track2中的所有特征点合并到track1中
                        for(int k=0;k<track2.features.size();k++)
                        {
                            Feature &feature=track2.features[k];
                            track1.addFeature(feature);
                            trackIds[feature.frameIdx][feature.idx]=trackId1;
                        }
                    }
                }
            }
        }
        //过滤掉冲突的特征点和空的track
        vector<bool> invalidTracks(tracks.size(),false);
        for(int i=0;i<tracks.size();i++)
        {
            Track &track=tracks[i];
            //删除掉视图小于3和有冲突的轨迹
            if(track.features.size()<3||track.features.empty()||track.hasConfilct())
            {
                invalidTracks[i]=true;
            }
        }
        removeTracks(invalidTracks,tracks);
    }
}

void TrackList::triangulate(const vector<Mat>&pmats)
{
    for(int i=0;i<tracks.size();i++)
    tracks[i].triangulate(pmats);
    //过滤重投影误差过大的track
    vector<bool> invalidTracks(tracks.size(),false);
    for(int i=0;i<tracks.size();i++)
    {
        Track&track=tracks[i];
        {
            if(track.error>2)
            {
                invalidTracks[i]=true;
            }
        }
        removeTracks(invalidTracks,tracks);
    }
}

//三角化
void Track::triangulate(const vector<Mat>&pmats)
{
    double minE=1e+10;
    double bestX=0;
    double bestY=0;
    double bestZ=0;
    for(int iter=0;iter<features.size()*2;iter++)
    {
        int i=abs(rand())%features.size();
        int j=abs(rand())%features.size();
        while(i==j)
        {
            j=abs(rand())%features.size();
        }
        Feature& f1=features[i];
        Feature& f2=features[j];

        Mat p1=pmats[f1.frameIdx];
        Mat p2=pmats[f2.frameIdx];
        vector<Point2d> points1,points2;
        points1.push_back(Point2d(f1.x,f1.y));
        points2.push_back(Point2d(f2.x,f2.y));
        Mat points4D;
        triangulatePoints(p1,p2,points1,points2,points4D);
        //齐次坐标除以最后一项
        points4D=points4D/points4D.at<double>(3,0);
        double e=reprojectionError(pmats,points4D);
        if(e<minE)
        {
            minE=e;
            bestX=points4D.at<double>(0,0);
            bestY=points4D.at<double>(1,0);
            bestZ=points4D.at<double>(2,0);
        }
    }
    this->x=bestX;
    this->y=bestY;
    this->z=bestZ;
    this->error=minE;
}

double Track::reprojectionError(const vector<Mat>&pmats,const Mat&points4D)
{
    double e=0;
    for(int i=0;i<features.size();i++)
    {
        Feature &f=features[i];
        Mat projectedPoint=pmats[f.frameIdx]*points4D;
        projectedPoint/=projectedPoint.at<double>(2,0);
        double e_x=f.x-projectedPoint.at<double>(0,0);
        double e_y=f.y-projectedPoint.at<double>(1,0);
        e+=sqrt(e_x*e_x+e_y*e_y);
    }
    return e/features.size();
}

void TrackList::getColor(const vector<Mat>&images)
{
    for(int i=0;i<tracks.size();i++)
    {
        Track &track=tracks[i];
        int r=0,g=0,b=0;
        for(int j=0;j<track.features.size();j++)
        {
            Feature &f=track.features[j];
            b+=images[f.frameIdx].at<Vec3b>(f.y,f.x)[0];
            g+=images[f.frameIdx].at<Vec3b>(f.y,f.x)[1];
            r+=images[f.frameIdx].at<Vec3b>(f.y,f.x)[2];
        }
        track.r/=track.features.size();
        track.g/=track.features.size();
        track.b/=track.features.size();
    }
}

void TrackList::save2ply(const string&filename)
{
    ofstream fout(filename);
    fout<<"ply"<<endl;
    fout<<"format ascii 1.0"<<endl;
    fout<<"element vertex "<<tracks.size()<<endl;
    fout<<"property float x"<<endl;
    fout<<"property float y"<<endl;
    fout<<"property float z"<<endl;
    fout<<"property uchar diffuse_red"<<endl;
    fout<<"property uchar diffuse_green"<<endl;
    fout<<"proerty uchar diffuse_blue"<<endl;
    fout<<"end_header"<<endl;
    for(int i=0;i<tracks.size();i++)
    {
        fout<<tracks[i].x<<" "<<tracks[i].y<<" "<<tracks[i].z<<" "<<
              tracks[i].r<<" "<<tracks[i].g<<" "<<tracks[i].b<<endl;
    }
    fout.close();
}