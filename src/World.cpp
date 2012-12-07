#include "World.h"

#include <QDebug>

namespace JGC
{
    World::World(QString xWorldName)
    {
        mWorldName = xWorldName;
        mWorldLoaded = false;
    }

    World::~World()
    {
        qDebug()<<"Destroying world:"<<mWorldName;
    }

    QString World::getName()
    {
        return mWorldName;
    }

    void World::setWorldLoaded(bool xWorldLoaded)
    {
        mWorldLoaded = xWorldLoaded;
    }

    bool World::isWorldLoaded()
    {
        return mWorldLoaded;
    }

    void World::addComponentToNode(qint32 xNodeID, qint32 xComponentType)
    {
        mNodesLibrary.insert(xNodeID, xComponentType);
    }

    void World::removeComponentFromNode(qint32 xNodeID, qint32 xComponentType)
    {
        mNodesLibrary.remove(xNodeID, xComponentType);
    }

    Entity* World::getEntity(QString xName)
    {
        return mEntitys.value(xName, 0);
    }

    void World::removeEntity(QString xName)
    {
        mEntitys.remove(xName);
    }

    QVector<Entity*> World::getEntitysInNode(qint32 xNodeID)
    {
        QVector<Entity*> xEntitys;

        QList<QString> xEntitysNames = mNodes.values(xNodeID);
        for(int i = 0; i < xEntitysNames.size(); ++i)
        {
            Entity *xEntity;
            xEntity = mEntitys.value(xEntitysNames.at(i));
            xEntitys.push_back(xEntity);
        }

        return xEntitys;
    }

    void World::addComponent(QString xEntityName, IComponent *xComponent)
    {
        Entity *xEntity;
        // Add component
        if(mEntitys.contains(xEntityName))
        {
            xEntity = mEntitys.value(xEntityName);
            xEntity->mComponents.insert(xComponent->getType(), xComponent);
        }
        else
        {
            xEntity = new Entity(xEntityName);
            xEntity->mComponents.insert(xComponent->getType(), xComponent);
            mEntitys.insert(xEntityName, xEntity);
        }

        // Update nodes
        {
            qint32 xComType = xComponent->getType();

            // Get list of nodes names that have type of added component
            QList<qint32> xNodesIDs;
            xNodesIDs = mNodesLibrary.keys(xComType);

            QList<qint32>::iterator xNodesIDsIter;
            xNodesIDsIter = xNodesIDs.begin();
            for(; xNodesIDsIter != xNodesIDs.end(); ++xNodesIDsIter)
            {
                // Form list of needed components to be in node
                QList<qint32> xComTypes;
                xComTypes = mNodesLibrary.values((*xNodesIDsIter));
                xComTypes.removeAll(xComType);

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

    void World::removeComponent(QString xEntityName, IComponent *xComponent)
    {
        if(mEntitys.contains(xEntityName))
        {
            // Remove entity
            Entity *xEntity;
            xEntity = mEntitys.value(xEntityName);
            xEntity->mComponents.remove(xComponent->getType());

            // Remove node
            QList<qint32> xNodesIDs;
            xNodesIDs = mNodesLibrary.keys(xComponent->getType());

            QList<qint32>::iterator xNodesIDsIter;
            xNodesIDsIter = xNodesIDs.begin();
            for( ; xNodesIDsIter != xNodesIDs.end(); ++xNodesIDsIter)
                mNodes.remove((*xNodesIDsIter), xEntityName);
        }
    }

    void World::addSystem(qint32 xPriority, ISystem *xSystem)
    {
        mSystems.insert(xPriority, xSystem);
    }

    void World::changeSystemPriority(ISystem *xSystem, qint32 xNewPriority)
    {
        qint32 xPriority;
        xPriority = mSystems.key(xSystem);
        mSystems.remove(xPriority, xSystem);
        mSystems.insert(xNewPriority, xSystem);
    }

    void World::removeSystem(ISystem *xSystem)
    {
        qint32 xPriority;
        xPriority = mSystems.key(xSystem);
        mSystems.remove(xPriority, xSystem);
    }

    void World::injectUpdate(const float &xTimeSinceLastUpdate)
    {
        QMultiHash<qint32, ISystem*>::iterator xSystemsIter;
        xSystemsIter = mSystems.begin();
        while(xSystemsIter != mSystems.end())
        {
            (*xSystemsIter)->injectUpdate(xTimeSinceLastUpdate);
            ++xSystemsIter;
        }
    }

    void World::printInfo()
    {
        qDebug()<<"--------------------- World::printInfo ---------------------";
        {
            qDebug()<<"--- Nodes library:";
            QMultiHash<qint32, qint32>::iterator i = mNodesLibrary.begin();
            while(i != mNodesLibrary.end())
            {
                qDebug()<<"Node name:"<<i.key()<<"Component type:"<<i.value();
                ++i;
            }
        }
        {
            qDebug()<<"--- Enitys:";
            QHash<QString, Entity*>::iterator i = mEntitys.begin();
            while(i != mEntitys.end())
            {
                qDebug()<<"Name:"<<i.value()->getName();
                ++i;
            }
        }
        {
            qDebug()<<"--- Nodes:";
            QMultiHash<qint32, QString>::iterator i = mNodes.begin();
            while(i != mNodes.end())
            {
                qDebug()<<"Node name:"<<i.key()<<"Entity name:"<<i.value();
                ++i;
            }
        }
        /*{
            qDebug()<<"--- Systems:";
            QMultiHash<qint32, ISystem*>::iterator i = mSystems.begin();
            while(i != mSystems.end())
            {
                qDebug()<<"System name:"<<i.key()<<"Node type:"<<i.value()->getNodeType();
                ++i;
            }
        }*/
        qDebug()<<"-------------------------------------------------------------------";
    }
}
