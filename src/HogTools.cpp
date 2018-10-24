//
//  HogTools.cpp
//  Hog-saliency
//
//  Created by 佐藤賢 on 2018/10/23.
//

#include "HogTools.hpp"

//Convert dlib::rectangle to cv::Rect.
cv::Rect HogTool::dlibRectangleToOpenCV(dlib::rectangle r)
{
    return cv::Rect(cv::Point2i(r.left(),
                                r.top()),
                    cv::Point2i(r.right() + 1, r.bottom() + 1));
}

//Convert cv::Rect to dlib::rectangle.
dlib::rectangle HogTool::openCVRectToDlib(cv::Rect r){
    return dlib::rectangle((long)r.tl().x,
                           (long)r.tl().y,
                           (long)r.br().x - 1,
                           (long)r.br().y - 1);
}

//Load svm file to FHog.
bool HogTool::loadSVM(string file, FHog &hog){
    ifstream fin(file, ios::binary);
    if (!fin)
    {
        cout << "Can't find a trained object detector file " << file << endl;
        return EXIT_FAILURE;
    }
    deserialize(hog, fin);
    bInit = true;
    return EXIT_SUCCESS;
}

//Load svm file without FHog.
bool HogTool::loadSVM(string file){
    ifstream fin(file, ios::binary);
    if (!fin)
    {
        cout << "Can't find a trained object detector file " << file << endl;
        return false;
    }
    deserialize(detector, fin);
    bInit = true;
    return true;
}

//Load svm files.
bool HogTool::loadMultiSVM(string file){
    FHog pupil_detector = FHog();
    if(loadSVM(file, pupil_detector) == EXIT_SUCCESS){
        detectors.push_back(pupil_detector);
        ofLog() << "Regist svm file >> " << file;
        bInit = true;
        return true;
    }else{
        return false;
    }
}

//Get image mat of hog shape.
cv::Mat HogTool::getFhogShapeMat(string file){
    FHog tmp_detector;
    if(loadSVM(file,tmp_detector) == EXIT_SUCCESS){
        dlib::matrix<unsigned char> svmImg = draw_fhog(tmp_detector);
        cv::Mat tmpMat = dlib::toMat(svmImg).clone();
        return tmpMat;
    }else{
        return Mat::ones(100,100,CV_8UC1) * 0;
    }
}

//Data clear.
void HogTool::clear(){
    detectors.clear();
    bInit = false;
}

//Using multi detectors processes.
std::vector<HogTool::FHogData> HogTool::multiUpdate(const cv::Mat& src){
    if(detectors.size() > 0 && src.empty() == false){
        //Init
        detectResults.clear();
        //Update
        dlib::cv_image<dlib::bgr_pixel> cimg(src);
        evaluate_detectors(detectors, cimg, detectResults);
        if(detectResults.size() > 0){
            //Init
            FHogDatas.clear();
            for(auto rd : detectResults){
                if(rd.detection_confidence > 0){
                    FHogData hog;
                    hog.rect = dlibRectangleToOpenCV(rd.rect);
                    hog.id = rd.weight_index;
                    hog.confidence = rd.detection_confidence;
                    FHogDatas.push_back(hog);
                }
            }
        }
    }
    return FHogDatas;
}

//Using single detector process.
std::vector<HogTool::FHogData> HogTool::update(const cv::Mat& src){
    if(src.empty() == false){
        //Init
        FHogDatas.clear();
        //Update
        dlib::cv_image<dlib::bgr_pixel> cimg(src);
        std::vector<dlib::rectangle> rects = detector(cimg);
        if(rects.size() > 0){
            for(auto rd : rects){
                FHogData hog;
                hog.rect = dlibRectangleToOpenCV(rd);
                FHogDatas.push_back(hog);
            }
        }
    }
    return FHogDatas;
}
