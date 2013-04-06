#ifndef MAPREADER_H
#define MAPREADER_H

#include <QMainWindow>

namespace Ui {
class MapReader;
}

class MapReader : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MapReader(QWidget *parent = 0);
    ~MapReader();
    
    void MapReader::displayMat(const cv::Mat& image);

private:
    Ui::MapReader *ui;
};

#endif // MAPREADER_H
