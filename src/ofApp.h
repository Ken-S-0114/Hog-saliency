//
//  ofApp.h
//  Hog-saliency
//
//  Created by 佐藤賢 on 2018/10/23.
//

#pragma once

#include "ofMain.h"

//OpenCV
#include "ofxOpenCv.h"
#include "ofxCv.h"

//Hogtools
#include "HogTools.hpp"

using namespace cv;
using namespace ofxCv;

enum MDOE{
    CAM,
    PHOTO,
    MOVIE,
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    //Camera
    ofVideoGrabber cam;

    //photo
    ofImage photo;

    //movie
    ofVideoPlayer player;

    //Hog
    HogTool hog;
    vector<HogTool::FHogData> fHogData;
    Mat hogShape;

    //Mode
    int mode;

    //File IO
    string GetExtension(const string &path) {
        string ext;
        size_t pos1 = path.rfind('.');
        if(pos1 != string::npos){
            ext = path.substr(pos1+1, path.size()-pos1);
            string::iterator itr = ext.begin();
            while(itr != ext.end()){
                *itr = tolower(*itr);
                itr++;
            }
            itr = ext.end()-1;
            while(itr != ext.begin()){
                if(*itr == 0 || *itr == 32){
                    ext.erase(itr--);
                }
                else{
                    itr--;
                }
            }
        }
        return ext;
    }
};
