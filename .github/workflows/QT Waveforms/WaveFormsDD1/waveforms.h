#ifndef WAVEFORMS_H
#define WAVEFORMS_H

#include <QMainWindow>
#include <QMap>
#include <QVector>
#include <QPair>

QT_BEGIN_NAMESPACE
namespace Ui { class WaveForms; }
QT_END_NAMESPACE

class WaveForms : public QMainWindow
{
    Q_OBJECT

public:
    WaveForms(QWidget *parent = nullptr);
    void PlotSignals(const QMap<QString, QPair<QVector<double>, QVector<double>>>& signalMap);
    void AccessFile(QString pathname);
    ~WaveForms();

private:
    Ui::WaveForms *ui;
};
struct simInfo
{
    int timestamp;
    QString variable;
    bool value;

};

#endif // WAVEFORMS_H
