#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(false);
    ofBackground(0);
    ofSetCircleResolution(360);
    ofSetLineWidth(3);
    
    // cargamos los settings;
    settings.load("settings.xml");
    
    saqueInicio = settings.getInt("saqueInicio");
    saqueFin = settings.getInt("saqueFin");
    zascaInterval = settings.getInt("zascaInterval");
    
    puertoCom = settings.get("imuCOM");
    
    diametroRaqueta = 100;
    
    origenGrafica.set(140, 430);

    //
    ximu = new ofxXimu();
    connected = ximu->setup(puertoCom); // with bluetooth
    lastConnectionTry = ofGetElapsedTimeMillis();
    
    //
    zascaSound.loadSound("zascaTennis.aif");
    
    dentroZona = false;
    entraZona = false;
    
    tiempoUltimoZasca = 0;
    
    
    //zascaBar
    zascaBar.set(ofPoint(50,300), 250, 20);
    
    ofAddListener(shake.shakeIt, this, &ofApp::initGame);
    
    //segoeuil.ttf
    fuente.loadFont("segoeuil.ttf", 18);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(ximu == NULL) return;
    // actualizamos el imu
    
    ximu->update();
    
    if(!ximu->running && ofGetElapsedTimeMillis()-lastConnectionTry > 30000){
        //ximu = NULL;
        delete ximu;
        
        ximu = new ofxXimu();
        connected = ximu->setup(puertoCom); // with bluetooth
        lastConnectionTry = ofGetElapsedTimeMillis();
    }
    
    /*
    // comprobamos si esta conectado, sino reconectamos
    // 15 segundos antes de dar un timeout
    long tiempoHearBeat = ofGetElapsedTimeMillis() - ximu.timelLastHeartBeat();
    long tiempoDesdeConexion = ofGetElapsedTimeMillis()-lastConnectionTry;
    
    
    if(tiempoHearBeat>15000){

            
            //ximu.serial.close();
            
            connected = ximu.setup("/dev/tty.x-IMU-D833-RNI-SPP"); // with bluetooth
            tiempoHearBeat = ofGetElapsedTimeMillis();
        
    }
    
    if (ofGetElapsedTimeMillis()-lastConnectionTry > 10000 && !ximu.running) {
        connected = ximu.setup(puertoCom); // with bluetooth
        lastConnectionTry = ofGetElapsedTimeMillis();
    }
    */
    //cout << ximu.running << endl;

    //int newAngle = abs(ximu.angulosEuler.roll);
    int newAngle = ofMap(ximu->angulosEuler.roll, 180, -180, 0, 360, true);
    // miramos si ha cambiado
    if (newAngle != anguloRaqueta) {
        
        //miramos la direccion
        if (newAngle > anguloRaqueta) {
            direccionMovimiento = 1;
        }else{
            direccionMovimiento = 0;
        }
        
        // actualizamos el angulo
        anguloRaqueta = newAngle;
    }
    
    // direccionMovimiento == 0 es palante
    // direccionMovimiento == 1 es patras
    
    if(direccionMovimiento == 1){
        // la raqueta avanza
        
        // la raqueta esta dentro del area
        if ((anguloRaqueta>20 && anguloRaqueta<90)) {
            
            if (!entraZona) {
                // aun no hemos marcado la entrada
                entradaGestura = ofGetElapsedTimeMillis();
                
                dentroZona = true;
                entraZona = true;
            }

        }else{
            // estas fuera
            if(dentroZona){
                
                dentroZona = false;
                entraZona = false;
                
                /// comprobamos que paso mas de N tiempo desde el ultimo zasca
                if((ofGetElapsedTimeMillis() - tiempoUltimoZasca) > zascaInterval){
                    zascaSound.play();
                    tiempoUltimoZasca = ofGetElapsedTimeMillis();
                }
            }
        }
    }else{
        // si la raqueta retrocede anulas todo
        dentroZona = false;
        entraZona = false;
    }
    
    
    
    if(dentroZona && (ofGetElapsedTimeMillis()-entradaGestura > 5000)){
        dentroZona = false;
        entraZona = false;
        
        cout <<  "acaba de salir forzado :: " << (ofGetElapsedTimeMillis()-entradaGestura) << " secs " << ":: angulo "<< anguloRaqueta << endl;
        cout << " ------------------------- " << endl;
    }
    
    
    shake.update(ximu->accMag.accX, ximu->accMag.accY, ximu->accMag.accZ);
 
    int ancho = ofMap(anguloRaqueta, 0, 360, 0, 250);
    zascaBar.setWidth(ancho);
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(ximu == NULL) return;
    // draw angulo de la raqueta
    drawRotacion(anguloRaqueta);
    
    string str;
    str = "framerate: " + ofToString(ofGetFrameRate()) + "\n";
    str += "imu conectado: " + ofToString(connected) + "\n";
    str += "direccion: " + ofToString(direccionMovimiento) + "\n";
    str += "gestura in: " + ofToString(saqueInicio) + "\n";
    str += "gestura out: " + ofToString(saqueFin) + "\n";
    //str += "Acc total: " + ofToString(shake.getAccMagnitude()) + "\n";
    str += "imu batteria: " + ofToString(ximu->batt.battery) + "\n";
    str += "angulo raqueta: " + ofToString(anguloRaqueta);
   
    ofSetColor(255);
    fuente.drawString(str, 30, 40);
}

//--------------------------------------------------------------
void ofApp::drawRotacion(int _angle){
    
    ofPushStyle();
    ofSetColor(255,0,0);
    ofPushMatrix();
    ofTranslate(origenGrafica);
    ofRotateZ(360-saqueFin);
    ofRect(ofPoint(0,-1), diametroRaqueta+2, 1);
    ofPopMatrix();
    ofPopStyle();
    
    ofPushMatrix();
    
    ofPushStyle();
    ofSetColor(200);
    ofPushMatrix();
    ofTranslate(origenGrafica);
    ofRotateZ(360-saqueInicio);
    ofRect(ofPoint(0,-1), diametroRaqueta+2, 1);
    ofPopMatrix();
    ofPopStyle();
    
    
    ofPushStyle();
    
    ofSetColor(255);
    ofTranslate(origenGrafica);
    
    ofNoFill();
    ofCircle(0, 0, diametroRaqueta);
    
    ofSetColor(0,234,255);
    ofFill();
    
    ofRotateZ(_angle*-1);
    ofRect(ofPoint(0,-2), diametroRaqueta+4, 4);
    
    ofPopStyle();
    
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::initGame(float &f){
    cout << "initGame " << f << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}