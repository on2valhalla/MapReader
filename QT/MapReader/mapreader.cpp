#include "mapreader.h"
#include "ui_mapreader.h"

MapReader::MapReader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MapReader)
{
    ui->setupUi(this);
}

MapReader::~MapReader()
{
    delete ui;
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
