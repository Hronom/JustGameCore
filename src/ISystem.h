#ifndef ISYSTEM_H
#define ISYSTEM_H

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
