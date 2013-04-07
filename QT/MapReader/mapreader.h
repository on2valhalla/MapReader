#ifndef MAPREADER_H
#define MAPREADER_H

#include <QMainWindow>

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

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
	Rect MBR;
	string description;

	friend ostream& operator<< (ostream &o, const Building &b)
	{
		o << (int)b.number << " (" << b.name << "): "
		  << "CoM: " << b.centerOfMass << "   Area: " << b.mom.m00
		  << "   Bound UL: [" << b.MBR.x << ", " << b.MBR.y
		  << "]  LR: [" << b.MBR.x + b.MBR.width << ", " << b.MBR.y +b.MBR.height
		  << "]\n  Description:  " << b.description;
		return o;
	}
};

class MapReader : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MapReader(QWidget *parent = 0);
	~MapReader();
	
	void displayMat(const cv::Mat& image);
	void processFeatures(const cv::Mat& image, cv::Mat& wRects);
	void describeBuildings();
	void getNames(string fileName);
	void findRelations();
	void printRelations(vector< vector< bool > > rel);
	void printBinaryPairs();
	vector<Point> findCloud(Point pt);

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
