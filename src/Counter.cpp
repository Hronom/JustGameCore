#include "Counter.h"

namespace JGC
{
    Counter::Counter()
    {
        mValues.insert(0, "");
    }

    Counter::~Counter()
    {

    }

    void Counter::removeName(QString xName)
    {
        QHash<qint32, QString>::Iterator xIter;
        xIter = mValues.begin();
        while(xIter != mValues.end())
        {
            if(xIter.value() == xName)
            {
                xIter.value() = "";
                break;
            }

            ++xIter;
        }
    }

    QString Counter::getNameWithSuffix(QString xName)
    {
        QHash<qint32, QString>::Iterator xIter;
        xIter = mValues.begin();
        while(xIter != mValues.end())
        {
            if(xIter.value() == "")
            {
                QString xNameWithSuffix;
                xNameWithSuffix = xName + QString::number(xIter.key());
                xIter.value() = xNameWithSuffix;
                return xNameWithSuffix;
            }

            ++xIter;
        }

        qint32 xValue;
        xValue = mValues.size();
        QString xNameWithSuffix;
        xNameWithSuffix = xName + QString::number(xValue);
        mValues.insert(xValue, xNameWithSuffix);
        return xNameWithSuffix;
    }
}
