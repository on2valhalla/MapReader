/********************************************************************************
** Form generated from reading UI file 'mapreader.ui'
**
** Created: Mon Apr 8 11:23:43 2013
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
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "clicklabel.h"

QT_BEGIN_NAMESPACE

class Ui_MapReader
{
public:
    QWidget *centralWidget;
    ClickableLabel *lblImage;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MapReader)
    {
        if (MapReader->objectName().isEmpty())
            MapReader->setObjectName(QString::fromUtf8("MapReader"));
        MapReader->resize(321, 571);
        centralWidget = new QWidget(MapReader);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        lblImage = new ClickableLabel(centralWidget);
        lblImage->setObjectName(QString::fromUtf8("lblImage"));
        lblImage->setGeometry(QRect(21, 15, 275, 495));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblImage->sizePolicy().hasHeightForWidth());
        lblImage->setSizePolicy(sizePolicy);
        lblImage->setMouseTracking(true);
        MapReader->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MapReader);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 321, 22));
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
        lblImage->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MapReader: public Ui_MapReader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPREADER_H
