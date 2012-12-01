#include "World.h"

#include <QDebug>

namespace JGC
{
    World::World(QString xWorldName)
    {
        mWorldName = xWorldName;
    }

    World::~World()
    {

    }

    QString World::getName()
    {
        return mWorldName;
    }

    void World::addComponentToNode(QString xNodeName, QString xComponentType)
    {
        mNodesLibrary.insert(xNodeName, xComponentType);
    }

    void World::removeComponentFromNode(QString xNodeName, QString xComponentType)
    {
        mNodesLibrary.remove(xNodeName, xComponentType);
    }

    void World::removeEntity(QString xName)
    {
        mEntitys.remove(xName);
    }

    QVector<Entity*> World::getEntitysInNode(QString xNodeName)
    {
        QVector<Entity*> xEntitys;

        QList<QString> xEntitysNames = mNodes.values(xNodeName);
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
            QString xComType = xComponent->getType();

            // Get list of nodes names that have type of added component
            QList<QString> xNodesNames;
            xNodesNames = mNodesLibrary.keys(xComType);

            QList<QString>::iterator xNodesNamesIter;
            xNodesNamesIter = xNodesNames.begin();
            for(; xNodesNamesIter != xNodesNames.end(); ++xNodesNamesIter)
            {
                // Form list of needed components to be in node
                QList<QString> xComTypes;
                xComTypes = mNodesLibrary.values((*xNodesNamesIter));
                xComTypes.removeAll(xComType);

                // Check is entity have all components to be in current node
                bool xHaveAllComponents = true;

                QList<QString>::iterator xComTypesIter;
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
                    mNodes.insert((*xNodesNamesIter), xEntityName);
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
            QList<QString> xNodesNames;
            xNodesNames = mNodesLibrary.keys(xComponent->getType());

            QList<QString>::iterator xNodesNamesIter;
            xNodesNamesIter = xNodesNames.begin();
            for( ; xNodesNamesIter != xNodesNames.end(); ++xNodesNamesIter)
                mNodes.remove((*xNodesNamesIter), xEntityName);
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
            QMultiHash<QString, QString>::iterator i = mNodesLibrary.begin();
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
            QMultiHash<QString, QString>::iterator i = mNodes.begin();
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
