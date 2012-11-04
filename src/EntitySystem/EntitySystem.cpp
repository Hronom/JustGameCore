#include "EntitySystem.h"
#include <QDebug>

namespace JGC
{
    EntitySystem::EntitySystem()
    {

    }

    EntitySystem::~EntitySystem()
    {

    }

    void EntitySystem::update()
    {
        QMultiMap<qint32, ISystem*>::iterator xSystemsIter;
        xSystemsIter = mSystems.begin();
        for( ; xSystemsIter != mSystems.end(); ++xSystemsIter)
        {
            QMultiMap<QString, IComponent*> xEntitys;

            QString xNodeType;
            xNodeType = (*xSystemsIter)->getNodeType();
            QList<QString> xEntitysNames = mNodes.values(xNodeType);

            QList<QString>::iterator xENIter;
            xENIter = xEntitysNames.begin();
            for( ; xENIter != xEntitysNames.end(); ++xENIter)
            {
                QList<IComponent*> xComponents;
                xComponents = mEntitys.values((*xENIter));

                QList<IComponent*>::iterator xCompIter;
                xCompIter = xComponents.begin();
                for( ; xCompIter != xComponents.end(); ++xCompIter)
                {
                    xEntitys.insert((*xENIter), (*xCompIter));
                }
            }

            if(xEntitys.size() > 0)
                (*xSystemsIter)->proceedEntitys(xEntitys);
        }
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
        QList<QString> xNodesNames;
        xNodesNames = mNodesLibrary.keys(xComponent->getType());

        QList<QString>::iterator xNodesIter;
        xNodesIter = xNodesNames.begin();
        for( ; xNodesIter != xNodesNames.end(); ++xNodesIter)
        {
            QList<QString> xComponentsTypes;
            xComponentsTypes = mNodesLibrary.values((*xNodesIter));
            xComponentsTypes.removeAll(xComponent->getType());

            QList<IComponent*> xEntityComponents;
            xEntityComponents = mEntitys.values(xEntityName);

            bool xHaveAllComponents = true;
            QList<QString>::iterator xTypesIter;
            xTypesIter = xComponentsTypes.begin();
            for( ; xTypesIter != xComponentsTypes.end(); ++xTypesIter)
            {
                bool xHaveComponent = false;

                QList<IComponent*>::iterator xComponIter;
                xComponIter = xEntityComponents.begin();
                for( ; xComponIter != xEntityComponents.end(); ++xComponIter)
                {
                    if((*xComponIter)->getType() == (*xTypesIter))
                    {
                        xHaveComponent = true;
                        break;
                    }
                }

                if(xHaveComponent == false)
                {
                    xHaveAllComponents = false;
                    break;
                }
            }

            if(xHaveAllComponents == true)
                mNodes.insert((*xNodesIter), xEntityName);
        }

        mEntitys.insert(xEntityName, xComponent);
    }

    void EntitySystem::removeComponent(QString xEntityName, IComponent *xComponent)
    {
        QList<QString> xNodesNames;
        xNodesNames = mNodesLibrary.keys(xComponent->getType());

        QList<QString>::iterator xNodesIter;
        xNodesIter = xNodesNames.begin();
        for( ; xNodesIter != xNodesNames.end(); ++xNodesIter)
        {
            mNodes.remove((*xNodesIter), xEntityName);
        }

        mEntitys.remove(xEntityName, xComponent);
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

    void EntitySystem::printInfo()
    {
        {
            qDebug()<<"--- Nodes library:";
            QMultiMap<QString, QString>::iterator i = mNodesLibrary.begin();
            while(i != mNodesLibrary.end())
            {
                qDebug()<<i.key()<<i.value();
                ++i;
            }
        }
        {
            qDebug()<<"--- Enitys:";
            QMultiMap<QString, IComponent*>::iterator i = mEntitys.begin();
            while(i != mEntitys.end())
            {
                qDebug()<<i.key()<<i.value()->getType();
                ++i;
            }
        }
        {
            qDebug()<<"--- Nodes:";
            QMultiMap<QString, QString>::iterator i = mNodes.begin();
            while(i != mNodes.end())
            {
                qDebug()<<i.key()<<i.value();
                ++i;
            }
        }
        {
            qDebug()<<"--- Systems:";
            QMultiMap<qint32, ISystem*>::iterator i = mSystems.begin();
            while(i != mSystems.end())
            {
                qDebug()<<i.key()<<i.value()->getNodeType();
                ++i;
            }
        }
    }
}
