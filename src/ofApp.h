#pragma once

#include "ofMain.h"
#include "ofxImuShake.h"
#include "ofxXimu.h"
#include "ofxGui.h"
#include "ofxSettings.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
    
    void initGame(float &f);
		
    void drawRotacion(int _angle);
    
    ofPoint origenGrafica;
    int saqueInicio, saqueFin;
    int diametroRaqueta;
    
    
    int anguloRaqueta;
    
    ofxImuShake shake;
    
    ofSoundPlayer zascaSound;
    
    // x-IMU
    ofxXimu * ximu;
    long lastConnectionTry;
    bool connected;
    
    
    ofRectangle zascaBar;
    
    int zascaInterval;
    int direccionMovimiento;
    long tiempoUltimoZasca;
    long entradaGestura;
    
    bool entraZona;
    bool dentroZona;
    
    string puertoCom;
    
    ofTrueTypeFont fuente;
    
    ofxSettings settings;
};
