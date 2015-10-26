//
//  ofxXimu.h
//  ofxXIMU
//
//  Created by Koki Ibukuro on 2013/12/10.
//
//

#ifndef __ofxXIMU__ofxXimu__
#define __ofxXIMU__ofxXimu__

#include <iostream>
#include "XimuReceiver.h"
#include "Quaternion.h"
#include "ofSerial.h"
#include "ofUtils.h"

class ofxXimu {
public:
    ofxXimu();
    virtual ~ofxXimu();
    
    bool setup(std::string portname);
    void update();
    long timelLastHeartBeat();
    
    QuaternionStruct armsQuaternion;
    InertialAndMagStruct accMag;
    BattAndThermStruct batt;
    EulerAnglesStruct angulosEuler;
    ofSerial serial;
    
    bool running;
    
    std::string portoCom;
    
protected:
    XimuReceiver ximuReceiver;
    
    long lastHeartBeat;
};

#endif /* defined(__ofxXIMU__ofxXimu__) */
