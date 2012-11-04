#ifndef ISYSTEM_H
#define ISYSTEM_H

#include <QString>

namespace JGC
{
    class ISystem
	{
	public:
        virtual ~ISystem(){}

        virtual QString getNodeType() = 0;
        virtual void proceedEntitys(QMultiMap<QString, IComponent*> xEntitys) = 0;
	};
}

#endif
