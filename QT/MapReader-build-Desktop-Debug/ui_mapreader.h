/********************************************************************************
** Form generated from reading UI file 'mapreader.ui'
**
** Created: Tue Apr 9 13:06:08 2013
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
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "clickablelabel.h"

QT_BEGIN_NAMESPACE

class Ui_MapReader
{
public:
    QWidget *centralWidget;
    ClickableLabel *lblImage;
    QPushButton *btnStart;
    QPushButton *btnReset;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MapReader)
    {
        if (MapReader->objectName().isEmpty())
            MapReader->setObjectName(QString::fromUtf8("MapReader"));
        MapReader->resize(443, 571);
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
        btnStart = new QPushButton(centralWidget);
        btnStart->setObjectName(QString::fromUtf8("btnStart"));
        btnStart->setGeometry(QRect(320, 20, 114, 32));
        btnReset = new QPushButton(centralWidget);
        btnReset->setObjectName(QString::fromUtf8("btnReset"));
        btnReset->setGeometry(QRect(320, 50, 114, 32));
        MapReader->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MapReader);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 443, 22));
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
        lblImage->setProperty("text", QVariant(QString()));
        btnStart->setText(QApplication::translate("MapReader", "Start", 0, QApplication::UnicodeUTF8));
        btnReset->setText(QApplication::translate("MapReader", "Reset", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MapReader: public Ui_MapReader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPREADER_H
