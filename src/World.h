#ifndef WORLD_H
#define WORLD_H

#include "Entity.h"
#include "IComponent.h"
#include "ISystem.h"

#include <QString>
#include <QMultiHash>

namespace JGC
{
    class World
    {
    private:
        QString mWorldName;

        /** Node name - Component type */
        QMultiHash<QString, QString> mNodesLibrary;

        /** Entity name - Entity */
        QHash<QString, Entity*> mEntitys;
        /** Node name - Entity name */
        QMultiHash<QString, QString> mNodes;

        /** Priority - System */
        QMultiHash<qint32, ISystem*> mSystems;

    public:
        World(QString xWorldName);
        virtual ~World();

        QString getName();

        void addComponentToNode(QString xNodeName, QString xComponentType);
        void removeComponentFromNode(QString xNodeName, QString xComponentType);

        void removeEntity(QString xName);
        QVector<Entity*> getEntitysInNode(QString xNodeName);
        void addComponent(QString xEntityName, IComponent* xComponent);
        void removeComponent(QString xEntityName, IComponent* xComponent);

        void addSystem(qint32 xPriority, ISystem* xSystem);
        void changeSystemPriority(ISystem* xSystem, qint32 xNewPriority);
        void removeSystem(ISystem* xSystem);

        void injectUpdate(const float& xTimeSinceLastUpdate);

        void printInfo();

        virtual void load() = 0;
        virtual void enter() = 0;
        virtual void exit() = 0;
    };
}

#endif
