#include "rectangle.h"


//------------------------------------------------------------------
rectangle::rectangle(){
//    printf("rectangle");
}

void rectangle::update(){
    
//    pos.x = pos.x + acc;
}

//------------------------------------------------------------------
void rectangle::draw() {
    ofFill();
    //ofSetRectMode(OF_RECTMODE_CENTER); // center around the position
    ofSetColor(198,246,55);
    ofDrawRectangle(pos.x, pos.y, 5,ofGetHeight());
}


//------------------------------------------------------------------
void rectangle::interpolateByPct(float myPct){
    pct = myPct;
//    pos.x = (1-pct) * posa.x + (pct) * posb.x;
//    pos.y = (1-pct) * posa.y + (pct) * posb.y;
    
    // this is an alternative way to write above..
    // differe style, but the same mathematically.
    pos.x = posa.x + (pct) * (posb.x-posa.x);
    pos.y = posa.y + (pct) * (posb.y-posa.y);
//    pos.x = pos.x + 0.5;
    //pos.y = pos.y + 0.5;
}
