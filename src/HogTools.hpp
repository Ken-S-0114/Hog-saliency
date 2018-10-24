//
//  HogTools.hpp
//  Hog-saliency
//
//  Created by 佐藤賢 on 2018/10/23.
//

#ifndef HogTools_hpp
#define HogTools_hpp

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "dlib/opencv.h"
#include "dlib/image_processing.h"
#include "dlib/image_processing/object_detector.h"
#include "dlib/image_processing/scan_fhog_pyramid.h"

#define PYRAMID 6

using namespace cv;
using namespace ofxCv;

class HogTool{
public:
    struct FHogData{
        cv::Rect rect;
        unsigned long id = 0;
        double confidence = -1;
    };

    static cv::Rect dlibRectangleToOpenCV(dlib::rectangle r);
    static dlib::rectangle openCVRectToDlib(cv::Rect r);

private:
    // Detector setting
    typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<PYRAMID> > image_scanner_type;
    typedef dlib::object_detector<image_scanner_type> FHog;

    // Detector
    std::vector<FHog> detectors;
    FHog detector;

    //Data sturact
    std::vector<dlib::rect_detection> detectResults;
    std::vector<HogTool::FHogData> FHogDatas;

    //Init
    bool bInit = false;

public:
    std::vector<HogTool::FHogData> multiUpdate(const cv::Mat& src);
    std::vector<HogTool::FHogData> update(const cv::Mat& src);

    bool loadMultiSVM(string file);
    bool loadSVM(string file, FHog &hog);
    bool loadSVM(string file);

    void clear();
    cv::Mat getFhogShapeMat(string file);
};

#endif /* HogTools_hpp */

