/********************************************************************************
** Form generated from reading UI file 'waveforms.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAVEFORMS_H
#define UI_WAVEFORMS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_WaveForms
{
public:
    QWidget *centralwidget;
    QCustomPlot *GraphWidget;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *WaveForms)
    {
        if (WaveForms->objectName().isEmpty())
            WaveForms->setObjectName("WaveForms");
        WaveForms->resize(800, 600);
        centralwidget = new QWidget(WaveForms);
        centralwidget->setObjectName("centralwidget");
        GraphWidget = new QCustomPlot(centralwidget);
        GraphWidget->setObjectName("GraphWidget");
        GraphWidget->setGeometry(QRect(10, 60, 821, 541));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(250, -10, 171, 81));
        QFont font;
        font.setPointSize(29);
        label->setFont(font);
        WaveForms->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WaveForms);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 24));
        WaveForms->setMenuBar(menubar);
        statusbar = new QStatusBar(WaveForms);
        statusbar->setObjectName("statusbar");
        WaveForms->setStatusBar(statusbar);

        retranslateUi(WaveForms);

        QMetaObject::connectSlotsByName(WaveForms);
    } // setupUi

    void retranslateUi(QMainWindow *WaveForms)
    {
        WaveForms->setWindowTitle(QCoreApplication::translate("WaveForms", "WaveForms", nullptr));
        label->setText(QCoreApplication::translate("WaveForms", "Waveforms", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WaveForms: public Ui_WaveForms {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAVEFORMS_H
