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

        template<typename T>
        T* getComponent() const
        {
            int xComponentType = qMetaTypeId<T>();
            if(mComponents.contains(xComponentType))
                return static_cast<T*>(mComponents.value(xComponentType));
            else
                return NULL;
        }

        QList<IComponent*> getComponents() const;
    };
}

#endif // ENTITY_H
