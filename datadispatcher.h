#ifndef DATADISPATCHER_H
#define DATADISPATCHER_H

#include <QObject>
#include <QXYSeries>

#include "datagenerator.h"

QT_CHARTS_USE_NAMESPACE

class DataDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit DataDispatcher(QObject *parent = nullptr);
    Q_INVOKABLE void setSeries(QXYSeries *series);

signals:
    void readyToStart();
    void started();
    void pauseGenerator();
    void resumeGenerator();
    void finished();

public slots:
    void start();
    void pause();
    void stop();

private slots:
    void onDataReady(QSharedPointer<DataRow> pData);
    void onGeneratorFinished();

private:
    DataGenerator *m_generator;
    QXYSeries *m_series;
};


#endif // DATADISPATCHER_H
