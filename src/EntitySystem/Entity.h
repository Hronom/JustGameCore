#ifndef ENTITY_H
#define ENTITY_H

#include "IComponent.h"
#include <QHash>

namespace JGC
{
    class Entity
    {
        friend class EntitySystem;
    private:
        QString mName;

        QHash<QString, IComponent*> mComponents;

    public:
        Entity(QString xName);

        QString getName() const;
        bool hasComponent(const QString &xType) const;
        IComponent* getComponent(const QString &xType) const;
    };
}

#endif // ENTITY_H
