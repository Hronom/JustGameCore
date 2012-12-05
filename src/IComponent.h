#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include <QString>

namespace JGC
{
    class IComponent
	{
	public:
        virtual ~IComponent(){}
        virtual qint32 getType() = 0;
	};
}

#endif
