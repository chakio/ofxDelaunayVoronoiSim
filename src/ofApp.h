#pragma once

#include "ofMain.h"

/*====================================
class point
======================================*/
class point
{
private:
	ofVec2f vel;
	double speed;
	double circleRadius;
	double globalRadius;

public:
	glm::vec2 position;
	void setup(double radius,double Speed, double size);
	void update();
	void draw();
};

/*====================================
class voronoi
======================================*/
class voronoi
{
private:
public:
	ofVec2f point[2];
	void draw();
};

/*====================================
class circle
======================================*/
class circle
{
private:

public:
	vector <point> elements;
	int  shareside[3]; //01:0 12:1 20:2
	int sharepoint[3];
	ofVec2f center;
	float radius;
	void setup(vector<point> elements);
	bool check(vector<point> points);
	void drawCircle();
	void drawTriangle();
	void drawCenter();
};

class ofApp : public ofBaseApp
{
	public:
		void setup();
		void update();
		void draw();
		vector<circle> getCircles();
		vector<voronoi> getVoronois();
		vector<point>points;
		vector<circle>circles;
		vector<voronoi>voronois;

		int GenNum;

		int POINT_NUM = 10;
		double GLOBAL_RADIUS = 240;
		double POINT_RADIUS = 5;
		double POINT_SPEED	= 0.01;
};
