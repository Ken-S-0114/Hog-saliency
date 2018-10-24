//
//  ofApp.cpp
//  Hog-saliency
//
//  Created by 佐藤賢 on 2018/10/23.
//
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //Camera
    {
        cam.initGrabber(640, 480);
        ofSetWindowShape(640, 480);
    }

    //Hog
    {
        //Load svm file
        if(hog.loadMultiSVM(ofToDataPath("test.svm")))
        {
            ofLog() << "[LOG] %s\n ","Load svm";
        }else{
            ofLogError() << "Cannot load svm";
        }

        hogShape = hog.getFhogShapeMat(ofToDataPath("face.svm")).clone();
    }

    //Set default mode
    mode = CAM;
}

//--------------------------------------------------------------
void ofApp::update(){
    Mat frameMat;

    //Input images
    switch(mode){
        case CAM:
        {
            cam.update();
            if(cam.isFrameNew()){
                frameMat = toCv(cam);
                fHogData.clear();
                fHogData = hog.multiUpdate(frameMat);

            }
        }
            break;
        case PHOTO:
        {
            if(photo.isAllocated()){
                frameMat = toCv(photo);
                fHogData.clear();
                fHogData = hog.multiUpdate(frameMat);
            }
        }
            break;
        case MOVIE:
        {
            player.update();
            if(player.isFrameNew()){
                frameMat = toCv(player);
                fHogData.clear();
                fHogData = hog.multiUpdate(frameMat);
            }
        }
            break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //Draw image
    switch(mode){
        case CAM:
        {
            cam.draw(0, 0);
        }
            break;
        case PHOTO:
        {
            photo.draw(0, 0);
        }
            break;
        case MOVIE:
        {
            player.draw(0,0);
            break;
        }
    }

    //Draw fHogdata
    ofPushStyle();
    ofNoFill();
    ofSetColor(255, 0, 0);
    for(auto fhog : fHogData){
        ofRectangle rect = toOf(fhog.rect);
        ofDrawRectangle(rect);
        ofDrawBitmapStringHighlight("ID:"+ofToString(fhog.id) , rect.x,rect.y);
        ofDrawBitmapStringHighlight("confidence:"+ofToString(fhog.confidence) , rect.x,rect.y+20);
    }
    ofPopStyle();

    //how to use
    ofDrawBitmapStringHighlight("Select mode : Push key [1 > camera, 2 > Photo, 3 > Moive]", 20,20);
    ofDrawBitmapStringHighlight("Drop file [jpg,png,gif,mov,mp4]", 20,40);
    //fps
    ofDrawBitmapStringHighlight("FPS:"+ofToString(ofGetFrameRate()), 20,60);

    //Draw Hogimage
    ofDrawBitmapStringHighlight("HogImage", 20, 80);
    drawMat(hogShape, 16, 85, 100, 100);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case '1':
            mode = CAM;
            break;
        case '2':
            mode = PHOTO;
            break;
        case '3':
            mode = MOVIE;
            break;
        case 'r':
            fHogData.clear();
            hog.clear();
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    //Load files
    {
        for(auto file : dragInfo.files){
            string ext = GetExtension(file);
            if(ext == "svm"){
                if(hog.loadMultiSVM(file))
                {
                    hogShape = hog.getFhogShapeMat(file).clone();
                    ofLog() << "[LOG] %s\n ","Load svm";
                }else{
                    ofLogError() << "Cannot load svm";
                }
            }else if(ext == "jpg" || ext == "png" || ext == "gif"){
                photo.load(file);
                photo.update();

                mode = PHOTO;

                ofLog() << "Load photo >> " + file;
            }else if(ext == "mov" || ext == "mp4"){
                player.load(file);
                player.play();

                mode = MOVIE;

                ofLog() << "Load movie >> " + file;
            }
        }
    }
}
