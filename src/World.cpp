#include "World.h"

namespace JGC
{
    World::World(QString xWorldName)
    {
        mWorldName = xWorldName;
        mWorldActive = false;
    }

    World::~World()
    {
        qDebug()<<"Destroying world:"<<mWorldName;
    }

    QString World::getName()
    {
        return mWorldName;
    }

    void World::setWorldActive(bool xWorldActive)
    {
        mWorldActive = xWorldActive;
    }

    Entity* World::getEntity(QString xName)
    {
        return mEntitys.value(xName, 0);
    }

    QList<Entity*> World::getAllEntitys()
    {
        return mEntitys.values();
    }

    void World::removeEntity(QString xName)
    {
        Entity *xEntity;
        xEntity = mEntitys.take(xName);
        QList<qint32> xKeys;
        xKeys = mNodes.keys(xEntity);
        while(!xKeys.isEmpty())
             mNodes.remove(xKeys.takeFirst(), xEntity);
    }

    QList<Entity*> World::getEntitysInNode(qint32 xNodeID)
    {
        QList<Entity*> xEntitys;
        xEntitys = mNodes.values(xNodeID);
        return xEntitys;
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

    QList<ISystem*> World::getAllSystems()
    {
        return mSystems.values();
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
            if(mWorldActive)
            {
                (*xSystemsIter)->injectUpdate(xTimeSinceLastUpdate);
                if(mSystems.size() > 0)
                    ++xSystemsIter;
            }
            else
                break;
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
            QMultiHash<qint32, Entity*>::iterator i = mNodes.begin();
            while(i != mNodes.end())
            {
                qDebug()<<"Node name:"<<i.key()<<"Entity name:"<<i.value()->getName();
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
