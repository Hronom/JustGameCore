#include "Entity.h"

#include <QMetaType>

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

    QList<IComponent*> Entity::getComponents() const
    {
        return mComponents.values();
    }
}
