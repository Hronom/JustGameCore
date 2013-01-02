#ifndef WORLD_H
#define WORLD_H

#include "Entity.h"
#include "IComponent.h"
#include "ISystem.h"

#include <QString>
#include <QMultiHash>
#include <QMetaType>
#include <QDebug>

namespace JGC
{
    class World
    {
    private:
        QString mWorldName;
        bool mWorldActive;

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
        void setWorldActive(bool xWorldActive);

    public:
        template<typename T>
        void addComponentToNode(qint32 xNodeID)
        {
            int xComponentType = qMetaTypeId<T>();
            mNodesLibrary.insert(xNodeID, xComponentType);
        }

        template<typename T>
        void removeComponentFromNode(qint32 xNodeID)
        {
            int xComponentType = qMetaTypeId<T>();
            mNodesLibrary.remove(xNodeID, xComponentType);
        }

        Entity* getEntity(QString xName);
        QList<Entity*> getEntitys();
        void removeEntity(QString xName);
        QVector<Entity*> getEntitysInNode(qint32 xNodeID);

        template<typename T>
        void addComponent(QString xEntityName, T *xComponent)
        {
            int xComponentType = qMetaTypeId<T>();

            Entity *xEntity;
            // Add component
            if(mEntitys.contains(xEntityName))
            {
                xEntity = mEntitys.value(xEntityName);
                xEntity->mComponents.insert(xComponentType, xComponent);
            }
            else
            {
                xEntity = new Entity(xEntityName);
                xEntity->mComponents.insert(xComponentType, xComponent);
                mEntitys.insert(xEntityName, xEntity);
            }

            // Update nodes
            {
                // Get list of nodes names that have type of added component
                QList<qint32> xNodesIDs;
                xNodesIDs = mNodesLibrary.keys(xComponentType);

                QList<qint32>::iterator xNodesIDsIter;
                xNodesIDsIter = xNodesIDs.begin();
                for(; xNodesIDsIter != xNodesIDs.end(); ++xNodesIDsIter)
                {
                    // Form list of needed components to be in node
                    QList<qint32> xComTypes;
                    xComTypes = mNodesLibrary.values((*xNodesIDsIter));
                    xComTypes.removeAll(xComponentType);

                    // Check is entity have all components to be in current node
                    bool xHaveAllComponents = true;

                    QList<qint32>::iterator xComTypesIter;
                    xComTypesIter = xComTypes.begin();
                    for(; xComTypesIter != xComTypes.end(); ++xComTypesIter)
                    {
                        if(!xEntity->hasComponent((*xComTypesIter)))
                        {
                            xHaveAllComponents = false;
                            break;
                        }
                    }

                    if(xHaveAllComponents)
                        mNodes.insert((*xNodesIDsIter), xEntityName);
                }
            }
        }

        template<typename T>
        void removeComponent(QString xEntityName, T *xComponent)
        {
            if(mEntitys.contains(xEntityName))
            {
                int xComponentType = qMetaTypeId<T>();

                // Remove entity
                Entity *xEntity;
                xEntity = mEntitys.value(xEntityName);
                xEntity->mComponents.remove(xComponentType);

                // Remove node
                QList<qint32> xNodesIDs;
                xNodesIDs = mNodesLibrary.keys(xComponentType);

                QList<qint32>::iterator xNodesIDsIter;
                xNodesIDsIter = xNodesIDs.begin();
                for( ; xNodesIDsIter != xNodesIDs.end(); ++xNodesIDsIter)
                    mNodes.remove((*xNodesIDsIter), xEntityName);
            }
        };

        void addSystem(qint32 xPriority, ISystem* xSystem);
        void changeSystemPriority(ISystem* xSystem, qint32 xNewPriority);
        QList<ISystem*> getAllSystems();
        void removeSystem(ISystem* xSystem);

        void injectUpdate(const float& xTimeSinceLastUpdate);

        void printInfo();

        virtual void enter() = 0;
        virtual void exit() = 0;
    };
}

#endif
