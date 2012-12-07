#ifndef COUNTERSSYSTEM_H
#define COUNTERSSYSTEM_H

#include "Counter.h"

#include <QHash>
#include <QString>

namespace JGC
{
    class CountersSystem
	{
	private:
        static CountersSystem* mInstance;

        QHash<QString, Counter*> mCounters;

	public:
        static bool initialize();
		static void shutdown();
        static CountersSystem* instance();

	private:
        CountersSystem();
        ~CountersSystem();

	public:
        void addCounter(QString xName);
        void removeCounter(QString xName);

        void removeName(QString xCounterName, QString xName);
        QString getNameWithSuffix(QString xCounterName, QString xName);
	};
}

#endif
