/********************************************************************************
** Form generated from reading UI file 'mapreader.ui'
**
** Created: Sun Apr 7 16:44:46 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPREADER_H
#define UI_MAPREADER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MapReader
{
public:
    QWidget *centralWidget;
    QLabel *lblImage;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MapReader)
    {
        if (MapReader->objectName().isEmpty())
            MapReader->setObjectName(QString::fromUtf8("MapReader"));
        MapReader->resize(544, 793);
        centralWidget = new QWidget(MapReader);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        lblImage = new QLabel(centralWidget);
        lblImage->setObjectName(QString::fromUtf8("lblImage"));
        lblImage->setGeometry(QRect(21, 15, 501, 711));
        MapReader->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MapReader);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 544, 22));
        MapReader->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MapReader);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MapReader->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MapReader);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MapReader->setStatusBar(statusBar);

        retranslateUi(MapReader);

        QMetaObject::connectSlotsByName(MapReader);
    } // setupUi

    void retranslateUi(QMainWindow *MapReader)
    {
        MapReader->setWindowTitle(QApplication::translate("MapReader", "MapReader", 0, QApplication::UnicodeUTF8));
        lblImage->setText(QApplication::translate("MapReader", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MapReader: public Ui_MapReader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPREADER_H
