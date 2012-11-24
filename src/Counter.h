#ifndef COUNTER_H
#define COUNTER_H

#include <QHash>
#include <qglobal.h>

namespace JGC
{
    class Counter
	{
	private:
        // Value - Key
        QHash<qint32, QString> mValues;

    public:
        Counter();
        ~Counter();

        void removeName(QString xName);
        QString getNameWithSuffix(QString xName);
    };
}

#endif
