#include "ofApp.h"

#define M_PI 3.141592

/*====================================
class point
======================================*/
void point::setup(double Radius,double Speed, double size)
{
	double radius = ofRandom(0, Radius);
	double theta = ofRandom (0,2*M_PI);

	circleRadius = size;
	globalRadius = Radius, 

	position.x = ofGetWidth() / 2 + radius*cos(theta);
	position.y = ofGetHeight() / 2 + radius*sin(theta);

	speed = ofRandom(Speed/5,Speed);
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
		vel = speed*pos.getNormalized().rotate(ofRandom(-70,70));
	}
}
void point::draw()
{
	ofFill();
	ofSetColor(255, 0, 0);
	ofDrawCircle(position, circleRadius);
}
ofVec2f point::getPosition()
{
	return position;
}
void point::setRadius(double radius)
{
	globalRadius = radius;
}
/*====================================
class circle
======================================*/
void circle::setup(vector <point>Elements)
{
	//外接円のパラメータの計算
	elements = Elements;
	double c = 2 * ((elements[1].getPosition().x - elements[0].getPosition().x)*(elements[2].getPosition().y - elements[0].getPosition().y) - (elements[1].getPosition().y - elements[0].getPosition().y)*(elements[2].getPosition().x - elements[0].getPosition().x));
	double x1, x2, y1, y2, xy1, xy2;
	x1 = (elements[2].getPosition().y - elements[0].getPosition().y);
	x2 = (elements[0].getPosition().y - elements[1].getPosition().y);
	y1 = (elements[0].getPosition().x - elements[2].getPosition().x);
	y2 = (elements[1].getPosition().x - elements[0].getPosition().x);
	xy1 = (pow(elements[1].getPosition().x, 2) - pow(elements[0].getPosition().x, 2) + pow(elements[1].getPosition().y, 2) - pow(elements[0].getPosition().y, 2));
	xy2 = (pow(elements[2].getPosition().x, 2) - pow(elements[0].getPosition().x, 2) + pow(elements[2].getPosition().y, 2) - pow(elements[0].getPosition().y, 2));
	center.x = (x1*xy1 + x2*xy2) / c;
	center.y = (y1*xy1 + y2*xy2) / c;
	radius = ofVec2f(center - elements[0].getPosition()).length();

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
		float length = ofVec2f(center - points[i].getPosition()).length();
		if (length < radius-0.01)
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
	mesh.addVertex(ofPoint(elements[0].getPosition()));
	mesh.addVertex(ofPoint(elements[1].getPosition()));
	mesh.addVertex(ofPoint(elements[2].getPosition()));
	mesh.addVertex(ofPoint(elements[0].getPosition()));
	mesh.drawWireframe();
	//mesh.draw();
}

/*====================================
class voronoi
======================================*/
voronoi::voronoi(double GLOBAL_RADIUS, double POINT_RADIUS, ofVec2f start ,ofVec2f end)
{
	globalRadius 	= GLOBAL_RADIUS;
	pointRadius		= POINT_RADIUS;
	point[0]		= start;
	point[1]		= end;
}
void voronoi::draw()
{
	ofSetLineWidth(1);
	double Ax, Ay, Bx, By;
	ofPoint center(ofGetWidth() / 2,ofGetHeight() / 2);
	ofFill();
	if ((point[0] - center).length() - globalRadius<0 && (point[1] - center).length() - globalRadius<0)
	{
		ofSetColor(0, 0, 255);
		ofDrawLine(point[0], point[1]);
	}
	else //if((point[0] - center).length() - globalRadius<0 || (point[1] - center).length() - globalRadius<0)
	{
		Ax = point[1].x - ofGetWidth() / 2;
		Ay = point[1].y - ofGetHeight() / 2;
		Bx = point[0].x - point[1].x;
		By = point[0].y - point[1].y;

		double t = 0;
		double t1, t2;
		t1 = (-(Ax*Bx + Ay*By) + sqrt(pow((Ax*Bx + Ay*By), 2) - (Bx*Bx + By*By)*(Ax*Ax + Ay*Ay - globalRadius*globalRadius))) / (Bx*Bx + By*By);
		t2 = (-(Ax*Bx + Ay*By) - sqrt(pow((Ax*Bx + Ay*By), 2) - (Bx*Bx + By*By)*(Ax*Ax + Ay*Ay - globalRadius*globalRadius))) / (Bx*Bx + By*By);
		if (t1 > 0 && t1 < 1)
		{
			t = t1;
		}
		else if (t2 > 0 && t2 < 1)
		{
			t = t2;
		}
		
		ofPoint start,end;
		if(((point[0] - center).length() - globalRadius)>0)
		{
			start=point[1];
		}
		else
		{
			start=point[0];
		}
		end=point[1] + t*(point[0] - point[1]);

		ofSetColor(0, 0, 255);
		ofDrawLine(start, end);

		ofSetColor(0, 255, 0);
		ofDrawCircle(end,pointRadius);
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
	dynamic_GLOBAL_RADIUS = GLOBAL_RADIUS+GLOBAL_RADIUS/4*sin(ofGetElapsedTimef()/2);
	for (int i = 0; i<points.size(); i++)
	{
		points[i].setRadius(dynamic_GLOBAL_RADIUS);
		points[i].update();
	}
	if (points.size() > 2)
	{
		circles 	= getCircles();
		voronois 	= getVoronois();
		vector<voronoi> additionalVoronoi = getOutlierVoronois();
		voronois.insert(voronois.end(),additionalVoronoi.begin(),additionalVoronoi.end());
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	//外周円の描画
	ofSetColor(0);
	ofNoFill();
	ofSetLineWidth(1);
	ofDrawCircle(ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2), dynamic_GLOBAL_RADIUS);

	//動きまわる赤色の点の描画
	for (int i = 0; i<points.size(); i++)
	{
		points[i].draw();
	}

	//ボロノイの描画
	for (int i = 0; i < voronois.size(); i++)
	{
		voronois[i].draw();
	}

	//ドロネーの描画
	for (int i = 0; i<circles.size(); i++)
	{
		circles[i].drawTriangle();
		circles[i].drawCircle();
		circles[i].drawCenter();
	}	
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
		for (int j = i+1; j < circles.size(); j++)
		{
			int sharepoint=0;
			for (int k = 0; k < 3; k++)
			{
				circles[i].sharepoint[k] = 0;
				circles[j].sharepoint[k] = 0;		
			}

			//ドロネーで分割した三角形の中で頂点を共有しているものの洗い出し
			for (int k = 0; k < 3; k++)
			{
				for (int l = 0; l < 3; l++)
				{
					if (circles[i].elements[k].getPosition().match(circles[j].elements[l].getPosition()))
					{
						circles[i].sharepoint[k]++;
						circles[j].sharepoint[l]++;
						sharepoint++;
					}
				}
			}

			//頂点を２つ共有している場合ひとつの辺を共有していることになる
			//辺を共有している場合はボロノイの線で繋がれる
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

				voronoi Voronoi(dynamic_GLOBAL_RADIUS,POINT_RADIUS, circles[i].center,  circles[j].center);
				Voronois.push_back(Voronoi);
			}
		}
	}
	return Voronois;
}

vector<voronoi> ofApp::getOutlierVoronois()
{
	vector<voronoi> Voronois;
	for (int i = 0; i<circles.size(); i++)
	{
		//移動点の外枠の探索
		int share=0;
		for (int j = 0; j < 3; j++)
		{
			if (circles[i].shareside[j] > 0)
			{
				share++;
			}
		}
		if(share<3)
		{
			for (int j = 0; j < 3; j++)
			{
				if (circles[i].shareside[j] == 0)
				{
					ofPoint p1, p2;
					p1 = circles[i].center;
					double dig1 = (circles[i].center - circles[i].elements[0].getPosition()).angleRad(circles[i].elements[1].getPosition() - circles[i].elements[0].getPosition());
					double dig2 = (circles[i].center - circles[i].elements[1].getPosition()).angleRad(circles[i].elements[2].getPosition() - circles[i].elements[1].getPosition());
					double dig3 = (circles[i].center - circles[i].elements[2].getPosition()).angleRad(circles[i].elements[0].getPosition() - circles[i].elements[2].getPosition());

					ofVec2f direct = (circles[i].elements[j].getPosition() + circles[i].elements[(j+1)%3].getPosition()) / 2 - circles[i].center;
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

					voronoi Voronoi(dynamic_GLOBAL_RADIUS,POINT_RADIUS, p1, p2);
					Voronois.push_back(Voronoi);
				}
			}
		}
	}
	return Voronois;
}