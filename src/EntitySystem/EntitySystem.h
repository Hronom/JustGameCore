#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include "Entity.h"
#include "IComponent.h"
#include "ISystem.h"

#include <QString>
#include <QMultiHash>

namespace JGC
{
    class EntitySystem
    {
    private:
        static EntitySystem* mInstance;

        /** Node name - Component type */
        QMultiHash<QString, QString> mNodesLibrary;

        /** Entity name - Entity */
        QHash<QString, Entity*> mEntitys;
        /** Node name - Entity name */
        QMultiHash<QString, QString> mNodes;

        /** Priority - System */
        QMultiHash<qint32, ISystem*> mSystems;

    public:
        static void initialize();
        static void shutdown();
        static EntitySystem* instance();

    private:
        EntitySystem();
        ~EntitySystem();

    public:
        void addComponentToNode(QString xNodeName, QString xComponentType);
        void removeComponentFromNode(QString xNodeName, QString xComponentType);

        void removeEntity(QString xName);
        void addComponent(QString xEntityName, IComponent* xComponent);
        void removeComponent(QString xEntityName, IComponent* xComponent);

        void addSystem(qint32 xPriority, ISystem* xSystem);
        void changeSystemPriority(ISystem* xSystem, qint32 xNewPriority);
        void removeSystem(ISystem* xSystem);

        void injectUpdate(const float& xTimeSinceLastUpdate);

        void printInfo();
    };
}

#endif
