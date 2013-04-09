

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



	// cout << campusLabeled.rows << " " << campusLabeled.cols << " "
	// 	 << (int)campusLabeled.at<uchar>(40,7) << endl;
	Mat wRects(campusImage.size(), CV_8UC1, Scalar(0));

	processFeatures(campusLabeled, wRects);
	namedWindow("Map");
	imshow("Map", wRects);

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

	ready = true;
	takeinput = false;
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
		b.description = "the ";
		// b.description = "";
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
			b.description += "I-shaped, ";
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
			b.description += "large";
		else if( b.mom.m00 < avgArea - areaRange / 4)
			b.description += "small";
		else
			b.description += "medium sized";


		//ORIENTATION descriptors
		if (b.MBR.width > b.MBR.height * 1.5)
			b.description += ", east to west oriented structure";
		else if ( b.MBR.height > b.MBR.width * 1.5)
			b.description += ", north to south oriented structure";
		else
			b.description += " structure";

	 //   // LOCATION descriptors
		// string ending = "erly ";
		// if (b.centerOfMass.y < campusImage.rows / 3)
		// {
		// 	b.description += "north";
		// 	if(b.centerOfMass.x < campusImage.cols / 3)
		// 		b.description += "-west";
		// 	if(b.centerOfMass.x > campusImage.cols - campusImage.cols / 3)
		// 		b.description += "-east";
		// }
		// else if (b.centerOfMass.y > campusImage.rows - campusImage.rows / 3)
		// {
		// 	b.description += "south";
		// 	if(b.centerOfMass.x < campusImage.cols / 3)
		// 		b.description += "-west";
		// 	if(b.centerOfMass.x > campusImage.cols - campusImage.cols / 3)
		// 		b.description += "-east";
		// }
		// else if(b.centerOfMass.x < campusImage.cols / 3)
		// 	b.description += "west";
	 //   else  if(b.centerOfMass.x > campusImage.cols - campusImage.cols / 3)
		// 	b.description += "east";
		// else
		// {
		// 	ending = " ";
		// 	b.description += "centered";
		// }


		// b.description += ending + "structure";

		cout << b << endl;
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
	if(g.MBR.y + g.MBR.height * .8 < s.MBR.y
				&& (abs(g.centerOfMass.x - s.centerOfMass.x) * 2 < abs(g.centerOfMass.y - s.centerOfMass.y)
					|| abs(g.centerOfMass.x - s.MBR.x) * 2 < abs(g.centerOfMass.y - s.centerOfMass.y)
					|| abs(g.centerOfMass.x - (s.MBR.x + s.MBR.width)) * 2 < abs(g.centerOfMass.y - s.centerOfMass.y)))
			return true;
	else
		return false;
}
bool MapReader::south(const Building &s, const Building &g)
{
	if((g.MBR.width > campusImage.cols *.9 || s.MBR.width > campusImage.cols * .9)
			&& g.MBR.y > s.MBR.y + s.MBR.height)
		return true;
	if(g.MBR.y  > s.MBR.y + s.MBR.height * .8
				&& (abs(g.centerOfMass.x - s.centerOfMass.x) * 2 < abs(g.centerOfMass.y - s.centerOfMass.y)
					|| abs(g.centerOfMass.x - s.MBR.x) * 2 < abs(g.centerOfMass.y - s.centerOfMass.y)
					|| abs(g.centerOfMass.x - (s.MBR.x + s.MBR.width)) * 2 < abs(g.centerOfMass.y - s.centerOfMass.y)))
		return true;
	else
		return false;
}
bool MapReader::east(const Building &s, const Building &g)
{
	if(g.MBR.width > campusImage.cols *.9 || s.MBR.width > campusImage.cols * .9)
		return false;
	if(g.MBR.x  > s.MBR.x + s.MBR.width * .8
				&& (abs(g.centerOfMass.y - s.centerOfMass.y) * 5 < abs(g.centerOfMass.x - s.centerOfMass.x)
					|| abs(g.centerOfMass.y - s.MBR.y) * 5 < abs(g.centerOfMass.x - s.centerOfMass.x)
					|| abs(g.centerOfMass.y - (s.MBR.y + s.MBR.height)) * 5 < abs(g.centerOfMass.x - s.centerOfMass.x)))
		return true;
	else
		return false;
}
bool MapReader::west(const Building &s, const Building &g)
{
	if(g.MBR.width > campusImage.cols *.9 || s.MBR.width > campusImage.cols * .9)
		return false;
	if(g.MBR.x + g.MBR.width * .8 < s.MBR.x 
				&& (abs(g.centerOfMass.y - s.centerOfMass.y) * 5 < abs(g.centerOfMass.x - s.centerOfMass.x)
					|| abs(g.centerOfMass.y - s.MBR.y) * 5 < abs(g.centerOfMass.x - s.centerOfMass.x)
					|| abs(g.centerOfMass.y - (s.MBR.y + s.MBR.height)) * 5 < abs(g.centerOfMass.x - s.centerOfMass.x)))
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

	for(uchar i = 0; i < buildings.size(); i++)
		for(uchar j = 0; j < buildings.size(); j++)
		{
			if( northR[i][j] )
				buildings[i].north.insert(j);
			if( southR[i][j] )
				buildings[i].south.insert(j);
			if( eastR[i][j] )
				buildings[i].east.insert(j);
			if( westR[i][j] )
				buildings[i].west.insert(j);
			if( nearR[i][j] )
				buildings[i].near.insert(j);
		}


	cout << "\n\n\nCOMPLETE\n";
	cout << "north\n";
	printRelations(northR);
	cout << "south\n";
	printRelations(southR);
	cout << "east\n";
	printRelations(eastR);
	cout << "west\n";
	printRelations(westR);
	cout << "near\n";
	printRelations(nearR);


	// eliminate duplicate compass relations via transitivity
	for(uchar i = 0; i < buildings.size(); i++)
		for(uchar j = 0; j < buildings.size(); j++)
		{
			if(i == j)
				continue;

			if( northR[i][j] )
				for( uchar k = 0; k < buildings.size(); k++ )
					if( northR[j][k] )
						buildings[i].north.erase(k);
			if( southR[i][j] )
				for( uchar k = 0; k < buildings.size(); k++ )
					if( southR[j][k] )
						buildings[i].south.erase(k);
			if( eastR[i][j] )
				for( uchar k = 0; k < buildings.size(); k++ )
					if( eastR[j][k] )
						buildings[i].east.erase(k);
			if( westR[i][j] )
				for( uchar k = 0; k < buildings.size(); k++ )
					if( westR[j][k] )
						buildings[i].west.erase(k);
		}

	// // eliminate further duplicates from near relations
	// for(uchar i = 0; i < buildings.size(); i++)
	// 	for(uchar j = 0; j < buildings.size(); j++)
	// 		if( nearR[i][j] )
	// 			for( uchar k = 0; k < buildings.size(); k++ )
	// 			{
	// 				if( buildings[i].north.count(k) > 0 
	// 									&& buildings[j].north.count(k) > 0 )
	// 				{
	// 					if( buildings[i].mom.m00 < buildings[j].mom.m00 )
	// 						buildings[i].north.erase(k);
	// 					else
	// 						buildings[j].north.erase(k);
	// 				}
	// 				if( buildings[i].south.count(k) > 0 
	// 									&& buildings[j].south.count(k) > 0 )
	// 				{
	// 					if( buildings[i].mom.m00 < buildings[j].mom.m00 )
	// 						buildings[i].south.erase(k);
	// 					else
	// 						buildings[j].south.erase(k);
	// 				}
	// 				if( buildings[i].east.count(k) > 0
	// 									&& buildings[j].east.count(k) > 0 )
	// 				{
	// 					if( buildings[i].mom.m00 < buildings[j].mom.m00 )
	// 						buildings[i].east.erase(k);
	// 					else
	// 						buildings[j].east.erase(k);
	// 				}
	// 				if( buildings[i].west.count(k) > 0
	// 									&& buildings[j].west.count(k) > 0 )
	// 				{
	// 					if( buildings[i].mom.m00 < buildings[j].mom.m00 )
	// 						buildings[i].west.erase(k);
	// 					else
	// 						buildings[j].west.erase(k);
	// 				}
	// 			}

	cout << "\n\n\nMINIMIZED\n";
	printRelations();
}


void MapReader::printRelations(const vector< vector< bool > > &rel)
{
	cout << "          ";

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
		cout << buildings[i].name.substr(0,7);
		int buffer = 7 - buildings[i].name.size();
		for (int i = 0; i < buffer; ++i)
			cout << " ";

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


void MapReader::printRelations()
{
	cout << "NORTH\n          ";

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
		cout << buildings[i].name.substr(0,7);
		int buffer = 7 - buildings[i].name.size();
		for (int i = 0; i < buffer; ++i)
			cout << " ";

		for( uchar j = 0; j < buildings.size(); j++)
		{
			if ( buildings[i].north.count(j) > 0 )
				cout << " " << 1 << " ";
			else
				cout << "   ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "SOUTH\n          ";

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
		cout << buildings[i].name.substr(0,7);
		int buffer = 7 - buildings[i].name.size();
		for (int i = 0; i < buffer; ++i)
			cout << " ";

		for( uchar j = 0; j < buildings.size(); j++)
		{
			if ( buildings[i].south.count(j) > 0 )
				cout << " " << 1 << " ";
			else
				cout << "   ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "EAST\n          ";

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
		cout << buildings[i].name.substr(0,7);
		int buffer = 7 - buildings[i].name.size();
		for (int i = 0; i < buffer; ++i)
			cout << " ";

		for( uchar j = 0; j < buildings.size(); j++)
		{
			if ( buildings[i].east.count(j) > 0 )
				cout << " " << 1 << " ";
			else
				cout << "   ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "WEST\n          ";

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
		cout << buildings[i].name.substr(0,7);
		int buffer = 7 - buildings[i].name.size();
		for (int i = 0; i < buffer; ++i)
			cout << " ";

		for( uchar j = 0; j < buildings.size(); j++)
		{
			if ( buildings[i].west.count(j) > 0 )
				cout << " " << 1 << " ";
			else
				cout << "   ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "NEAR\n          ";

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
		cout << buildings[i].name.substr(0,7);
		int buffer = 7 - buildings[i].name.size();
		for (int i = 0; i < buffer; ++i)
			cout << " ";

		for( uchar j = 0; j < buildings.size(); j++)
		{
			if ( buildings[i].near.count(j) > 0 )
				cout << " " << 1 << " ";
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
		if(buildings[i].north.size() > 0 )
		{
			cout << "North of " << buildings[i].name << " is:\n";
			for(uchar j = 0; j <buildings.size(); j++)
				if( buildings[i].north.count(j) > 0 )
					cout << "\t" << buildings[j].name << "\t" << relCount++ << endl;
		}
		if(buildings[i].south.size() > 0 )
		{
			cout << "South of " << buildings[i].name << " is:\n";
			for(uchar j = 0; j <buildings.size(); j++)
				if( buildings[i].south.count(j) > 0 )
					cout << "\t" << buildings[j].name << "\t" << relCount++ << endl;
		}
		if(buildings[i].east.size() > 0 )
		{
			cout << "East of " << buildings[i].name << " is:\n";
			for(uchar j = 0; j <buildings.size(); j++)
				if( buildings[i].east.count(j) > 0 )
					cout << "\t" << buildings[j].name << "\t" << relCount++ << endl;
		}
		if(buildings[i].west.size() > 0 )
		{
			cout << "West of " << buildings[i].name << " is:\n";
			for(uchar j = 0; j <buildings.size(); j++)
				if( buildings[i].west.count(j) > 0 )
					cout << "\t" << buildings[j].name << "\t" << relCount++ << endl;
		}
		if(buildings[i].near.size() > 0 )
		{
			cout << "Near to " << buildings[i].name << " is:\n";
			for(uchar j = 0; j <buildings.size(); j++)
				if( buildings[i].near.count(j) > 0 )
					cout << "\t" << buildings[j].name << "\t" << relCount++ << endl;
		}
		cout << endl << endl;
	}
}







void MapReader::printFeatures(const Building &buildPt)
{
	QTextStream qout(stdout);
	bool last = true;
	string join = "";

	// qout << "(" << buildPt.centerOfMass.x 
	// 	<< ", " << buildPt.centerOfMass.y << ") is ";
	
	for(auto i = buildPt.north.begin(); i != buildPt.north.end(); i++)
		// qout << "(" << buildings[*i].description.c_str() 
		// 	<< ")(called " << buildings[*i].name.c_str() << ") ";
		qout << "(" << buildings[*i].description.c_str() << ")";

	if(buildPt.north.size() > 1)
		qout << join.c_str() << "are NORTH of ";
	else if(buildPt.north.size() == 1)
		qout << join.c_str() << "is NORTH of ";
	else
		last = false;

	if(last)
		join =  "and ";
	last = true;	
	for(auto i = buildPt.south.begin(); i != buildPt.south.end(); i++)
		// qout << "(" << buildings[*i].description.c_str() 
		// 	<< ")(called " << buildings[*i].name.c_str() << "), ";
		qout << "(" << buildings[*i].description.c_str() << ")";

	if(buildPt.south.size() > 1)
		qout << join.c_str() << "are SOUTH of ";
	else if(buildPt.south.size() == 1)
		qout << join.c_str() << "is SOUTH of ";
	else
		last = false;

	if(last)
		join = "and ";
	last = true;	
	for(auto i = buildPt.east.begin(); i != buildPt.east.end(); i++)
		// qout << "(" << buildings[*i].description.c_str() 
		// 	<< ")(called " << buildings[*i].name.c_str() << "), ";
		qout << "(" << buildings[*i].description.c_str() << ")";

	if(buildPt.east.size() > 1)
		qout << join.c_str() << "are EAST of ";
	else if(buildPt.east.size() == 1)
		qout << join.c_str() << "is EAST of ";
	else
		last = false;

	if(last)
		join = "and ";
	last = true;	
	for(auto i = buildPt.west.begin(); i != buildPt.west.end(); i++)
		// qout << "(" << buildings[*i].description.c_str() 
		// 	<< ")(called " << buildings[*i].name.c_str() << "), ";
		qout << "(" << buildings[*i].description.c_str() << ")";

	if(buildPt.west.size() > 1)
		qout << join.c_str() << "are WEST of ";
	else if(buildPt.west.size() == 1)
		qout << join.c_str() << "is WEST of ";
	else
		last = false;

	if(last)
		join = "and ";
	last = true;
	for(auto i = buildPt.near.begin(); i != buildPt.near.end(); i++)
		// qout << "(" << buildings[*i].description.c_str() 
		// 	<< ")(called " << buildings[*i].name.c_str() << "), ";
		qout << "(" << buildings[*i].description.c_str() << ")";

	if(buildPt.near.size() > 1)
		qout << join.c_str() << "are NEAR ";
	else if(buildPt.near.size() == 1)
		qout << join.c_str() << "is NEAR ";
	else
		last = false;
	qout << endl;
}


vector<Point> MapReader::findCloud(const Building &buildPt)
{
	vector<Point> cloud;
	queue<Point> children;
	unordered_set<Point, hash<Point> > seen;
	Point curPoint;
	int count = 1;


	children.push(buildPt.centerOfMass);

	while(!children.empty())
	{
		curPoint = children.front();
		children.pop();

		if(seen.find(curPoint) != seen.end())
			continue;
		seen.insert(curPoint);

		cloud.push_back(curPoint);

		Building curBuild(curPoint, minArea);
		retrMinFeatures(curBuild);

		if( buildPt.north != curBuild.north
			|| buildPt.south != curBuild.south
			|| buildPt.east != curBuild.east
			|| buildPt.west != curBuild.west
			|| buildPt.near != curBuild.near )
		{
			cloud.pop_back();
			continue;
		}


		// Get Children
		Point n = Point(curPoint.x, curPoint.y-1),
			s = Point(curPoint.x, curPoint.y+1),
			e = Point(curPoint.x+1, curPoint.y),
			w = Point(curPoint.x-1, curPoint.y);
		if(seen.count(w) == 0 && curPoint.x != 0)
			children.push(w);
		if(seen.count(e) == 0 && curPoint.x != campusImage.cols)
			children.push(e);
		if(seen.count(n) == 0 && curPoint.y != 0)
			children.push(n);
		if(seen.count(s) == 0 && curPoint.y != campusImage.rows)
			children.push(s);

		count++;

	}

	return cloud;
}

void MapReader::retrMinFeatures(Building &buildPt)
{
	if( campusImage.at<Vec3b>(buildPt.centerOfMass) == Vec3b(255,255,255) )
	{
		buildPt.near.insert(campusLabeled.at<char>(buildPt.centerOfMass) - 1);
		return;
	}

	int minDistN, minDistS, minDistE, minDistW, minN, minS, minE, minW; 
	minDistN=minDistS=minDistE=minDistW = campusImage.rows + campusImage.cols;
	for(uchar i = 0; i < buildings.size(); i++)
	{
		int dist = abs(buildings[i].centerOfMass.x - buildPt.centerOfMass.x)
					+ abs(buildings[i].centerOfMass.y - buildPt.centerOfMass.y);
		if( near( buildings[i], buildPt ) )
			buildPt.near.insert(i);
		if( north( buildings[i], buildPt ) && dist < minDistN)
		{
			minDistN = dist;
			minN = i;
		}
		if( south( buildings[i], buildPt ) && dist < minDistS)
		{
			minDistS = dist;
			minS = i;
		}
		if( east( buildings[i], buildPt ) && dist < minDistE)
		{
			minDistE = dist;
			minE = i;
		}
		if( west( buildings[i], buildPt ) && dist < minDistW)
		{
			minDistW = dist;
			minW = i;
		}
	}


	//Keep only the closest direction relation
	if( minDistN < campusImage.rows + campusImage.cols )
		buildPt.north.insert(minN);
	if( minDistS < campusImage.rows + campusImage.cols )
		buildPt.south.insert(minS);
	if( minDistE < campusImage.rows + campusImage.cols )
		buildPt.east.insert(minE);
	if( minDistW < campusImage.rows + campusImage.cols )
		buildPt.west.insert(minW);

	
	for(auto i = buildPt.near.begin(); i != buildPt.near.end(); i++)
	{
		if ( !buildPt.north.empty() 
				&& north( buildings[*(buildPt.north.begin())], buildings[*i]) )
			buildPt.north.clear();
		if ( !buildPt.south.empty() 
				&& south( buildings[*(buildPt.south.begin())], buildings[*i]) )
			buildPt.south.clear();
		if ( !buildPt.east.empty() 
				&& east( buildings[*(buildPt.east.begin())], buildings[*i]) )
			buildPt.east.clear();
		if ( !buildPt.west.empty() 
				&& west( buildings[*(buildPt.west.begin())], buildings[*i]) )
			buildPt.west.clear();
	}
}


void MapReader::clicked(QMouseEvent *e)
{
	QDebug debugMessage(QtDebugMsg);
	if(takeinput)
	{
		Point pt(e->x(), e->y());

		int dist = abs(pt.x - currentPath[curIdx].centerOfMass.x) 
				+ abs(pt.y - currentPath[curIdx].centerOfMass.y);
		debugMessage << dist << "  from intended target";
		distances.push_back(dist);
		if(curIdx == currentPath.size() -1)
		{
			takeinput == false;
			return;
		}
		int i = currentPath[curIdx].number;
		int j = currentPath[curIdx + 1].number;
		debugMessage << "Now go ";
		string join = "";

	    printFeatures(currentPath[curIdx]);
		
		if(!buildings[i].north.empty() && buildings[i].north.count(j) > 0)
		{
			debugMessage << "north ";
			join = "and ";
		}
		
		if(!buildings[i].south.empty() && buildings[i].south.count(j) > 0)
		{
			debugMessage << join.c_str() << "south ";
			join = "and ";
		}
		
		if(!buildings[i].east.empty() && buildings[i].east.count(j) > 0)
		{
			debugMessage << join.c_str() << "east ";
			join = "and ";
		}
		
		if(!buildings[i].west.empty() && buildings[i].west.count(j) > 0)
		{
			debugMessage << join.c_str() << "west ";
			join = "and ";
		}
		
		if(!buildings[i].near.empty() && buildings[i].near.count(j) > 0)
		{
			debugMessage << join.c_str() << "near ";
		}

		curIdx++;
		return;
	}
	// cout << e->x() << "," << e->y() << endl;
	Point pt(e->x(), e->y());
	Building buildPt(pt, minArea);

	retrMinFeatures(buildPt);


	vector<Point> cloud = findCloud(buildPt);

	if(!ready)
	{
		drawCloud(cloud);
		goal = buildPt;
		ready = true;
		qDebug() << "Goal: ";
	}
	else
	{
		cloudImage = campusImage.clone();
		drawCloud(cloud);
		source = buildPt;
		ready = false;
		qDebug() << "Start: ";
	}

	printFeatures(buildPt);
}

void MapReader::drawCloud(vector<Point> cloud)
{
	for (std::vector<Point>::iterator i = cloud.begin(); i != cloud.end(); ++i)
	{
		if( !ready )
			cloudImage.at<Vec3b>(*i) = Vec3b(0,0,100);
		else
			cloudImage.at<Vec3b>(*i) = Vec3b(0,100,0);
		// qDebug() << (*i).x << ", " << (*i).y;
	}
	displayMat(cloudImage);
}




vector<Building> MapReader::search()
{
	vector<Building> moves;
	//keep total distance in first element
	Building dist;
	dist.number = 0;
	moves.push_back(dist);
	moves.push_back(source);

	unordered_set<Point> contained;
	moves = dfs(moves, contained);

	QTextStream qout(stdout);
	qout << "distance: " << moves[0].number << "\n";
	qout << "size: " << moves.size() << "\n";
	for(uchar i = 1; i < moves.size() - 1; i++)
	{
		qout << "step " << i << ":  go to ";
		int cur = moves[i].number -1;
		int next = moves[i+1].number -1;
		string join = "";

		if(next < 0)
		{
			// qDebug() << moves[i+1] << "\n:::-1221312:::" << next;
			// printFeatures(moves[i]);
			// qDebug()<<"\n";
			if(!moves[i+1].north.empty() && moves[i+1].north.count(cur) > 0)
			{
				qout << "south ";
				join = "and ";
			}
			
			if(!moves[i+1].south.empty() && moves[i+1].south.count(cur) > 0)
			{
				qout << join.c_str() << "north ";
				join = "and ";
			}
			
			if(!moves[i+1].east.empty() && moves[i+1].east.count(cur) > 0)
			{
				qout << join.c_str() << "west ";
				join = "and ";
			}
			
			if(!moves[i+1].west.empty() && moves[i+1].west.count(cur) > 0)
			{
				qout << join.c_str() << "east ";
				join = "and ";
			}
			
			if(!moves[i+1].near.empty() && moves[i+1].near.count(cur) > 0)
			{
				qout << join.c_str() << "near ";
			}

		}
		else
		{
			// qDebug() << moves[i] << "\n:::" << next;
			// printFeatures(moves[i]);
			// qDebug()<<"\n";
			if(!moves[i].north.empty() && moves[i].north.count(next) > 0)
			{
				qout << "north ";
				join = "and ";
			}
			
			if(!moves[i].south.empty() && moves[i].south.count(next) > 0)
			{
				qout << join.c_str() << "south ";
				join = "and ";
			}
			
			if(!moves[i].east.empty() && moves[i].east.count(next) > 0)
			{
				qout << join.c_str() << "east ";
				join = "and ";
			}
			
			if(!moves[i].west.empty() && moves[i].west.count(next) > 0)
			{
				qout << join.c_str() << "west ";
				join = "and ";
			}
			
			if(!moves[i].near.empty() && moves[i].near.count(next) > 0)
			{
				qout << join.c_str() << "near ";
			}
			else
			{
				qout << "to ";
			}
		}
		if(next < 0)
		{
            qout << "the location that: ";
            qout.flush();
            printFeatures(moves.back());
		}
		else
			qout << "(" << moves[i+1].description.c_str() << ")" << "\n";
	}

	return moves;
}

static int cntr = 0;
vector<Building> MapReader::dfs(const vector<Building> &moves, unordered_set<Point> contained)
{
	if(cntr++ % 100000 == 0 )
		qDebug() << cntr;
	Building curBuild = moves.back();
	if(moves.size() > buildings.size()/3 || moves[0].number > campusImage.rows + campusImage.cols)
		return vector<Building>();


	if(curBuild.north == goal.north && curBuild.south == goal.south
		&& curBuild.east == goal.east && curBuild.west == goal.west
		&& curBuild.near == goal.near )
	{
		return moves;
	}

	int idx = curBuild.number;
	vector<Building> minMoves = moves, tmp;
	minMoves[0].number = (campusImage.rows + campusImage.cols);

	if( goal.north.count(idx) || goal.south.count(idx)
		|| goal.east.count(idx) || goal.west.count(idx) || goal.near.count(idx))
	{
		int dist = abs(goal.centerOfMass.x - curBuild.centerOfMass.x) 
				+ abs(goal.centerOfMass.y - curBuild.centerOfMass.y);
		tmp = moves;
		tmp[0].number += dist;
		tmp.push_back(goal);
		return tmp;
	}

	for(auto i = curBuild.north.begin(); i != curBuild.north.end(); i++)
	{
		Building nextBuild = buildings[*i];
		if( contained.count(nextBuild.centerOfMass) > 0)
			continue;
		int dist = abs(nextBuild.centerOfMass.x - curBuild.centerOfMass.x) 
				+ abs(nextBuild.centerOfMass.y - curBuild.centerOfMass.y);
		tmp = moves;
		tmp[0].number += dist;
		tmp.push_back(nextBuild);
		unordered_set<Point> cTmp = (const unordered_set<Point>) contained;
		cTmp.insert(nextBuild.centerOfMass);
		tmp = dfs(tmp, cTmp);

		if(!tmp.empty() && tmp[0].number < minMoves[0].number)
			minMoves = tmp;
	}

	for(auto i = curBuild.south.begin(); i != curBuild.south.end(); i++)
	{
		Building nextBuild = buildings[*i];
		if( contained.count(nextBuild.centerOfMass) > 0)
			continue;
		int dist = abs(nextBuild.centerOfMass.x - curBuild.centerOfMass.x) 
				+ abs(nextBuild.centerOfMass.y - curBuild.centerOfMass.y);
		tmp = moves;
		tmp[0].number += dist;
		tmp.push_back(nextBuild);
		unordered_set<Point> cTmp = (const unordered_set<Point>) contained;
		cTmp.insert(nextBuild.centerOfMass);
		tmp = dfs(tmp, cTmp);

		if(!tmp.empty() && tmp[0].number < minMoves[0].number)
			minMoves = tmp;
	}

	for(auto i = curBuild.east.begin(); i != curBuild.east.end(); i++)
	{
		Building nextBuild = buildings[*i];
		if( contained.count(nextBuild.centerOfMass) > 0)
			continue;
		int dist = abs(nextBuild.centerOfMass.x - curBuild.centerOfMass.x) 
				+ abs(nextBuild.centerOfMass.y - curBuild.centerOfMass.y);
		tmp = moves;
		tmp[0].number += dist;
		tmp.push_back(nextBuild);
		unordered_set<Point> cTmp = (const unordered_set<Point>) contained;
		cTmp.insert(nextBuild.centerOfMass);
		tmp = dfs(tmp, cTmp);

		if(!tmp.empty() && tmp[0].number < minMoves[0].number)
			minMoves = tmp;
	}

	for(auto i = curBuild.west.begin(); i != curBuild.west.end(); i++)
	{
		Building nextBuild = buildings[*i];
		if( contained.count(nextBuild.centerOfMass) > 0)
			continue;
		int dist = abs(nextBuild.centerOfMass.x - curBuild.centerOfMass.x) 
				+ abs(nextBuild.centerOfMass.y - curBuild.centerOfMass.y);
		tmp = moves;
		tmp[0].number += dist;
		tmp.push_back(nextBuild);
		unordered_set<Point> cTmp = (const unordered_set<Point>) contained;
		cTmp.insert(nextBuild.centerOfMass);
		tmp = dfs(tmp, cTmp);

		if(!tmp.empty() && tmp[0].number < minMoves[0].number)
			minMoves = tmp;
	}

	for(auto i = curBuild.near.begin(); i != curBuild.near.end(); i++)
	{
		Building nextBuild = buildings[*i];
		if( contained.count(nextBuild.centerOfMass) > 0)
			continue;
		int dist = abs(nextBuild.centerOfMass.x - curBuild.centerOfMass.x) 
				+ abs(nextBuild.centerOfMass.y - curBuild.centerOfMass.y);
		tmp = moves;
		tmp[0].number += dist;
		tmp.push_back(nextBuild);
		unordered_set<Point> cTmp = (const unordered_set<Point>) contained;
		cTmp.insert(nextBuild.centerOfMass);
		tmp = dfs(tmp, cTmp);

		if(!tmp.empty() && tmp[0].number < minMoves[0].number)
			minMoves = tmp;
	}

	return minMoves;

}


void MapReader::on_btnStart_clicked()
{
    currentPath = search();
    displayMat(campusImage);

    // qDebug() << "Begin at:";
    // printFeatures(currentPath[1]);

    if(!currentPath.size() > 2)
    {
	    takeinput = true;
	    curIdx = 1;
	}
}




