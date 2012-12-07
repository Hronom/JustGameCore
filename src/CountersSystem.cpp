#include "CountersSystem.h"

#include <QDebug>

namespace JGC
{
    CountersSystem* CountersSystem::mInstance = 0;

    bool CountersSystem::initialize()
    {
        mInstance = new CountersSystem();
        return true;
    }

    void CountersSystem::shutdown()
    {
        qDebug()<<"Shutdown:"<<"CountersSystem";
        delete mInstance;
        mInstance = 0;
    }

    CountersSystem* CountersSystem::instance()
    {
        return mInstance;
    }

    CountersSystem::CountersSystem()
    {
    }

    CountersSystem::~CountersSystem()
    {
        while(!mCounters.empty())
        {
            QHash<QString, Counter*>::Iterator xIter;
            xIter = mCounters.begin();
            delete xIter.value();
            mCounters.erase(xIter);
        }
    }

    void CountersSystem::addCounter(QString xName)
    {
        if(!mCounters.contains(xName))
            mCounters.insert(xName, new Counter());
    }

    void CountersSystem::removeCounter(QString xName)
    {
        if(!mCounters.contains(xName))
            delete mCounters.take(xName);
    }

    void CountersSystem::removeName(QString xCounterName, QString xName)
    {
        if(mCounters.contains(xCounterName))
        {
            Counter *xCounter;
            xCounter = mCounters.value(xCounterName);
            xCounter->removeName(xName);
        }
    }

    QString CountersSystem::getNameWithSuffix(QString xCounterName, QString xName)
    {
        if(mCounters.contains(xCounterName))
            return mCounters.value(xCounterName)->getNameWithSuffix(xName);
        else
        {
            mCounters.insert(xName, new Counter());
            return mCounters.value(xCounterName)->getNameWithSuffix(xName);
        }
    }
}
