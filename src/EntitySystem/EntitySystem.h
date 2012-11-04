#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include <QString>
#include <QMultiMap>

#include "IComponent.h"
#include "ISystem.h"

namespace JGC
{
    class EntitySystem
    {
    private:
        /** Node name - Component type */
        QMultiMap<QString, QString> mNodesLibrary;
        /** Entity name - Component */
        QMultiMap<QString, IComponent*> mEntitys;
        /** Node name - Entity name */
        QMultiMap<QString, QString> mNodes;
        /** Priority - System */
        QMultiMap<qint32, ISystem*> mSystems;

    public:
        EntitySystem();
        ~EntitySystem();

        void update();

        void addComponentToNode(QString xNodeName, QString xComponentType);
        void removeComponentFromNode(QString xNodeName, QString xComponentType);

        void removeEntity(QString xName);
        void addComponent(QString xEntityName, IComponent* xComponent);
        void removeComponent(QString xEntityName, IComponent* xComponent);

        void addSystem(qint32 xPriority, ISystem* xSystem);
        void changeSystemPriority(ISystem* xSystem, qint32 xNewPriority);
        void removeSystem(ISystem* xSystem);

        void printInfo();
    };
}

#endif
