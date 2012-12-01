#ifndef LEVELSSYSTEM_H
#define LEVELSSYSTEM_H

#include "World.h"

#include <QHash>
#include <QString>

namespace JGC
{
    class WorldsSystem
	{
	private:
        static WorldsSystem* mInstance;

        // world name - world pointer
        QHash<QString, World*> mWorlds;

        World *mActiveWorld;

	public:
        static bool initialize();
		static void shutdown();
        static WorldsSystem* instance();

	private:
        WorldsSystem();
        ~WorldsSystem();
		bool init();

	public:
        void addWorld(World *xWorld);
        // if world not found return 0
        World* getWorld(QString xWorldName);
        void removeWorld(QString xWorldName);

        void loadWorld(QString xWorldName);

        void setActiveWorld(QString xWorldName);
        World* getActiveWorld();

        void injectUpdate(const float& xTimeSinceLastUpdate);
	};
}

#endif
