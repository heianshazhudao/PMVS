
#pragma once
#include"global.hpp"
#include"ImageSet.hpp"

using namespace std;
using namespace cv;

int main()
{
    const string imageDir="D:\\workspace\\code\\temple\\temple";
    const string parFileName="D:\\workspace\\code\\temple\\temple";
    ImageSet imageSet(imageDir,parFileName);

    return 0;
}