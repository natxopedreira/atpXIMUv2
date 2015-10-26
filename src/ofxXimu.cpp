//
//  ofxXimu.cpp
//  ofxXIMU
//
//  Created by Koki Ibukuro on 2013/12/10.
//
//

#include "ofxXimu.h"

ofxXimu::ofxXimu() {
    running = false;
}

ofxXimu::~ofxXimu() {
    serial.close();
}

bool ofxXimu::setup(std::string portname) {
    portoCom = portname;
    return serial.setup(portname, B115200);
}

long ofxXimu::timelLastHeartBeat(){
    return lastHeartBeat;
}

void ofxXimu::update() {
    if (!serial.isInitialized()){
        running = false;
        return;
    }
    running = true;
    
    ErrorCode e = ERR_NO_ERROR;
    
    if ((ofGetElapsedTimeMillis()-lastHeartBeat) > 1000) {
        cout << "fai mas de des segundos que non di na" << endl;
        running = false;
        lastHeartBeat = ofGetElapsedTimeMillis();
    }
    
    while (serial.available()) {
        char c = serial.readByte();
        e = ximuReceiver.processNewChar(c);
        // hay mensaje
        lastHeartBeat = ofGetElapsedTimeMillis();
//        std::cout << c;
    }
    
    if(e != ERR_NO_ERROR) {
        std::cout << "ERROR: " << e << std::endl;
    }
    
    if(ximuReceiver.isBattAndThermGetReady()) {
        BattAndThermStruct battAndThermStruct = ximuReceiver.getBattAndTherm();
        batt = battAndThermStruct;
        //std::cout << "battery = " << battAndThermStruct.battery << ", thermometer = " << battAndThermStruct.thermometer << std::endl;
    }
    
    if(ximuReceiver.isInertialAndMagGetReady()) {
        InertialAndMagStruct inertialAndMagStruct = ximuReceiver.getInertialAndMag();
        accMag = inertialAndMagStruct;
        /*
        std::cout
        << "gyr:[" << inertialAndMagStruct.gyrX << ',' << inertialAndMagStruct.gyrY << ',' << inertialAndMagStruct.gyrZ
        << "] acc:[" << inertialAndMagStruct.accX << ',' << inertialAndMagStruct.accY << ',' << inertialAndMagStruct.accZ
        << "] mag:[" << inertialAndMagStruct.magX << ',' << inertialAndMagStruct.magY << ',' << inertialAndMagStruct.magZ
        << ']' << std::endl;*/
    }
    
    if(ximuReceiver.isQuaternionGetReady()) {
        QuaternionStruct quaternionStruct = ximuReceiver.getQuaternion();
        armsQuaternion = quaternionStruct;
        
        Quaternion quaternion = Quaternion(quaternionStruct.w, quaternionStruct.x, quaternionStruct.y, quaternionStruct.z);
        
        EulerAnglesStruct eulerAnglesStruct = quaternion.getEulerAngles();
        angulosEuler = eulerAnglesStruct;
        /*
        std::cout
        << "roll = "
        << eulerAnglesStruct.roll
        << ", pitch = "
        << eulerAnglesStruct.pitch
        << ", yaw = "
        << eulerAnglesStruct.yaw
        << std::endl;*/
    }
    
}