#include "WorldsSystem.h"

namespace JGC
{
    WorldsSystem* WorldsSystem::mInstance = 0;

    bool WorldsSystem::initialize()
	{
        mInstance = new WorldsSystem();
		return mInstance->init();
	}

    void WorldsSystem::shutdown()
	{
		delete mInstance;
		mInstance = 0;
	}

    WorldsSystem* WorldsSystem::instance()
	{
		return mInstance;
	}

    WorldsSystem::WorldsSystem()
	{
        mActiveWorld = 0;
	}

    WorldsSystem::~WorldsSystem()
	{
        mActiveWorld = 0;
	}

    bool WorldsSystem::init()
	{
		return true;
	}

    void WorldsSystem::addWorld(World *xWorld)
    {
        mWorlds.insert(xWorld->getName(), xWorld);
    }

    World* WorldsSystem::getWorld(QString xWorldName)
    {
        return mWorlds.value(xWorldName, 0);
    }

    void WorldsSystem::removeWorld(QString xWorldName)
    {
        mWorlds.remove(xWorldName);
    }

    void WorldsSystem::loadWorld(QString xWorldName)
    {
        if(mWorlds.contains(xWorldName))
            mWorlds.value(xWorldName)->load();
    }

    void WorldsSystem::setActiveWorld(QString xWorldName)
    {
        if(mActiveWorld != 0)
            mActiveWorld->exit();

        if(mWorlds.contains(xWorldName))
        {
            mActiveWorld = mWorlds.value(xWorldName);
            mActiveWorld->enter();
        }
    }

    World* WorldsSystem::getActiveWorld()
    {
        return mActiveWorld;
    }

    void WorldsSystem::injectUpdate(const float& xTimeSinceLastUpdate)
    {
        if(mActiveWorld != 0)
            mActiveWorld->injectUpdate(xTimeSinceLastUpdate);
    }
}
