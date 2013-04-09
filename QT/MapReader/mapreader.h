#ifndef MAPREADER_H
#define MAPREADER_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QDebug>

#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>
#include <unordered_set>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

namespace Ui {
class MapReader;
}

struct Building {
	char number;
	string name;
	Moments mom;
	Point centerOfMass;
	vector<Point> contour;
	unordered_set< int > north;
	unordered_set< int > south;
	unordered_set< int > east;
	unordered_set< int > west;
	unordered_set< int > near;
	Rect MBR;
	string description;

	Building()
	{

	}

	Building(Point pt, int area)
	{
		this->number = -1;
		this->name = "";
		this->contour.push_back(pt);
		this->mom = moments(Mat(contour));
		this->mom.m00 = area;
		this->centerOfMass = pt;
		this->MBR = Rect(pt, Size(1,1));
		this->description = "";
	}

	friend ostream& operator<< (ostream &o, const Building &b)
	{
		o << (int)b.number << " (" << b.name << "): "
		  << "CoM: " << b.centerOfMass << "   Area: " << b.mom.m00
		  << "   Bound UL: [" << b.MBR.x << ", " << b.MBR.y
		  << "]  LR: [" << b.MBR.x + b.MBR.width << ", " << b.MBR.y +b.MBR.height
		  << "]\n  Description:  " << b.description;
		return o;
	}

	friend QDebug operator<< (QDebug q, const Building &b)
	{
		q << (int)b.number << " (" << b.name.c_str() << "): "
		  << "CoM: (" << b.centerOfMass.y << ", " 
		  << b.centerOfMass.x << ")   Area: " << b.mom.m00
		  << "   Bound UL: [" << b.MBR.x << ", " << b.MBR.y
		  << "]  LR: [" << b.MBR.x + b.MBR.width << ", " << b.MBR.y +b.MBR.height
		  << "]\n  Description:  " << b.description.c_str();
		return q;
	}
};



namespace std 
{
	// Hash combination emulates from Boost library
	template<>
	class hash<Point> {
	public:
		size_t operator()(const Point &pt) const
		{
			hash< int > iHash;
			size_t hash = iHash(pt.x);
			hash ^= iHash(pt.y)
			+ 0x9e3779b9 + (hash << 6) + (hash >> 2);
			return hash;
		}
	};
}

class MapReader : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MapReader(QWidget *parent = 0);
	~MapReader();
	
	void displayMat(const cv::Mat& image);
	void getNames(string fileName);

	void processFeatures(const cv::Mat& image, cv::Mat& wRects);
	void describeBuildings();

	void findRelations();
	void printRelations(const vector< vector< bool > > &rel);
	void printRelations();
	void printBinaryPairs();

	vector<Point> findCloud(const Point &pt, const vector<unordered_set<int> > &features);
	void drawCloud(vector<Point> cloud);
	void printFeatures(const Point &pt, const vector< unordered_set< int > > &features);

    void clicked(QMouseEvent *e);

	bool north(const Building &s, const Building &g);
	bool south(const Building &s, const Building &g);
	bool east(const Building &s, const Building &g);
	bool west(const Building &s, const Building &g);
	bool near(const Building &s, const Building &g);

private:
	Ui::MapReader *ui;
	Mat campusImage;
	Mat campusLabeled;
	vector<Building> buildings;
	vector< vector< bool > > northR;
	vector< vector< bool > > southR;
	vector< vector< bool > > eastR;
	vector< vector< bool > > westR;
	vector< vector< bool > > nearR;

	int minArea, maxArea, avgArea, areaRange;
};

#endif // MAPREADER_H
