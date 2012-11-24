#ifndef ISYSTEM_H
#define ISYSTEM_H

#include "Entity.h"

#include <QString>
#include <QVector>

namespace JGC
{
    class ISystem
	{
	public:
        virtual ~ISystem(){}

        virtual void injectUpdate(const float &xTimeSinceLastUpdate) = 0;
	};
}

#endif
