
#include "simple-OSC/simple-OSC.h"

float r;
float g;
float b;

String localIP;
String remoteIP;

int localPort;
int remotePort;
int theTime;
int bounceTimerInterval = 50;

int currentState1 = 1;
int currentState2 = 1;
int pastState1 = 1;
int pastState2 = 1;

Timer publishTimer(2000, timedPublish);

Timer bounceTimer1(bounceTimerInterval, button1, TRUE);
Timer bounceTimer2(bounceTimerInterval, button2, TRUE);

UDP Udp;

IPAddress outIp(192, 168, 105, 147);//your computer IP
unsigned int outPort = 8000; //computer incoming port

void setup() {
    
    localIP = WiFi.localIP();
    Udp.begin(0);

    pinMode(7, OUTPUT);
    
    pinMode(1, INPUT_PULLUP);
    pinMode(2, INPUT_PULLUP);
    
    Particle.function("ledSwitch", ledSwitch);
    
    Particle.variable("time", theTime);
    Particle.variable("localIP", localIP);
    Particle.variable("remoteIP", remoteIP);
    Particle.variable("localPort", localPort);
    Particle.variable("remotePort", remotePort);
    
    publishTimer.start();
}

void loop() {
    
    currentState1 = digitalRead(1);
    currentState2 = digitalRead(2);
    
    if(!bounceTimer1.isActive() && currentState1 == 0){
        pastState1 = currentState1;
        bounceTimer1.start();
    }
    if(!bounceTimer2.isActive() && currentState2 == 0){
        pastState2 = currentState2;
        bounceTimer2.start();
    }
    
    theTime = millis();
}

int ledSwitch(String args){
    digitalWrite(7, args.toInt());
    return 1;
}

void button1(){
    currentState1 = digitalRead(1);
    if(currentState1 == pastState1){
        sendOSC("button1", 1);
    }
    currentState1 = 1;
    pastState1 = 1;
}

void button2(){
    currentState2 = digitalRead(2);
    if(currentState2 == pastState2){
        sendOSC("button2", 1);
    }
    currentState2 = 1;
    pastState2 = 1;
}

void sendOSC(String path, int value){
    OSCMessage outMessage("/"+path);
    outMessage.addInt(value);
    outMessage.send(Udp,outIp,outPort);
}

void timedPublish(){
    OSCMessage outMessage("/timed");
    outMessage.addInt(theTime);
    outMessage.send(Udp,outIp,outPort);
}

/*
https://api.particle.io/v1/devices/2d0029001347343432313031/remotePort?access_token=73324860de578a16cbb22bb4810a326d5e0610b4
*/

