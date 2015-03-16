#pragma once


#include "ofMain.h"
#include <utility>

#define ETC_2DPLOT_MAX_WIDTH 1024
#define ETC_2DPLOT_MAX_HEIGHT 768

class ofx2dPlot {

public:
    ofx2dPlot(int x, int y, int w = ETC_2DPLOT_MAX_WIDTH, int h=ETC_2DPLOT_MAX_HEIGHT,
              int xData = 0, int yData = 0, int wData = ETC_2DPLOT_MAX_WIDTH, int hData = ETC_2DPLOT_MAX_HEIGHT,
              string title = "",string unitsX = "", string unitsY = "", int skipUnits = 2,
              int xNumDivisions = 10,
              float yMin = 0.0, float yMax = 1.0, int yNumDivisions = 10);
    ~ofx2dPlot();

    void setup();
    void update();
    void draw();
    void setDebug(bool b);
    bool getDebug();

    /**************************
        META DATA
    **************************/
    void setTitle(string s);
    void setTitlePos(int x, int y);
    void setUnitsName(string sX, string sY);

    /**************************
        STYLE
    **************************/
    void setColorTitle(ofColor c);
    void setColorMetaData(ofColor c);
    void setColorData(ofColor c);
    void setColorGrid(ofColor c);
    void setColorFrame(ofColor c);
    void setFont(string sFont, int iSize);
    void setFont2(string sFont, int iSize);

    /**************************
        FRAME & GRID
    **************************/
    void setGridSkipUnits(int skipX=1, int skipY=1);
    void setOffset(int xD = 0, int yD = 0);
    void setZoom(float fx = 1.0, float fy = 1.0);
    void setNumDivisions(int xNumDiv = 10, int yNumDiv = 10);
    void setShowAxisValues(bool showAxisValuesX= true, bool showAxisValuesY = true);

    /**************************
        VALUES
    **************************/
//    void addValueXY(ofPoint ptVal);
//    void addValueXY(float x, float y);
    void addValueXY(string x, float y);
    void setFixedMinAndMaxYGridValues(bool fixedMin=false, bool fixedMax= false);
    void setValueLimits(float downLimit, float upLimit);
    void setUseValueLimits(bool b);



private:
    bool b_debug;
    /**************************
        META DATA
    **************************/
    string s_title;
    ofPoint pt_titlePos;
    string s_unitsX, s_unitsY;


    /**************************
        STYLE
    **************************/
    //ofTrueType font;
    ofColor col_title, col_metaData, col_data, col_axis, col_grid, col_frame;
    string s_font;
    int i_fontSize;
    ofTrueTypeFont font;
    int i_fontSize2;
    string s_font2;
    ofTrueTypeFont font2;

    /**************************
        FRAME & GRID
    **************************/
    int i_x, i_y, i_w, i_h;
    int i_xData, i_yData, i_wData, i_hData;
    //This values work inside the main frame
    float f_zoomX, f_zoomY;

    bool b_newGrid;
    int i_pixStepX, i_pixStepY;
    int i_xNumDivisions, i_yNumDivisions;

    int i_skipUnitsX, i_skipUnitsY;

    bool b_showAxisValuesX, b_showAxisValuesY;

    ofVboMesh mesh_grid;
    vector <string> vec_xValues;
    vector <string> vec_yValues;
    void setupGrid();
    void updateGrid();
    void drawGrid();


    /**************************
        VALUES
    **************************/
    float f_yMin, f_yMax, f_yStep;
    bool b_fixedMinGridY, b_fixedMaxGridY;
    float f_yUpLimit, f_yDownLimit;
    bool b_useLimits;


    /**************************
        LINE AND SHAPES
    **************************/
    void setupValues();
    void updateValues();
    void drawValues();

    bool b_newValue;

    //deque <ofPoint> dq_values;
    deque <string> dq_valuesX;
    deque <float> dq_valuesY;
    ofPolyline poly_values;
};
