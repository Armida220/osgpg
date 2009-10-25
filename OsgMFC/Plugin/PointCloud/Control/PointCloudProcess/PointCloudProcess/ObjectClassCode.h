//各种可操作对象的分类代码

#pragma once

namespace PointCloudProcess
{
struct CLASSCODE
{
    enum
    {
        NONE,
        ClassCloudStation = 0xEFFFFFFF,
        ClassTriangleMesh = 0xDFFFFFFF,
        ClassSignObjects = 0xCFFFFFFF,
        ClassSignObject = 0xBFFFFFFF,
        PseudSelectedPoint = 0x10FFFFFF,
        PseudSelectedLine = 0x0FFFFFFF,
        PseudSelectedFace = 0x0EFFFFFF,
        PseudMouseDrawBox = 0x0DFFFFFF,
        ALL = 0xFFFFFFFF,
    };
};

}