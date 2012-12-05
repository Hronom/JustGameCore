#ifndef ENTITY_H
#define ENTITY_H

#include "IComponent.h"
#include <QHash>

namespace JGC
{
    class Entity
    {
        friend class World;
    private:
        QString mName;

        // component type - component
        QHash<quint32, IComponent*> mComponents;

    public:
        Entity(QString xName);

        QString getName() const;
        bool hasComponent(const qint32 &xType) const;
        IComponent* getComponent(const qint32 &xType) const;
    };
}

#endif // ENTITY_H
