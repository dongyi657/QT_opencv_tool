#include "lib4opencvtoolmanager.h"
#include "opencv_morphology.h"
#include "opencv_filter.h"
#include "opencv_edge_detection.h"
#include "opencv_draw.h"
#include "opencv_remap.h"
#include "opencv_template_match.h"

QStringList includeMethods(){
#ifdef OPENCV_MORPHOLOGY_H
    Digital_Image_Processing_Method<<OPENCV_MORPHOLOGY_METHOD_NAME;
#endif
#ifdef OPENCV_FILTER_H
    Digital_Image_Processing_Method<<OPENCV_FILTER_METHOD_NAME;
#endif
#ifdef OPENCV_EDGE_DETECTION_H
    Digital_Image_Processing_Method<<OPENCV_EDGE_DETECT_METHOD_NAME;
#endif
#ifdef OPENCV_DRAW_H
    Digital_Image_Processing_Method<<OPENCV_DRAW_METHOD_NAME;
#endif
#ifdef OPENCV_REMAP_H
    Digital_Image_Processing_Method<<OPENCV_REMAP_METHOD_NAME;
#endif
#ifdef OPENCV_REMAP_H
    Digital_Image_Processing_Method<<OPENCV_TEMPLATE_MATCH_METHOD_NAME;
#endif
    return Digital_Image_Processing_Method;
}

lib4opencvtool* Lib4opencvtoolManager::create(args_info *_arginfo)
{
#ifdef OPENCV_MORPHOLOGY_H
    if (_arginfo->method_name==OPENCV_MORPHOLOGY_METHOD_NAME){
        lib4opencvtool* lib4opencvPtr = new opencv_morphology(_arginfo);
        return lib4opencvPtr;
    }
#endif
#ifdef OPENCV_FILTER_H
    if (_arginfo->method_name==OPENCV_FILTER_METHOD_NAME){
        lib4opencvtool* lib4opencvPtr = new opencv_filter(_arginfo);
        return lib4opencvPtr;
    }
#endif
#ifdef OPENCV_EDGE_DETECTION_H
    if (_arginfo->method_name==OPENCV_EDGE_DETECT_METHOD_NAME){
        lib4opencvtool* lib4opencvPtr = new opencv_edge_detection(_arginfo);
        return lib4opencvPtr;
    }
#endif
#ifdef OPENCV_DRAW_H
    if (_arginfo->method_name==OPENCV_DRAW_METHOD_NAME){
        lib4opencvtool* lib4opencvPtr = new opencv_draw(_arginfo);
        return lib4opencvPtr;
    }
#endif
#ifdef OPENCV_REMAP_H
    if (_arginfo->method_name==OPENCV_REMAP_METHOD_NAME){
        lib4opencvtool* lib4opencvPtr = new opencv_remap(_arginfo);
        return lib4opencvPtr;
    }
#endif
#ifdef OPENCV_TEMPLATE_MATCH_H
    if (_arginfo->method_name==OPENCV_TEMPLATE_MATCH_METHOD_NAME){
        lib4opencvtool* lib4opencvPtr = new opencv_template_match(_arginfo);
        return lib4opencvPtr;
    }
#endif
    return NULL;
};


void Lib4opencvtoolManager::destroy(lib4opencvtool* lib4opencvPtr) {
    delete lib4opencvPtr;
};
