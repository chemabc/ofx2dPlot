#include "ofx2dPlot.h"


ofx2dPlot::ofx2dPlot(int x, int y, int w, int h,
                     int xData, int yData, int wData, int hData,
                     string title, string unitsX, string unitsY, int skipUnits,
                     int xNumDivisions,
                     float yMin, float yMax, int yNumDivisions){

    s_title = title;
    s_unitsX = unitsX;
    s_unitsY = unitsY;
    i_skipUnitsX = skipUnits;
    i_skipUnitsY = skipUnits;

    col_title = ofColor(0,255,255,255);
    col_data = ofColor(255,255,0,150);
    col_metaData = col_data;
    col_grid = ofColor(150,150,150,60);
    col_axis = ofColor(150,150,150,250);
    col_frame = ofColor(0,0,255,0);
    s_font = "";
    i_fontSize = -1;
    s_font2 = "";
    i_fontSize2 = -1;
    pt_titlePos.x = 0;
    pt_titlePos.y = 0;


    i_x = x;
    i_y = y;
    i_w = w;
    i_h = h;

    i_xData = xData;
    i_yData = yData;
    i_wData = wData;
    i_hData = hData;

    i_xNumDivisions = xNumDivisions;

    f_yMin = yMin;
    f_yMax = yMax;
    i_yNumDivisions = yNumDivisions;

    f_yUpLimit = f_yMax;
    f_yDownLimit = f_yMin;
    b_useLimits = false;

    f_zoomX = 1.0;
    f_zoomY = 1.0;

    b_showAxisValuesX, b_showAxisValuesY = true;

    b_debug = false;

    setup();

}

ofx2dPlot::~ofx2dPlot(){
    dq_valuesX.clear();
    dq_valuesY.clear();
    poly_values.clear();
}

void ofx2dPlot::setup(){

    setupGrid();
    setupValues();


}
void ofx2dPlot::update(){
    updateGrid();
    updateValues();


}
void ofx2dPlot::draw(){
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(1.0);
    ofPushMatrix();
        ofTranslate(i_x, i_y);
        //Frame
        ofSetColor(col_frame);
        ofRect(0,0, i_w, i_h);

        //Title
        ofSetColor(col_title);
        if(i_fontSize != -1){
            font.drawString(s_title, pt_titlePos.x, pt_titlePos.y);
        }
        else ofDrawBitmapString(s_title, pt_titlePos.x, pt_titlePos.y);

        if(b_debug){
            ofDrawBitmapString("N:" + ofToString(dq_valuesY.size(),0), pt_titlePos.x, 45);
            string sDebug = "yMin:" + ofToString(f_yMin, 1) + "yMax: " + ofToString(f_yMax, 1) + "\n";
//            for(int i=0; i < dq_valuesY.size(); i++){
//                sDebug += "(" + ofToString(i) + ")" + ofToString(dq_valuesY.at(i)) + "\n";
//            }
//            ofDrawBitmapString("(debug) " + sDebug, i_w-100, 60);
        }
        //ofDrawBitmapString("y: [" + ofToString(f_yMin,1) + "," + ofToString(f_yMax,1) +"]; Step: " + ofToString(f_yStep) + "." , i_w-100, 60);

        //Grid and data
        ofPushMatrix();
            ofTranslate(i_xData, i_yData, 0.0);
            ofScale(f_zoomX,f_zoomY,1.0);
            drawGrid();
            drawValues();
        ofPopMatrix();

    ofPopMatrix();
    ofPopStyle();

}

/********************************************************************************************************
                                        METADATA
********************************************************************************************************/

void ofx2dPlot::setTitle(string s){
    s_title = s;
}
void ofx2dPlot::setUnitsName(string sX, string sY){
    s_unitsX = sX;
    s_unitsY = sY;
}
void ofx2dPlot::setDebug(bool b){
    b_debug = b;
}
bool ofx2dPlot::getDebug(){
    return b_debug;
}
/********************************************************************************************************
                                        STYLE
********************************************************************************************************/
void ofx2dPlot::setColorTitle(ofColor c){
    col_title = c;
}
void ofx2dPlot::setTitlePos(int x, int y){
    pt_titlePos.x = x;
    pt_titlePos.y = y;
}
void ofx2dPlot::setColorMetaData(ofColor c){
    col_metaData = c;
}
void ofx2dPlot::setColorData(ofColor c){
    col_data = c;
}
void ofx2dPlot::setColorGrid(ofColor c){
    col_grid = c;
}
void ofx2dPlot::setColorFrame(ofColor c){
    col_frame = c;
}
void ofx2dPlot::setFont(string sFont, int iSize){
    if(sFont != ""){
        if(font.loadFont(sFont, iSize, true, true)){
            s_font = sFont;
            i_fontSize = iSize;
        }
    }else{
            s_font = "";
            i_fontSize = -1;
    }
}
void ofx2dPlot::setFont2(string sFont, int iSize){
    if(sFont != ""){
        if(font2.loadFont(sFont, iSize, true, true)){
            s_font2 = sFont;
            i_fontSize2 = iSize;
        }
    }else{
            s_font2 = "";
            i_fontSize2 = -1;
    }
}


/********************************************************************************************************
                                        FRAME & GRID
********************************************************************************************************/
void ofx2dPlot::setGridSkipUnits(int skipX, int skipY){
    if(skipX >= 1)
        i_skipUnitsX = skipX;
    else i_skipUnitsX = 1;

    if(skipY >= 1)
        i_skipUnitsY = skipY;
    else i_skipUnitsY = 1;
}
void ofx2dPlot::setOffset(int xD, int yD){
    i_xData = xD;
    i_yData = yD;
}
void ofx2dPlot::setZoom(float fx, float fy){
    f_zoomX = fx;
    f_zoomY = fy;
}
void ofx2dPlot::setNumDivisions(int xNumDiv, int yNumDiv){
    i_xNumDivisions = xNumDiv;
    i_yNumDivisions = yNumDiv;
    b_newGrid = true; //to update values
}

void ofx2dPlot::setupGrid(){
    mesh_grid.setMode(OF_PRIMITIVE_LINES);
    b_newGrid = true;
}
void ofx2dPlot::setShowAxisValues(bool showAxisValuesX, bool showAxisValuesY){
    b_showAxisValuesX = showAxisValuesX;
    b_showAxisValuesY = showAxisValuesY;

}
void ofx2dPlot::updateGrid(){
    if(b_newGrid){
        i_pixStepX =  i_wData/i_xNumDivisions;
        i_pixStepY =  i_hData/i_yNumDivisions;

        mesh_grid.clear();
        //Vertical lines
        for(int i=0; i<= i_xNumDivisions; i++){
            mesh_grid.addVertex(ofVec2f(i*i_pixStepX, 0));
            mesh_grid.addVertex(ofVec2f(i*i_pixStepX, i_hData));
        }
        //Horizontal lines and string values
        vec_yValues.clear();
        f_yStep = (f_yMax-f_yMin)/i_yNumDivisions;
        for(int j=0; j<= i_yNumDivisions; j++){
            mesh_grid.addVertex(ofVec2f(0, j*i_pixStepY));
            mesh_grid.addVertex(ofVec2f(i_wData, j*i_pixStepY));
            vec_yValues.push_back(ofToString(f_yMin + j*f_yStep,1));
        }

        //ofLogVerbose("[ofx2dPlot::setupGrid] done. Lines=["+ofToString(i_xNumDivisions) + "," +ofToString(i_yNumDivisions) + "]");
        b_newGrid = false;
    }

    if(b_newValue){
        //Write "X" value in axis
        vec_xValues.clear();
        for(int i= 0; i<= i_xNumDivisions; i++){
            int indexDqValues = i*i_pixStepX;
            if(indexDqValues < dq_valuesX.size() && dq_valuesX.size() > 0){
//                float xVal = dq_values.at(indexDqValues).x;
                //string sVal = ofToString(xVal,1);
                //vec_xValues.push_back(sVal);
                vec_xValues.push_back(dq_valuesX.at(indexDqValues));
            }
        }
    }

}
void ofx2dPlot::drawGrid(){
    ofPushStyle();
    ofSetColor(col_grid);
    ofDisableAntiAliasing();
    //Draw mesh grid
    mesh_grid.draw();

    ofSetLineWidth(2);
    ofSetColor(col_axis);
    ofLine(0,0,0,i_hData);
    ofLine(0,i_hData,i_wData,i_hData);

    ofSetColor(col_data);
    //Draw updated X string values
    if(b_showAxisValuesX){
        for(int i=0; i<=i_xNumDivisions; i+=i_skipUnitsX){
            if(i<vec_xValues.size()){
                if(i_fontSize2 != -1){
                        //font.drawString(vec_xValues.at(i), i*i_pixStepX-(10.0/f_zoomX), i_h+(20.0/f_zoomY));
                        font2.drawString(vec_xValues.at(i), i*i_pixStepX-10.0, i_hData+(20.0));
                } else ofDrawBitmapString(vec_xValues.at(i), i*i_pixStepX-10.0, i_hData+20.0);
            }
        }
    }

    if(b_showAxisValuesY){
        //Draw updated Y string values
        for(int j=0; j<=i_yNumDivisions; j+=i_skipUnitsY){
            if(j<vec_yValues.size()){
                if(i_fontSize2 != -1){
                    //font.drawString(vec_yValues.at(j), -50.0/f_zoomX, (i_yNumDivisions-j)*i_pixStepY+(0.0/f_zoomY));
                    font2.drawString(vec_yValues.at(j), -50.0, (i_yNumDivisions-j)*i_pixStepY+(0.0));
                } else ofDrawBitmapString(vec_yValues.at(j), -30.0, (i_yNumDivisions-j)*i_pixStepY);
            }

        }
    }

    //Draw X Axis
    ofSetColor(col_metaData);
    if(i_fontSize2 != -1){
        if(b_showAxisValuesX)font2.drawString(s_unitsX, i_wData + 5, i_hData + 15);
        if(b_showAxisValuesY)font2.drawString(s_unitsY, -15, 0);
    }else{
        if(b_showAxisValuesX)ofDrawBitmapString(s_unitsX, i_wData, i_hData + 15); //X axis title
        if(b_showAxisValuesY)ofDrawBitmapString(s_unitsY, 5, i_yData); //Y axis title
    }

    ofEnableAntiAliasing();
    ofPopStyle();
}


/********************************************************************************************************
                                    VALUES - DATA
********************************************************************************************************/
void ofx2dPlot::setFixedMinAndMaxYGridValues(bool fixedMin, bool fixedMax){
    b_fixedMinGridY = fixedMin;
    b_fixedMaxGridY = fixedMax;
}

void ofx2dPlot::addValueXY(string x, float y){
    //Update min and max values instantly
    if(!b_fixedMinGridY && y < f_yMin){
        f_yMin = y;
        b_newGrid = true;
    }
    if(!b_fixedMaxGridY && y > f_yMax){
        f_yMax = y;
        b_newGrid = true;
    }

    //Add value to queue
    if(dq_valuesY.size() >= i_wData){
        //Elimina el primero
        dq_valuesX.pop_front();
        dq_valuesY.pop_front();
    }
    dq_valuesX.push_back(x);
    dq_valuesY.push_back(y);
    b_newValue = true;
}

void ofx2dPlot::setupValues(){
    b_newValue = false;
    dq_valuesX.clear();
    dq_valuesY.clear();
    poly_values.clear();
}

void ofx2dPlot::updateValues(){
    if(b_newValue){
        poly_values.clear();

        float f_newMin = 99999999;
        float f_newMax = -99999999;

        for(int i= 0; i<dq_valuesY.size()-1; i++){
            float val = dq_valuesY.at(i);
            float valMapped = 0;
            if(f_yMax-f_yMin!=0)
                 valMapped = ofMap(val, f_yMin, f_yMax, i_hData, 0);
            else valMapped = -1.0;
            //if(b_debug)cout << "Val ("<< i << "):" << val << " (["<< f_yMin<<","<<f_yMax<<"]); ValMapped: "<< valMapped << endl;
            poly_values.addVertex(i, valMapped);
            //poly_values.curveTo(i, valMapped);

            if(val > f_newMax) f_newMax = val;
            if(val < f_newMin) f_newMin = val;
        }

        b_newValue = false;
        if(!b_fixedMinGridY && f_newMin > f_yMin){
          f_yMin = f_newMin;
          b_newGrid = true;
        }
        if(!b_fixedMaxGridY && f_newMax < f_yMax){
          f_yMax = f_newMax;
          b_newGrid = true;
        }
    }
}
void ofx2dPlot::drawValues(){
    ofSetColor(col_data);
    //poly_values.getSmoothed(2).draw();
    poly_values.draw();
    if(b_useLimits){
        ofPushStyle();
        ofFill();
        vector<ofPoint> vertices = poly_values.getVertices();
        if(vertices.size()>0){
            for(int i= 0; i<dq_valuesY.size()-1; i++){
                float val = dq_valuesY.at(i);

                //Green: 48, 254, 103  http://hcolor.info/en/30fe67
                if(val <= f_yDownLimit){
                    ofSetColor(48, 254, 103);
                    ofCircle(vertices.at(i), 2);
                }
                //Red: 254, 48, 199
                else if (val >= f_yUpLimit){
                    ofSetColor(254, 48, 199,255);
                    ofCircle(vertices.at(i), 7);
                }
                //Oranje: 48, 96, 254
                else {
                    ofSetColor(48, 96, 254,255);
                    ofCircle(vertices.at(i), 4);
                }
        }

        }
        ofPopStyle();

    }
}
void ofx2dPlot::setValueLimits(float downLimit, float upLimit){
    f_yDownLimit = downLimit;
    f_yUpLimit = upLimit;
    b_useLimits = true;
}
void ofx2dPlot::setUseValueLimits(bool b){
    b_useLimits = b;
}
