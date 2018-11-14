#include "ofApp.h"

#define M_PI 3.141592

/*====================================
class point
======================================*/
void point::setup(double Radius,double Speed, double size)
{
	double radius = ofRandom(0, Radius);
	double theta = ofRandom (0,M_PI);

	circleRadius = size;
	globalRadius = Radius, 

	position.x = ofGetWidth() / 2 + radius*cos(theta);
	position.y = ofGetHeight() / 2 + radius*sin(theta);

	speed = Speed;
	double rad = -theta + M_PI / 2 - ofRandom(-70, 70) / 180 * M_PI;
	vel.x = speed*sin(rad);
	vel.y = speed*cos(rad);
}
void point::update()
{
	position -= vel;
	ofVec2f pos = position - ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);
	if (pos.length() > globalRadius)
	{
		vel = speed*pos.getNormalized().rotate(ofRandom(-45,45));
	}
}
void point::draw()
{
	ofFill();
	ofSetColor(255, 0, 0);
	ofDrawCircle(position, circleRadius);
}

/*====================================
class circle
======================================*/
void circle::setup(vector <point>Elements)
{
	//外接円のパラメータの計算
	elements = Elements;
	double c = 2 * ((elements[1].position.x - elements[0].position.x)*(elements[2].position.y - elements[0].position.y) - (elements[1].position.y - elements[0].position.y)*(elements[2].position.x - elements[0].position.x));
	double x1, x2, y1, y2, xy1, xy2;
	x1 = (elements[2].position.y - elements[0].position.y);
	x2 = (elements[0].position.y - elements[1].position.y);
	y1 = (elements[0].position.x - elements[2].position.x);
	y2 = (elements[1].position.x - elements[0].position.x);
	xy1 = (pow(elements[1].position.x, 2) - pow(elements[0].position.x, 2) + pow(elements[1].position.y, 2) - pow(elements[0].position.y, 2));
	xy2 = (pow(elements[2].position.x, 2) - pow(elements[0].position.x, 2) + pow(elements[2].position.y, 2) - pow(elements[0].position.y, 2));
	center.x = (x1*xy1 + x2*xy2) / c;
	center.y = (y1*xy1 + y2*xy2) / c;
	radius = ofVec2f(center - elements[0].position).length();

	shareside[0] = 0;
	shareside[1] = 0;
	shareside[2] = 0;
}

//円の中に他のpointが入っていないかを確認
bool circle::check(vector <point>points)
{
	bool result = true;
	for (int i = 0; i < points.size(); i++)
	{
		float length = ofVec2f(center - points[i].position).length();
		if (length< radius - 0.5)
		{
			result = false;
		}
	}
	return result;
}
void circle::drawCircle()
{
	ofSetLineWidth(0.3);
	ofNoFill();
	ofSetColor(0, 0, 0);
	ofDrawCircle(center, radius);
}
void circle::drawCenter()
{
	ofFill();
	ofSetColor(0, 0, 255);
	ofDrawCircle(center, 6);
}
void circle::drawTriangle()
{
	ofSetLineWidth(0.2);
	ofSetColor(255, 0, 0);

	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	mesh.addVertex(ofPoint(elements[0].position));
	mesh.addVertex(ofPoint(elements[1].position));
	mesh.addVertex(ofPoint(elements[2].position));
	mesh.addVertex(ofPoint(elements[0].position));
	mesh.drawWireframe();
	//mesh.draw();
}

/*====================================
class voronoi
======================================*/
void voronoi::draw()
{
	ofSetLineWidth(1);
	double d = 240;
	double Ax, Ay, Bx, By;
	ofFill();
	if ((point[0] - ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)).length() - d<0 && (point[1] - ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)).length() - d<0)
	{
		ofDrawLine(point[0], point[1]);
	}
	else
	{
		Ax = point[1].x - ofGetWidth() / 2;
		Ay = point[1].y - ofGetHeight() / 2;
		Bx = point[0].x - point[1].x;
		By = point[0].y - point[1].y;
		double t1, t2;
		t1 = (-(Ax*Bx + Ay*By) + sqrt(pow((Ax*Bx + Ay*By), 2) - (Bx*Bx + By*By)*(Ax*Ax + Ay*Ay - d*d))) / (Bx*Bx + By*By);
		t2 = (-(Ax*Bx + Ay*By) - sqrt(pow((Ax*Bx + Ay*By), 2) - (Bx*Bx + By*By)*(Ax*Ax + Ay*Ay - d*d))) / (Bx*Bx + By*By);
		if (t1 > 0 && t1 < 1)
		{
			if(((point[0] - ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)).length() - d)>((point[1] - ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)).length() - d))
			{
				ofSetColor(0, 0, 255);
				ofDrawLine(point[1], point[1] + t1*(point[0] - point[1]));
				ofSetColor(0, 255, 0);
				ofDrawCircle(point[1] + t1*(point[0] - point[1]), 6);
			}
			else
			{
				ofSetColor(0, 0, 255);
				ofDrawLine(point[0], point[1] + t1*(point[0] - point[1]));
				ofSetColor(0, 255, 0);
				ofDrawCircle(point[1] + t1*(point[0] - point[1]), 6);
			}
			
		}
		else if (t2 > 0 && t2 < 1)
		{
			if (((point[0] - ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)).length() - d)>((point[1] - ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)).length() - d))
			{
				ofSetColor(0, 0, 255);
				ofDrawLine(point[1], point[1] + t2*(point[0] - point[1]));
				ofSetColor(0, 255, 0);
				ofDrawCircle(point[1] + t2*(point[0] - point[1]), 6);
			}
			else
			{
				ofSetColor(0, 0, 255);
				ofDrawLine(point[0], point[1] + t2*(point[0] - point[1]));
				ofSetColor(0, 255, 0);
				ofDrawCircle(point[1] + t2*(point[0] - point[1]), 6);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetCircleResolution(36);
	ofNoFill();
	ofSetFrameRate(30);
	ofBackground(255);
	
	for (int i = 0; i < POINT_NUM; i++)
	{
		point Point;
		double rad = ofRandom(0, 360) / 180 * M_PI;
		Point.setup(GLOBAL_RADIUS,POINT_SPEED,POINT_RADIUS);
		points.push_back(Point);
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
	for (int i = 0; i<points.size(); i++)
	{
		points[i].update();
	}
	if (points.size() > 2)
	{
		circles 	= getCircles();
		voronois 	= getVoronois();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	for (int i = 0; i<points.size(); i++)
	{
		points[i].draw();
	}
	for (int i = 0; i<circles.size(); i++)
	{
		circles[i].drawTriangle();
		circles[i].drawCircle();
		circles[i].drawCenter();

		for (int j = 0; j < 3; j++)
		{
			if (circles[i].shareside[j] == 0)
			{
				ofPoint p1, p2;
				p1 = circles[i].center;
				double dig1 = (circles[i].center - circles[i].elements[0].position).angleRad(circles[i].elements[1].position - circles[i].elements[0].position);
				double dig2 = (circles[i].center - circles[i].elements[1].position).angleRad(circles[i].elements[2].position - circles[i].elements[1].position);
				double dig3 = (circles[i].center - circles[i].elements[2].position).angleRad(circles[i].elements[0].position - circles[i].elements[2].position);

				ofVec2f direct = (circles[i].elements[j].position + circles[i].elements[(j+1)%4].position) / 2 - circles[i].center;
				direct = direct.getNormalized();

				if(j == 0)
				{
					if ((dig1 < 0 && dig2 > 0 && dig3 > 0) || (dig1 > 0 && dig2 < 0 && dig3 < 0))
					{
						p2 = circles[i].center - direct * 1000;
					}
					else
					{
						p2 = circles[i].center + direct * 1000;
					}
				}
				else if (j == 1)
				{
					if ((dig1 > 0 && dig2 < 0 && dig3 >0) || (dig1 < 0 && dig2 > 0 && dig3 < 0))
					{
						p2 = circles[i].center - direct * 1000;
					}
					else
					{
						p2 = circles[i].center + direct * 1000;
					}
				}
				else if (j == 2)
				{
					if ((dig1 > 0 && dig2 > 0 && dig3 < 0) || (dig1 < 0 && dig2 < 0 && dig3 > 0))
					{
						p2 = circles[i].center - direct * 10000;
					}
					else
					{
						p2 = circles[i].center + direct * 10000;
					}
				}
				
				ofSetLineWidth(0.5);
				ofDrawLine(p1, p2);
				
				/*if ((p1 - ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)).length() - GLOBAL_RADIUS < 0 && (p2 - ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)).length() - GLOBAL_RADIUS < 0)
				{
					ofDrawLine(p1, p2);
				}
				else
				{
					double Ax, Ay, Bx, By;
					Ax = p2.x - ofGetWidth() / 2;
					Ay = p2.y - ofGetHeight() / 2;
					Bx = p1.x - p2.x;
					By = p1.y - p2.y;
					double t1, t2;
					t1 = (-(Ax*Bx + Ay*By) + sqrt(pow((Ax*Bx + Ay*By), 2) - (Bx*Bx + By*By)*(Ax*Ax + Ay*Ay - GLOBAL_RADIUS*GLOBAL_RADIUS))) / (Bx*Bx + By*By);
					t2 = (-(Ax*Bx + Ay*By) - sqrt(pow((Ax*Bx + Ay*By), 2) - (Bx*Bx + By*By)*(Ax*Ax + Ay*Ay - GLOBAL_RADIUS*GLOBAL_RADIUS))) / (Bx*Bx + By*By);
					if (t1 > 0 && t1 < 1)
					{
						if (((p1 - ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)).length() - GLOBAL_RADIUS) > ((p2 - ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)).length() - GLOBAL_RADIUS))
						{
							ofSetColor(0, 0, 255);
							ofDrawLine(p2, p2 + t1*(p1 - p2));
							ofSetColor(0, 255, 0);
							ofDrawCircle(p2 + t1*(p1 - p2), 6);
						}
						else
						{
							ofSetColor(0, 0, 255);
							ofDrawLine(p1, p2 + t1*(p1 - p2));
							ofSetColor(0, 255, 0);
							ofDrawCircle(p2 + t1*(p1 - p2), 6);
						}
					}
					else if (t2 > 0 && t2 < 1)
					{
						if (((p1 - ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)).length() - GLOBAL_RADIUS) > ((p2 - ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)).length() - GLOBAL_RADIUS))
						{
							ofSetColor(0, 0, 255);
							ofDrawLine(p2, p2 + t2*(p1 - p2));
							ofSetColor(0, 255, 0);
							ofDrawCircle(p2 + t2*(p1 - p2), 6);
						}
						else
						{
							ofSetColor(0, 0, 255);
							ofDrawLine(p1, p2 + t2*(p1 - p2));
							ofSetColor(0, 255, 0);
							ofDrawCircle(p2 + t2*(p1 - p2), 6);
						}
					}
				}*/
			}
		}
	}
	for (int i = 0; i < voronois.size(); i++)
	{
		ofSetColor(0, 0, 255);
		voronois[i].draw();
	}
	ofSetColor(0);
	ofNoFill();
	ofSetLineWidth(1);
	ofDrawCircle(ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2), GLOBAL_RADIUS);
}
vector<circle> ofApp::getCircles()
{
	//外接円の中に他のpointが存在しないpointの組み合わせを全探索
	vector<circle> Circles;
	for (int i = 0; i < points.size() - 2; i++)
	{
		for (int j = i + 1; j < points.size() - 1; j++)
		{
			for (int k = j + 1; k < points.size(); k++)
			{
				vector <point>elements;
				elements.push_back(points[i]);
				elements.push_back(points[j]);
				elements.push_back(points[k]);

				circle Circle;
				Circle.setup(elements);
				//外接円内に他のpointがないか確認
				if (Circle.check(points))
				{
					Circles.push_back(Circle);
				}
			}
		}
	}
	return Circles;
}
vector<voronoi> ofApp::getVoronois()
{
	vector<voronoi> Voronois;
	for (int i = 0; i<circles.size()-1; i++)
	{
		for (int j = i; j < circles.size(); j++)
		{
			int sharepoint=0;
			for (int k = 0; k < 3; k++)
			{
				circles[i].sharepoint[k] = 0;
				circles[j].sharepoint[k] = 0;		
			}

			for (int k = 0; k < 3; k++)
			{
				for (int l = 0; l < 3; l++)
				{
					if (circles[i].elements[k].position == circles[j].elements[l].position)
					{
						circles[i].sharepoint[k]++;
						circles[j].sharepoint[l]++;
						sharepoint++;
					}
				}
			}
			
			if (sharepoint == 2)
			{
				if (circles[i].sharepoint[0] > 0)
				{
					if (circles[i].sharepoint[1] > 0)
					{
						circles[i].shareside[0]++;
					}
					if (circles[i].sharepoint[2] > 0)
					{
						circles[i].shareside[2]++;
					}
				}
				else
				{
					if (circles[i].sharepoint[1] > 0 && circles[i].sharepoint[2] > 0)
					{
						circles[i].shareside[1]++;
					}
				}
				if (circles[j].sharepoint[0] > 0)
				{
					if (circles[j].sharepoint[1] > 0)
					{
						circles[j].shareside[0]++;
					}
					if (circles[j].sharepoint[2] > 0)
					{
						circles[j].shareside[2]++;
					}
				}
				else
				{
					if (circles[j].sharepoint[1] > 0 && circles[j].sharepoint[2] > 0)
					{
						circles[j].shareside[1]++;
					}
				}
				voronoi Voronoi;
				Voronoi.point[0] = circles[i].center;
				Voronoi.point[1] = circles[j].center;
				Voronois.push_back(Voronoi);
			}
		}
	}
	return Voronois;
}