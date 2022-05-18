#include"global.hpp"


vector<Mat> getAllImages(const string&imageDir)
{
    string dir=imageDir+"\\*.png";
    vector<Mat> imageSet; 
    // //文件句柄
    // long handle;
    // //文件属性
    // struct _finddata_t fileinfo;
    // //第一次查找
    // handle=_findfirst(imageDir.c_str(),&fileinfo);
    // if(handle==-1)
    //     return imageSet;
    // do
    // {
    //     cout<<fileinfo.name<<endl;
    //     Mat im=imread(fileinfo.name);
    //     imageSet.push_back(im);
    // } while (_findnext(handle,&fileinfo));

    // _findclose(handle);
    // return imageSet;
    vector<string> filename;
    glob(imageDir,filename,false);
    size_t count=filename.size();
    cout<<"文件夹中共有"<<count<<"个图片"<<endl;
    for(int i=0;i<count;i++)
    {
        //获取不带路径的文件名，如00001.jpg
        string::size_type iPos=filename[i].find_last_of('/')+1;
        string fn=filename[i].substr(iPos,filename[i].length()-iPos);
        //获取不带后缀的编号00001
        string name=fn.substr(0,fn.rfind("."));
        filename[i]=name;
    }
    sort(filename.begin(),filename.end(),
            [](string a,string b){return stoi(a)<stoi(b);});
    for(auto fn:filename)
    {
        imageSet.push_back(imread(fn));
        imshow(fn,imageSet[imageSet.size()-1]);
        waitKey(1000);
        destroyAllWindows();
    }
    return imageSet;
}