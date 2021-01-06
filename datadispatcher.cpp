#include "datadispatcher.h"

DataDispatcher::DataDispatcher(QObject *parent):
    QObject(parent)
{
    qRegisterMetaType<QSharedPointer<DataRow>>("QSharedPointer<DataRow>");
}

void DataDispatcher::setSeries(QXYSeries *series)
{
    if (series)
    {
        m_series = series;
        emit readyToStart();
    }
}

void DataDispatcher::start()
{
    if (m_generator)
    {
        emit resumeGenerator();
    }
    else
    {
        m_generator = new DataGenerator(m_series);
        connect(m_generator, SIGNAL(dataReady(QSharedPointer<DataRow>)), this, SLOT(onDataReady(QSharedPointer<DataRow>)));
        connect(m_generator, SIGNAL(finished()), this, SLOT(onGeneratorFinished()));
        connect(this, SIGNAL(pauseGenerator()), m_generator, SLOT(pause()));
        connect(this, SIGNAL(resumeGenerator()), m_generator, SLOT(resume()));

        m_generator->start();
        emit started();
    }
}

void DataDispatcher::pause()
{
    if (m_generator)
    {
        if (m_generator->isPaused())
            emit resumeGenerator();
        else
            emit pauseGenerator();
    }
}

void DataDispatcher::stop()
{
    if (m_generator)
    {
        if (m_generator->isPaused())
            emit resumeGenerator();

        m_generator->requestInterruption();
        m_generator->release();
        m_generator->wait();
    }
}

void DataDispatcher::onDataReady(QSharedPointer<DataRow> pData)
{
    m_series->replace(*pData.data());
    m_generator->release();
}

void DataDispatcher::onGeneratorFinished()
{
    delete m_generator;
    m_generator = nullptr;
    m_series->clear();
    emit readyToStart();
}
