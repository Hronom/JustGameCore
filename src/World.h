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
        bool mWorldLoaded;

        /** node id - component type */
        QMultiHash<qint32, qint32> mNodesLibrary;

        /** entity name - entity */
        QHash<QString, Entity*> mEntitys;
        /** node id - entity name */
        QMultiHash<qint32, QString> mNodes;

        /** system priority - system */
        QMultiHash<qint32, ISystem*> mSystems;

    public:
        World(QString xWorldName);
        virtual ~World();

        QString getName();

    protected:
        void setWorldLoaded(bool xWorldLoaded);

    public:
        bool isWorldLoaded();

        void addComponentToNode(qint32 xNodeID, qint32 xComponentType);
        void removeComponentFromNode(qint32 xNodeID, qint32 xComponentType);

        Entity* getEntity(QString xName);
        void removeEntity(QString xName);
        QVector<Entity*> getEntitysInNode(qint32 xNodeID);

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
