#include "waveforms.h"
#include "ui_waveforms.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QPainter>

WaveForms::WaveForms(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WaveForms)
{
    ui->setupUi(this);
    ui->GraphWidget->addGraph();
    AccessFile("/Users/alyalaswad/Desktop/Spring 2024/CSCE 2301/Project 1/QT_DD1/WaveFormsDD1/mySimulation.sim");
}

void WaveForms::AccessFile(QString pathname)
{
    QFile simFile(pathname);
    if (!simFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << simFile.errorString();
        return;
    }

    QMap<QString, QPair<QVector<double>, QVector<double>>> signalMap;

    QTextStream in(&simFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');

        if (parts.size() != 3) {
            qDebug() << "Skipping line due to incorrect format:" << line;
            continue;
        }

        QString signalName = parts[1];
        int time = parts[0].toInt(); // Convert time to integer
        bool signalValue = parts[2].toInt();

        signalMap[signalName].first.append(time); // Use integer time value
        signalMap[signalName].second.append(signalValue ? 1.0 : 0.0);
    }

    PlotSignals(signalMap);
}

void WaveForms::PlotSignals(const QMap<QString, QPair<QVector<double>, QVector<double>>>& signalMap)
{
    ui->GraphWidget->clearGraphs();

    // Set background color to black
    ui->GraphWidget->setBackground(QBrush(Qt::black));

    // Customize the Y-axis to display signal names
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    QMap<QString, double> signalPositions;
    double yPos = 1.0; // Start from 1 to ensure no signal is on the x-axis

    // Find the maximum and minimum time values
    double maxTime = 0.0;
    for (const QString& signalName : signalMap.keys()) {
        const QVector<double>& times = signalMap.value(signalName).first;
        if (!times.isEmpty()) {
            double maxSignalTime = times.last();
            if (maxSignalTime > maxTime) {
                maxTime = maxSignalTime;
            }
        }
    }

    for (const QString& signalName : signalMap.keys()) {
        signalPositions[signalName] = yPos;
        textTicker->addTick(yPos, signalName);
        yPos += 2.0; // Increment position for each signal
    }

    ui->GraphWidget->yAxis->setTicker(textTicker);
    ui->GraphWidget->yAxis->setTickLabelColor(Qt::white); // Set signal names color to white
    ui->GraphWidget->yAxis->setLabelColor(Qt::white); // Set axis label color to white

    ui->GraphWidget->xAxis->setLabel("Time");
    ui->GraphWidget->xAxis->setLabelColor(Qt::white); // Set axis label color to white

    // Plot each signal
    for (const QString& signalName : signalMap.keys()) {
        const QVector<double>& times = signalMap.value(signalName).first;
        const QVector<double>& states = signalMap.value(signalName).second;

        // Create a new graph for the signal
        QCPGraph *graph = ui->GraphWidget->addGraph();
        graph->setName(signalName);
        graph->setLineStyle(QCPGraph::lsStepCenter); // Set the line style to step center
        graph->setScatterStyle(QCPScatterStyle::ssNone); // Set scatter style to none

        // Set line color to green
        graph->setPen(QPen(Qt::green));

        // Store the points to plot
        QVector<double> xData, yData;

        // Add the initial point
        xData.append(0.0);
        yData.append(signalPositions[signalName]);

        // Iterate over the signal states and plot the points
        for (int i = 0; i < times.size(); ++i) {
            double time = times[i];
            double state = states[i];
            double y = state > 0.0 ? signalPositions[signalName] + 1.0 : signalPositions[signalName];

            // Add current state point
            xData.append(time);
            yData.append(y);

            // Extend the signal until the end of the plot
            if (i == times.size() - 1) {
                xData.append(maxTime);
                yData.append(y);
            }
        }

        // Set the data for the graph
        graph->setData(xData, yData);
    }

    // Rescale axes and replot the graph
    ui->GraphWidget->rescaleAxes();
    ui->GraphWidget->replot();
}


WaveForms::~WaveForms()
{
    delete ui;
}
