#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QSemaphore>
#include <QWaitCondition>
#include <QXYSeries>

QT_CHARTS_USE_NAMESPACE

typedef QVector<QPointF> DataRow;

class DataGenerator : public QThread
{
    Q_OBJECT
public:
    explicit DataGenerator(QXYSeries* series);
    bool isPaused() { QMutexLocker l(&m_mutex); return m_paused; }
    void release();

signals:
    void dataReady(QSharedPointer<DataRow> pData);

protected:
    void run() override;

public slots:
    void pause();
    void resume();

private:
    QXYSeries* m_series;
    DataRow m_data;
    bool m_paused;
    QMutex m_mutex;
    QWaitCondition m_pauseCondition;
    QSemaphore m_semaphore;
};

#endif // DATAGENERATOR_H
