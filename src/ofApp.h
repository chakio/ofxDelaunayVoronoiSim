#pragma once

#include "ofMain.h"

/*====================================
class point
======================================*/
class point
{
private:
	ofVec2f position;
	ofVec2f vel;
	double speed;
	double circleRadius;
	double globalRadius;

public:
	void setup(double radius,double Speed, double size);
	void update();
	void draw();
	ofVec2f getPosition();
	void setRadius(double globalradius);
};

/*====================================
class voronoi
======================================*/
class voronoi
{
private:
	double globalRadius;
	double pointRadius;
	ofVec2f point[2];

public:
	voronoi(double GlobalRadius, double PoiintRadius,ofVec2f start, ofVec2f end);
	void draw();
};

/*====================================
class circle
======================================*/
class circle
{
private:
	float radius;

public:
	void setup(vector<point> elements);
	bool check(vector<point> points);

	void drawCircle();
	void drawTriangle();
	void drawCenter();

	vector <point> elements;
	int shareside[3]; //01:0 12:1 20:2
	int sharepoint[3];
	ofVec2f center;
};

/*====================================
class ofApp
======================================*/
class ofApp : public ofBaseApp
{
	public:
		void setup();
		void update();
		void draw();
		
		vector<circle> getCircles();
		vector<voronoi> getVoronois();
		vector<voronoi> getOutlierVoronois();

		vector<point>points;
		vector<circle>circles;
		vector<voronoi>voronois;

		double dynamic_GLOBAL_RADIUS;

		int GenNum;

		int POINT_NUM 			= 10;
		double GLOBAL_RADIUS 	= 230;
		double POINT_RADIUS 	= 5;
		double POINT_SPEED		= 10;
};