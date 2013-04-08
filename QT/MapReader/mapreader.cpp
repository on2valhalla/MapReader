

#include "mapreader.h"
#include "ui_mapreader.h"

MapReader::MapReader(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MapReader)
{
	ui->setupUi(this);
	ui->lblImage->setParent(this);

	campusImage = imread("../../../../../ass3-campus.pgm", CV_LOAD_IMAGE_GRAYSCALE);
	campusLabeled = imread("../../../../../ass3-labeled.pgm", CV_LOAD_IMAGE_GRAYSCALE);

	getNames("../../../../../ass3-table.txt");


	// namedWindow("Map");
	// imshow("Map", campusImage);

	// cout << campusLabeled.rows << " " << campusLabeled.cols << " "
	// 	 << (int)campusLabeled.at<uchar>(40,7) << endl;
	Mat wRects(campusImage.size(), CV_8UC1, Scalar(0));

	processFeatures(campusLabeled, wRects);

	describeBuildings();

	findRelations();
	
	// for(uchar i = 0; i < buildings.size() ; i++)
	// 	for( uchar j = 0; j < buildings.size(); j++)
			// cout << buildings[i] << endl;
	// 		if(near( buildings[j], buildings[i]) )
	// 			cout << (int) buildings[i].number << ":  " << buildings[i].name 
	// 				<< " is near to " << buildings[j].name << endl;

	printBinaryPairs();

	cvtColor(campusImage, campusImage, CV_GRAY2BGR);
	displayMat(campusImage);
}

MapReader::~MapReader()
{
	delete ui;
}

void MapReader::getNames(string fileName)
{
	ifstream iFile;
	iFile.open(fileName.c_str());
	string line;

	while(getline(iFile, line))
   {
		size_t eq = line.find_first_of("=");
		size_t f = line.find_first_of("\"", eq+1) + 1;
		size_t e = line.find_first_of("\"", f+1);

		Building b;
		b.number = atoi(line.substr(0,eq).c_str());
		b.name = line.substr(f, e-f);
		buildings.push_back(b);
	   // cout << (int) b.number << " " << b.name <<  endl;
	}
}

void MapReader::processFeatures(const Mat& labeled, Mat& wRects)
{
	// find contours in the filtered image
	vector< vector<Point> > contours;
	vector< Vec4i> hierarchy;

	cv::findContours(labeled.clone(),
					contours, // a vector of contours
					hierarchy, // a hierarchy of contours if there are parent
								//child relations in the image
					CV_RETR_EXTERNAL, // retrieve the external contours
					CV_CHAIN_APPROX_SIMPLE); // an approximation algorithm

	// iterate through all the top-level contours
	for(char idx = 0 ; idx >= 0; idx = hierarchy[idx][0] )
	{
		char number = labeled.at<uchar>(contours[idx][0]);
		Building (*b) = &buildings[number - 1];
		(*b).contour = contours[idx];
		(*b).MBR = boundingRect(contours[idx]);
		(*b).mom = cv::moments(cv::Mat(contours[idx]));
		(*b).centerOfMass = cv::Point((*b).mom.m10/(*b).mom.m00,
									(*b).mom.m01/(*b).mom.m00);

	   // cout << (int) (*b).number << endl;

		rectangle(wRects, (*b).MBR, cv::Scalar(128), 2);
		drawContours(wRects, contours, idx, Scalar(255));
		circle( wRects, (*b).centerOfMass, 3, Scalar(128), 1 );
		circle( wRects, Point( (*b).MBR.x + (*b).MBR.width/2, (*b).MBR.y + (*b).MBR.height/2 ),
					3, Scalar(255), 1 );


	   // wRects += labeled;
	}

}

void MapReader::describeBuildings()
{
	// Compute ranges/averages for size
	minArea = campusImage.rows * campusImage.cols;
	maxArea =0;
	int totalArea =0;
	for( uchar i = 0; i < buildings.size(); i++)
	{
		Building b = buildings[i];
		totalArea += b.mom.m00;
		if(b.mom.m00 < minArea)
			minArea = b.mom.m00;
		if(b.mom.m00 > maxArea)
			maxArea = b.mom.m00;
	}
	avgArea = totalArea / buildings.size();
	areaRange = maxArea - minArea;


	for( uchar i = 0; i < buildings.size(); i++)
	{
		Building &b = buildings[i];
		b.description = "The ";
		int centerx = b.MBR.x + b.MBR.width/2, centery = b.MBR.y + b.MBR.height/2;

		//FIND SHAPE DESCRIPTORS
		if( abs(b.centerOfMass.x - (b.MBR.x + b.MBR.width/2)) + abs(b.centerOfMass.y - (b.MBR.y + b.MBR.height/2))
				>= campusImage.cols * .025 )
		{
			//Center of mass is different from center of bounding rectangle
			if( campusImage.at<uchar>( b.MBR.y + b.MBR.height/2, b.MBR.x + b.MBR.width/2 ) != 0
					|| campusImage.at<uchar>(b.centerOfMass) != 0)
				b.description += "lumpy, ";
			else
			{
			   //differ between u-shaped and l-shaped
				bool north = false, south = false, east = false, west = false ;
				for(int i = 1; i < b.MBR.height/2 - 1; i++)
				{
					if( (north && south) || (east && west) )
						break;

					if(!north)
						if (campusImage.at<uchar>(centery - i, centerx ) != 0 )
							north = true;
					if(!south)
						if (campusImage.at<uchar>(centery + i, centerx ) != 0 )
							south = true;
					if(!east)
						if (campusImage.at<uchar>(centery, centerx + i ) != 0 )
							east = true;
					if(!west)
						if (campusImage.at<uchar>(centery, centerx - i ) != 0 )
							west = true;
				}

				if ((north && south) || (east && west))
					b.description += "U-shaped, ";
				else
					b.description += "L-shaped, ";

			}
		}
		else if(b.MBR.width > b.MBR.height * 3 || b.MBR.height > b.MBR.width * 3 )
			b.description += "narrow, ";
		else if( (campusImage.at<uchar>(centery, b.MBR.x + 1) == 0
							&& campusImage.at<uchar>(centery, b.MBR.x + b.MBR.width - 1) == 0)
					||(campusImage.at<uchar>(b.MBR.y + 1, centerx) == 0
							&& campusImage.at<uchar>(b.MBR.y + b.MBR.height - 1, centerx) == 0) )
			b.description += "H-shaped, ";
		else if( campusImage.at<uchar>(b.MBR.y + 2, b.MBR.x + 2) == 0
					&& campusImage.at<uchar>(b.MBR.y + 2, b.MBR.x + b.MBR.width - 2) == 0
					&& campusImage.at<uchar>(b.MBR.y + b.MBR.height - 2, b.MBR.x + 2) == 0
					&& campusImage.at<uchar>(b.MBR.y + b.MBR.height - 2, b.MBR.x + b.MBR.width - 2) == 0 )
			b.description += "cross-shaped, ";
		else if( b.mom.m00 >= b.MBR.width * b.MBR.height * .85)
		{
			if( abs(b.MBR.width - b.MBR.height) < .3 * b.MBR.width )
				b.description += "squarish, ";
			else
				b.description += "rectangular, ";
		}
	   // else
	   //     cout  << (int) b.number << " "<< b.name << " " << (int)campusImage.at<uchar>(centery, b.MBR.x + 1) << " "
	   //      << (int)campusImage.at<uchar>(centery, b.MBR.x + b.MBR.width - 1) << " "
	   //      << (int)campusImage.at<uchar>(b.MBR.y + 1, centerx) << " "
	   //       <<  (int)campusImage.at<uchar>(b.MBR.y + b.MBR.height - 1, centerx) << endl;

	   // cout  << (int) b.number << " "<< b.name << " " <<  b.description << endl;


		// FIND SIZE DESCRIPTORS
		if( b.mom.m00 > avgArea + areaRange / 6)
			b.description += "large, ";
		else if( b.mom.m00 < avgArea - areaRange / 4)
			b.description += "small, ";
		else
			b.description += "medium sized, ";


		//ORIENTATION descriptors
		if (b.MBR.width > b.MBR.height * 1.5)
			b.description += "east to west oriented, ";
		else if ( b.MBR.height > b.MBR.width * 1.5)
			b.description += "north to south oriented, ";

	   // LOCATION descriptors
		string ending = "erly ";
		if (b.centerOfMass.y < campusImage.rows / 3)
		{
			b.description += "north";
			if(b.centerOfMass.x < campusImage.cols / 3)
				b.description += "-west";
			if(b.centerOfMass.x > campusImage.cols - campusImage.cols / 3)
				b.description += "-east";
		}
		else if (b.centerOfMass.y > campusImage.rows - campusImage.rows / 3)
		{
			b.description += "south";
			if(b.centerOfMass.x < campusImage.cols / 3)
				b.description += "-west";
			if(b.centerOfMass.x > campusImage.cols - campusImage.cols / 3)
				b.description += "-east";
		}
		else if(b.centerOfMass.x < campusImage.cols / 3)
			b.description += "west";
	   else  if(b.centerOfMass.x > campusImage.cols - campusImage.cols / 3)
			b.description += "east";
		else
		{
			ending = " ";
			b.description += "centered";
		}


		b.description += ending + "structure";

	   // cout  << (int) b.number << " "<< b.name << "\n  " <<  b.description << endl << endl;
	}

}


//Convert cv::Mat to QImage and display
void MapReader::displayMat(const cv::Mat& image)
{
	//BGR openCV Mat to QImage
	QImage img_qt = QImage((const unsigned char*)image.data,image.cols, image.rows, image.step, QImage::Format_RGB888);

	//For Binary Images
	if (img_qt.isNull()){

		//ColorTable for Binary Images
		QVector<QRgb> colorTable;
		for (int i = 0; i < 256; i++)
			colorTable.push_back(qRgb(i, i, i));

		img_qt = QImage((const unsigned char*)image.data,image.cols, image.rows, image.step, QImage::Format_Indexed8);
		img_qt.setColorTable(colorTable);
		}

	//Display the QImage in the Label
	QPixmap img_pix = QPixmap::fromImage(img_qt.rgbSwapped()); //BGR to RGB
	this->ui->lblImage->setPixmap(img_pix.scaled(ui->lblImage->size(), Qt::KeepAspectRatio));
}

bool MapReader::north(const Building &s, const Building &g)
{
	if((g.MBR.width > campusImage.cols *.9 || s.MBR.width > campusImage.cols * .9)
			&& g.MBR.y + g.MBR.height < s.MBR.y)
		return true;
	if(g.MBR.y + g.MBR.height * .80 < s.MBR.y
				&& (abs(g.centerOfMass.x - s.centerOfMass.x) * .5 < abs(g.centerOfMass.y - s.centerOfMass.y)
					|| abs(g.centerOfMass.x - s.MBR.x) * .5 < abs(g.centerOfMass.y - s.centerOfMass.y)
					|| abs(g.centerOfMass.x - (s.MBR.x + s.MBR.width)) * .5 < abs(g.centerOfMass.y - s.centerOfMass.y)))
			return true;
	else
		return false;
}
bool MapReader::south(const Building &s, const Building &g)
{
	if((g.MBR.width > campusImage.cols *.9 || s.MBR.width > campusImage.cols * .9)
			&& g.MBR.y > s.MBR.y + s.MBR.height)
		return true;
	if(g.MBR.y  > s.MBR.y + s.MBR.height * .80
				&& (abs(g.centerOfMass.x - s.centerOfMass.x) * .5 < abs(g.centerOfMass.y - s.centerOfMass.y)
					|| abs(g.centerOfMass.x - s.MBR.x) * .5 < abs(g.centerOfMass.y - s.centerOfMass.y)
					|| abs(g.centerOfMass.x - (s.MBR.x + s.MBR.width)) * .5 < abs(g.centerOfMass.y - s.centerOfMass.y)))
		return true;
	else
		return false;
}
bool MapReader::east(const Building &s, const Building &g)
{
	if(g.MBR.width > campusImage.cols *.9 || s.MBR.width > campusImage.cols * .9)
		return false;
	if(g.MBR.x  > s.MBR.x + s.MBR.width * .80
				&& (abs(g.centerOfMass.y - s.centerOfMass.y) < abs(g.centerOfMass.x - s.centerOfMass.x)
					|| abs(g.centerOfMass.y - s.centerOfMass.y) < abs(g.centerOfMass.x - s.MBR.x)
					|| abs(g.centerOfMass.y - s.centerOfMass.y) < abs(g.centerOfMass.x - (s.MBR.x + s.MBR.width))))
		return true;
	else
		return false;
}
bool MapReader::west(const Building &s, const Building &g)
{
	if(g.MBR.width > campusImage.cols *.9 || s.MBR.width > campusImage.cols * .9)
		return false;
	if(g.MBR.x + g.MBR.width * .80 < s.MBR.x 
				&& (abs(g.centerOfMass.y - s.centerOfMass.y) < abs(g.centerOfMass.x - s.centerOfMass.x)
					|| abs(g.centerOfMass.y - s.centerOfMass.y) < abs(g.centerOfMass.x - s.MBR.x)
					|| abs(g.centerOfMass.y - s.centerOfMass.y) < abs(g.centerOfMass.x - (s.MBR.x + s.MBR.width))))
		return true;
	else
		return false;
}
bool MapReader::near(const Building &s, const Building &g)
{
	if(s.MBR == g.MBR)
		return false;

	// adjust for size of structure
	int defaultDistance = (campusImage.cols + campusImage.rows) / 40;
	int adjustment = (s.mom.m00/avgArea) * 10;
	int nearDistance =  adjustment + defaultDistance;
	// cout << s.name << " " << nearDistance << endl;

	// accept if the MBRs overlap
	if((s.MBR & g.MBR).area() > 0)
		return true;


	// double nearness = 1.4;
	// int nearSWidth = s.MBR.width *nearness, 
	// 	nearSHeight = s.MBR.height *nearness,
	// 	newSX = s.MBR.x - (nearSWidth - s.MBR.width)/2,
	// 	newSY = s.MBR.y - (nearSHeight - s.MBR.height)/2,
	// 	nearGWidth = g.MBR.width *nearness, 
	// 	nearGHeight = g.MBR.height *nearness,
	// 	newGX = g.MBR.x - (nearGWidth - g.MBR.width)/2,
	// 	newGY = g.MBR.y - (nearGHeight - g.MBR.height)/2;

	

	// Rect nearS(newSX, newSY, nearSWidth, nearSHeight);
	// Rect nearG(newGX, newGY, nearGWidth, nearGHeight);

	// // accept if the MBRs overlap
	// if((nearS & nearG).area() > 0)
	// 	return true;



	if( (g.MBR.width > campusImage.cols *.9 || s.MBR.width > campusImage.cols * .9) 
		&& (abs(g.centerOfMass.y - s.centerOfMass.y) < nearDistance
			|| abs(g.MBR.y - s.centerOfMass.y) < nearDistance
			|| abs(g.centerOfMass.y - s.MBR.y) < nearDistance
			|| abs(g.MBR.y - s.MBR.y) < nearDistance ) )
		return true;

	if(abs(g.centerOfMass.y - s.centerOfMass.y) 
		+ abs(g.centerOfMass.x - s.centerOfMass.x) > 8 * defaultDistance)
		return false;

	for(uchar i = 0; i < s.contour.size(); i++)
	{
		Point sP = s.contour[i];
		for(uchar j = 0; j < g.contour.size(); j++)
		{
			Point gP = g.contour[j];
			if(abs(sP.x - gP.x) + abs(sP.y - gP.y) < nearDistance )
				return true;
		}
	}

	return false;
}



void MapReader::findRelations()
{
	northR = vector< vector< bool > >();
	southR = vector< vector< bool > >();
	eastR = vector< vector< bool > >();
	westR = vector< vector< bool > >();
	nearR = vector< vector< bool > >();


	//FIND ALL RELATIONS
	for(uchar i = 0; i < buildings.size(); i++)
	{
		northR.push_back( vector< bool >() );
		southR.push_back( vector< bool >() );
		eastR.push_back( vector< bool >() );
		westR.push_back( vector< bool >() );
		nearR.push_back( vector< bool >() );
		for(uchar j = 0; j < buildings.size(); j++)
		{
			northR[i].push_back( north( buildings[i], buildings[j] ));
			southR[i].push_back( south( buildings[i], buildings[j] ));
			eastR[i].push_back( east( buildings[i], buildings[j] ));
			westR[i].push_back( west( buildings[i], buildings[j] ));
			nearR[i].push_back( near( buildings[i], buildings[j] ));
		}
	}



	// cout << "\n\n\nCOMPLETE\n";
	// cout << "north\n";
	// printRelations(northR);
	// cout << "south\n";
	// printRelations(southR);
	// cout << "east\n";
	// printRelations(eastR);
	// cout << "west\n";
	// printRelations(westR);
	// cout << "near\n";
	// printRelations(nearR);


	// eliminate duplicate compass relations via transitivity
	for(uchar i = 0; i < buildings.size(); i++)
		for(uchar j = 0; j < buildings.size(); j++)
		{
			if(i == j)
			{
				northR[i][j] = false;
				southR[i][j] = false;
				eastR[i][j] = false;
				westR[i][j] = false;
				nearR[i][j] = false;
				continue;
			}

			if( northR[i][j] )
				for( uchar k = 0; k < buildings.size(); k++ )
					if( northR[j][k] )
						northR[i][k] = false;
			if( southR[i][j] )
				for( uchar k = 0; k < buildings.size(); k++ )
					if( southR[j][k] )
						southR[i][k] = false;
			if( eastR[i][j] )
				for( uchar k = 0; k < buildings.size(); k++ )
					if( eastR[j][k] )
						eastR[i][k] = false;
			if( westR[i][j] )
				for( uchar k = 0; k < buildings.size(); k++ )
					if( westR[j][k] )
						westR[i][k] = false;
		}

	// eliminate further duplicates from near relations
	for(uchar i = 0; i < buildings.size(); i++)
		for(uchar j = 0; j < buildings.size(); j++)
			if( nearR[i][j] )
				for( uchar k = 0; k < buildings.size(); k++ )
				{
					if( northR[i][k] && northR[j][k] )
					{
						if( buildings[i].mom.m00 < buildings[j].mom.m00 )
							northR[i][k] = false;
						else
							northR[j][k] = false;
					}
					if( southR[i][k] && southR[j][k] )
					{
						if( buildings[i].mom.m00 < buildings[j].mom.m00 )
							southR[i][k] = false;
						else
							southR[j][k] = false;
					}
					if( eastR[i][k] && eastR[j][k] )
					{
						if( buildings[i].mom.m00 < buildings[j].mom.m00 )
							eastR[i][k] = false;
						else
							eastR[j][k] = false;
					}
					if( westR[i][k] && westR[j][k] )
					{
						if( buildings[i].mom.m00 < buildings[j].mom.m00 )
							westR[i][k] = false;
						else
							westR[j][k] = false;
					}
				}

	// cout << "\n\n\nMINIMIZED\n";
	// cout << "north\n";
	// printRelations(northR);
	// cout << "south\n";
	// printRelations(southR);
	// cout << "east\n";
	// printRelations(eastR);
	// cout << "west\n";
	// printRelations(westR);
	// cout << "near\n";
	// printRelations(nearR);
}


void MapReader::printRelations(const vector< vector< bool > > &rel)
{
	// COMPLETE RELATIONS
	cout << "       ";

	for(uchar i = 0; i < buildings.size() ; i++)
	{
		if(i < 9)
			cout << " " << i+1 << " ";
		else
			cout << " " << i+1;
	}

	cout << endl;

	for(uchar i = 0; i < buildings.size() ; i++)
	{
		if(i < 9)
			cout << " " << i+1 << " " ;
		else
			cout << i+1 << " " ;
		cout << buildings[i].name.substr(0,4);
		for( uchar j = 0; j < buildings.size(); j++)
		{
			if (rel[i][j])
				cout << " " << rel[i][j] << " ";
			else
				cout << "   ";
		}
		cout << endl;
	}
	cout << endl;
}


void MapReader::printBinaryPairs()
{
	int relCount = 1;
	for(uchar i = 0; i < buildings.size(); i++)
	{
		cout << (int)buildings[i].number << ":\n";
		if(std::find(northR[i].begin(), northR[i].end(), true) != northR[i].end())
		{
			cout << "North of " << buildings[i].name << " is:\n";
			for(uchar j = 0; j <buildings.size(); j++)
				if( northR[i][j] )
					cout << "\t" << buildings[j].name << "\t" << relCount++ << endl;
		}
		if(std::find(southR[i].begin(), southR[i].end(), true) != southR[i].end())
		{
			cout << "South of " << buildings[i].name << " is:\n";
			for(uchar j = 0; j <buildings.size(); j++)
				if( southR[i][j] )
					cout << "\t" << buildings[j].name << "\t" << relCount++ << endl;
		}
		if(std::find(eastR[i].begin(), eastR[i].end(), true) != eastR[i].end())
		{
			cout << "East of " << buildings[i].name << " is:\n";
			for(uchar j = 0; j <buildings.size(); j++)
				if( eastR[i][j] )
					cout << "\t" << buildings[j].name << "\t" << relCount++ << endl;
		}
		if(std::find(westR[i].begin(), westR[i].end(), true) != westR[i].end())
		{
			cout << "West of " << buildings[i].name << " is:\n";
			for(uchar j = 0; j <buildings.size(); j++)
				if( westR[i][j] )
					cout << "\t" << buildings[j].name << "\t" << relCount++ << endl;
		}
		if(std::find(nearR[i].begin(), nearR[i].end(), true) != nearR[i].end())
		{
			cout << "Near to " << buildings[i].name << " is:\n";
			for(uchar j = 0; j <buildings.size(); j++)
				if( nearR[i][j] )
					cout << "\t" << buildings[j].name << "\t" << relCount++ << endl;
		}
		cout << endl << endl;
	}
}




// void printFeatures(const vector< vector< int > > &features)
// {
//     QDebug debugMessage(QtDebugMsg);
//     for(int i = 0; i < features.size(); i++)
//     {
//         debugMessage << i << ":\n";
//         for(int j = 0; j < features[i].size(); j++)
//         {
//             debugMessage << "\t" << features[i][j];
//         }
//         debugMessage << "\n";
//     }
// }


vector<Point> MapReader::findCloud(const Point &pt, 
				const vector<unordered_set<int> > &features)
{
	vector<Point> cloud;
	queue<Point> children;
	unordered_set<Point, hash<Point> > seen;
	Point curPoint;
	int count = 1;


	children.push(pt);

	while(!children.empty())
	{
		curPoint = children.front();
		children.pop();
        if(seen.find(curPoint) != seen.end())
			continue;

		cloud.push_back(curPoint);
		seen.insert(curPoint);
		bool brk = false;

		Building buildPt(curPoint, avgArea);

        // printFeatures(features);
        bool feature, contained;

		for(uchar i = 0; i < buildings.size(); i++)
		{
			if(features[0].size() > 0)
			{
				feature = near( buildings[i], buildPt );
				contained = features[0].find(i+1) != features[0].end();
				if( (feature && !contained) || (!feature && contained) )
				{
					cloud.pop_back();
					brk = true;
					break;
				}
			}
				
			if(features[1].size() > 0)
			{
				feature = north( buildings[i], buildPt );
				contained = features[1].find(i+1) != features[1].end();
				if( (feature && !contained) || (!feature && contained) )
				{
					cloud.pop_back();
					brk = true;
					break;
				}
			}

			if(features[2].size() > 0)
			{
				feature = south( buildings[i], buildPt );
				contained = features[2].find(i+1) != features[2].end();
				if( (feature && !contained) || (!feature && contained) )
				{
					cloud.pop_back();
					brk = true;
					break;
				}
			}
			
			if(features[3].size() > 0)
			{	
				feature = east( buildings[i], buildPt );
				contained = features[3].find(i+1) != features[3].end();
				if( (feature && !contained) || (!feature && contained) )
				{
					cloud.pop_back();
					brk = true;
					break;
				}
			}
				
			if(features[4].size() > 0)
			{
				feature = west( buildings[i], buildPt );
				contained = features[4].find(i+1) != features[4].end();
				if( (feature && !contained) || (!feature && contained) )
				{
					cloud.pop_back();
					brk = true;
					break;
				}
			}
		}

		if(brk)
			continue;

		// Get Children
		Point n = Point(curPoint.x, curPoint.y-1),
			s = Point(curPoint.x, curPoint.y+1),
			e = Point(curPoint.x+1, curPoint.y),
            w = Point(curPoint.x-1, curPoint.y);
        if(seen.find(w) == seen.end() && curPoint.x != 0)
			children.push(w);
        if(seen.find(e) == seen.end() && curPoint.x != campusImage.cols)
            children.push(e);
        if(seen.find(n) == seen.end() && curPoint.y != 0)
			children.push(n);
        if(seen.find(s) == seen.end() && curPoint.y != campusImage.rows)
			children.push(s);

		// qDebug() << count++;

	}

	return cloud;
}


void MapReader::clicked(QMouseEvent *e)
{
	// cout << e->x() << "," << e->y() << endl;
	Point pt(e->x(), e->y());
    Building buildPt(pt, minArea);

	unordered_set<int> nearLoc, northLoc, southLoc, eastLoc, westLoc;


	// qDebug() << "here";
	// for(uchar i = 0; i < buildings.size(); i++)
	// {
	// 	// qDebug() <<(Building) buildings[i] << endl;
	// 	if( near( buildings[i], buildPt ) )
	// 		qDebug() << "Near " << buildings[i].name.c_str();
	// }
	// qDebug()  << "   ---";
	
	for(uchar i = 0; i < buildings.size(); i++)
	{
		if( near( buildings[i], buildPt ) )
			nearLoc.insert(i+1);
		if( north( buildings[i], buildPt ) )
			northLoc.insert(i+1);
		if( south( buildings[i], buildPt ) )
			southLoc.insert(i+1);
		if( east( buildings[i], buildPt ) )
			eastLoc.insert(i+1);
		if( west( buildings[i], buildPt ) )
			westLoc.insert(i+1);
	}

	vector< unordered_set< int > > features;
	features.push_back(nearLoc);
	features.push_back(northLoc);
	features.push_back(southLoc);
	features.push_back(eastLoc);
	features.push_back(westLoc);

	// printFeatures(features);

     vector<Point> cloud = findCloud(pt, features);

     drawCloud(cloud);
}

void MapReader::drawCloud(vector<Point> cloud)
{
	Mat cloudImage = campusImage.clone();
	for (std::vector<Point>::iterator i = cloud.begin(); i != cloud.end(); ++i)
	{
        if(cloudImage.at<Vec3b>(*i) == Vec3b(255, 255, 255))
			cloudImage.at<Vec3b>(*i) = Vec3b(0,100,0);
		else
			cloudImage.at<Vec3b>(*i) += Vec3b(0,100,0);
		// qDebug() << (*i).x << ", " << (*i).y;
	}
	displayMat(cloudImage);
}









