#include "EntitySystem.h"
#include <QDebug>

namespace JGC
{
    EntitySystem* EntitySystem::mInstance = 0;

    void EntitySystem::initialize()
    {
        mInstance = new EntitySystem();
    }

    void EntitySystem::shutdown()
    {
        delete mInstance;
        mInstance = 0;
    }

    EntitySystem* EntitySystem::instance()
    {
        return mInstance;
    }

    EntitySystem::EntitySystem()
    {

    }

    EntitySystem::~EntitySystem()
    {

    }

    void EntitySystem::addComponentToNode(QString xNodeName, QString xComponentType)
    {
        mNodesLibrary.insert(xNodeName, xComponentType);
    }

    void EntitySystem::removeComponentFromNode(QString xNodeName, QString xComponentType)
    {
        mNodesLibrary.remove(xNodeName, xComponentType);
    }

    void EntitySystem::removeEntity(QString xName)
    {
        mEntitys.remove(xName);
    }

    void EntitySystem::addComponent(QString xEntityName, IComponent *xComponent)
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

    void EntitySystem::removeComponent(QString xEntityName, IComponent *xComponent)
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

    void EntitySystem::addSystem(qint32 xPriority, ISystem *xSystem)
    {
        mSystems.insert(xPriority, xSystem);
    }

    void EntitySystem::changeSystemPriority(ISystem *xSystem, qint32 xNewPriority)
    {
        qint32 xPriority;
        xPriority = mSystems.key(xSystem);
        mSystems.remove(xPriority, xSystem);
        mSystems.insert(xNewPriority, xSystem);
    }

    void EntitySystem::removeSystem(ISystem *xSystem)
    {
        qint32 xPriority;
        xPriority = mSystems.key(xSystem);
        mSystems.remove(xPriority, xSystem);
    }

    void EntitySystem::injectUpdate(const float &xTimeSinceLastUpdate)
    {
        QMultiHash<qint32, ISystem*>::iterator xSystemsIter;
        xSystemsIter = mSystems.begin();
        while(xSystemsIter != mSystems.end())
        {
            QVector<Entity*> xEntitys;

            QString xNodeType;
            xNodeType = (*xSystemsIter)->getNodeType();
            QList<QString> xEntitysNames = mNodes.values(xNodeType);

            for(int i = 0; i < xEntitysNames.size(); ++i)
            {
                Entity *xEntity;
                xEntity = mEntitys.value(xEntitysNames.at(i));
                xEntitys.push_back(xEntity);
            }

            if(xEntitys.size() > 0)
                (*xSystemsIter)->proceedEntitys(xEntitys, xTimeSinceLastUpdate);

            ++xSystemsIter;
        }
    }

    void EntitySystem::printInfo()
    {
        qDebug()<<"--------------------- EntitySystem::printInfo ---------------------";
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
        {
            qDebug()<<"--- Systems:";
            QMultiHash<qint32, ISystem*>::iterator i = mSystems.begin();
            while(i != mSystems.end())
            {
                qDebug()<<"System name:"<<i.key()<<"Node type:"<<i.value()->getNodeType();
                ++i;
            }
        }
        qDebug()<<"-------------------------------------------------------------------";
    }
}
