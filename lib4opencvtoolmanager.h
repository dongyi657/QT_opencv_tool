#ifndef LIB4OPENCVTOOLMANAGER_H
#define LIB4OPENCVTOOLMANAGER_H
#include "lib4opencvtool.h"

static QStringList Digital_Image_Processing_Method;
QStringList includeMethods();

class Lib4opencvtoolManager {
public:
    static lib4opencvtool* create(args_info *_arginfo);     // 创建lib4opencvtool实例
    static void destroy(lib4opencvtool* LibCVPtr);   // 销毁lib4opencvtool实例
};

#endif // LIB4OPENCVTOOLMANAGER_H
