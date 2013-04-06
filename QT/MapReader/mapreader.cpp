

#include "mapreader.h"
#include "ui_mapreader.h"

MapReader::MapReader(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MapReader)
{
	ui->setupUi(this);

	campusImage = imread("../../../../../ass3-campus.pgm", CV_LOAD_IMAGE_GRAYSCALE);
	campusLabeled = imread("../../../../../ass3-labeled.pgm", CV_LOAD_IMAGE_GRAYSCALE);

	getNames("../../../../../ass3-table.txt");


//	namedWindow("Map");
//	imshow("Map", campusImage);

//	cout << campusLabeled.rows << " " << campusLabeled.cols << " "
//		 << (int)campusLabeled.at<uchar>(40,7) << endl;
	Mat wRects(campusImage.size(), CV_8UC1, Scalar(0));
    processFeatures(campusLabeled, wRects);
    describeBuildings();

    for(uchar i = 0; i < buildings.size() ; i++)
        for( uchar j = 0; j < buildings.size(); j++)
            cout << buildings[i].name << " is north of " << buildings[j].name << ": "
                    << north( buildings[j], buildings[i]) << endl;

	displayMat(wRects);
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
//        cout << (int) b.number << " " << b.name <<  endl;
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
					CV_CHAIN_APPROX_NONE); // an approximation algorithm

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

//        cout << (int) (*b).number << endl;

		rectangle(wRects, (*b).MBR, cv::Scalar(128), 2);
		drawContours(wRects, contours, idx, Scalar(255));
		circle( wRects, (*b).centerOfMass, 3, Scalar(128), 1 );
		circle( wRects, Point( (*b).MBR.x + (*b).MBR.width/2, (*b).MBR.y + (*b).MBR.height/2 ),
					3, Scalar(255), 1 );


//        wRects += labeled;
    }

}

void MapReader::describeBuildings()
{
	// Compute ranges/averages for size
	int min = campusImage.rows * campusImage.cols, max=0, total =0;
	for( uchar i = 0; i < buildings.size(); i++)
	{
		Building b = buildings[i];
		total += b.mom.m00;
		if(b.mom.m00 < min)
			min = b.mom.m00;
		if(b.mom.m00 > max)
			max = b.mom.m00;
	}
    int average = total / buildings.size(), range = max - min;



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
//        else
//            cout  << (int) b.number << " "<< b.name << " " << (int)campusImage.at<uchar>(centery, b.MBR.x + 1) << " "
//             << (int)campusImage.at<uchar>(centery, b.MBR.x + b.MBR.width - 1) << " "
//             << (int)campusImage.at<uchar>(b.MBR.y + 1, centerx) << " "
//              <<  (int)campusImage.at<uchar>(b.MBR.y + b.MBR.height - 1, centerx) << endl;

//        cout  << (int) b.number << " "<< b.name << " " <<  b.description << endl;


        // FIND SIZE DESCRIPTORS
        if( b.mom.m00 > average + range / 6)
            b.description += "large, ";
        else if( b.mom.m00 < average - range / 4)
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

//        cout  << (int) b.number << " "<< b.name << "\n  " <<  b.description << endl << endl;
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
    return true;
}
bool MapReader::east(const Building &s, const Building &g)
{
    return true;
}
bool MapReader::west(const Building &s, const Building &g)
{
    return true;
}
bool MapReader::near(const Building &s, const Building &g)
{
    return true;
}
