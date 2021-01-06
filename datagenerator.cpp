#include <QApplication>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>

#include "datagenerator.h"

DataGenerator::DataGenerator(QXYSeries *series):
    m_series{series},
    m_paused{false},
    m_semaphore{1}
{}

void DataGenerator::release()
{
    m_semaphore.release();
}

void DataGenerator::run()
{
    qreal x, y;

    while (!isInterruptionRequested())
    {
        m_mutex.lock();
        if (m_paused)
        {
            m_pauseCondition.wait(&m_mutex);
        }
        m_mutex.unlock();

        m_data.clear();
        m_data.reserve(1024);
        for (int i = 0; i < 1024; i++)
        {
            x = i;
            y = qCos(M_PI / 256 * i) + QRandomGenerator::global()->generateDouble() - 0.5;
            m_data.append(QPointF(x, y));
        }

        m_semaphore.acquire();
        QSharedPointer<DataRow> pData{new DataRow{m_data}};
        emit dataReady(pData);
    }
}

void DataGenerator::pause()
{
    QMutexLocker l{&m_mutex};
    m_paused = true;
}

void DataGenerator::resume()
{
    m_mutex.lock();
    m_paused = false;
    m_mutex.unlock();
    m_pauseCondition.wakeAll();
}
