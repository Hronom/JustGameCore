#include "Entity.h"

namespace JGC
{
    Entity::Entity(QString xName)
    {
        mName = xName;
    }

    QString Entity::getName() const
    {
        return mName;
    }

    bool Entity::hasComponent(const qint32 &xType) const
    {
        return mComponents.contains(xType);
    }

    IComponent *Entity::getComponent(const qint32 &xType) const
    {
        return mComponents.value(xType);
    }
}
