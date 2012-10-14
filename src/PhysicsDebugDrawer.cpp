#include "PhysicsDebugDrawer.h"
#include "Utils.h"

namespace JGC
{
	PhysicsDebugDrawer::PhysicsDebugDrawer(Ogre::SceneManager *xSceneManager)
	{
		mFrameStarted = false;
		mContactPoints = &mContactPoints1;
		mLines = new Ogre::ManualObject("physics lines");
		mTriangles = new Ogre::ManualObject("physics triangles");
		mLines->setDynamic(true);
		mTriangles->setDynamic(true);
		//mLines->estimateVertexCount( 100000 );
		//mLines->estimateIndexCount( 0 );

		xSceneManager->getRootSceneNode()->attachObject(mLines);
		xSceneManager->getRootSceneNode()->attachObject(mTriangles);

		static const char *xMatName = "OgreBulletCollisionsDebugDefault";
		Ogre::MaterialPtr xMaterialPtr = Ogre::MaterialManager::getSingleton().getDefaultSettings()->clone(xMatName);
		xMaterialPtr->setReceiveShadows(false);
		xMaterialPtr->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
		xMaterialPtr->setDepthBias(0.1, 0);
		Ogre::TextureUnitState *xTUS = xMaterialPtr->getTechnique(0)->getPass(0)->createTextureUnitState();
		xTUS->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_DIFFUSE);
		xMaterialPtr->getTechnique(0)->setLightingEnabled(false);
		//xMaterialPtr->getTechnique(0)->setSelfIllumination(Ogre::ColourValue::White); 

		mLines->begin(xMatName, Ogre::RenderOperation::OT_LINE_LIST);
		mLines->position(Ogre::Vector3::ZERO);
		mLines->colour(Ogre::ColourValue::Blue);
		mLines->position(Ogre::Vector3::ZERO);
		mLines->colour(Ogre::ColourValue::Blue);

		mTriangles->begin(xMatName, Ogre::RenderOperation::OT_TRIANGLE_LIST);
		mTriangles->position(Ogre::Vector3::ZERO);
		mTriangles->colour(Ogre::ColourValue::Blue);
		mTriangles->position(Ogre::Vector3::ZERO);
		mTriangles->colour(Ogre::ColourValue::Blue);
		mTriangles->position(Ogre::Vector3::ZERO);
		mTriangles->colour(Ogre::ColourValue::Blue);

		mDebugModes = btIDebugDraw::DBG_DrawWireframe;
		//Ogre::Root::getSingleton().addFrameListener(this);
	}

	PhysicsDebugDrawer::~PhysicsDebugDrawer()
	{
		//Ogre::Root::getSingleton().removeFrameListener(this);
		delete mLines;
		delete mTriangles;
	}

	void PhysicsDebugDrawer::drawLine(const btVector3 &xFrom, const btVector3 &xTo, const btVector3 &xColor)
	{
		Ogre::ColourValue xColour(xColor.getX(), xColor.getY(), xColor.getZ());  
		xColour.saturate();
		mLines->position(Utils::toOgreVector3(xFrom));
		mLines->colour(xColour);
		mLines->position(Utils::toOgreVector3(xTo));
		mLines->colour(xColour);
	}

	void PhysicsDebugDrawer::drawTriangle(const btVector3 &xDot1, const btVector3 &xDot2, const btVector3 &xDot3, const btVector3 &xColor, btScalar xAlpha)
	{
		Ogre::ColourValue xColour(xColor.getX(), xColor.getY(), xColor.getZ(), xAlpha);  
		xColour.saturate();
		mTriangles->position(Utils::toOgreVector3(xDot1));
		mTriangles->colour(xColour);
		mTriangles->position( Utils::toOgreVector3(xDot2));
		mTriangles->colour(xColour);
		mTriangles->position(Utils::toOgreVector3(xDot3));
		mTriangles->colour(xColour);
	}

	void PhysicsDebugDrawer::drawContactPoint(const btVector3 &xPointOnB, const btVector3 &xNormalOnB, btScalar xDistance, int xLifeTime, const btVector3 &xColor)
	{
		mContactPoints->resize(mContactPoints->size() + 1);
		ContactPoint p = mContactPoints->back();
		p.xFrom = Utils::toOgreVector3(xPointOnB);
		p.xTo = p.xFrom + Utils::toOgreVector3(xNormalOnB) * xDistance;
		p.xDieTime = Ogre::Root::getSingleton().getTimer()->getMilliseconds() + xLifeTime;
		p.xColor.r = xColor.x();
		p.xColor.g = xColor.y();
		p.xColor.b = xColor.z();
	}

	bool PhysicsDebugDrawer::frameStarted(const float& xTimeSinceLastFrame)
	{
		mFrameStarted = true;
		size_t xNow = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
		std::vector< ContactPoint > *newCP = mContactPoints == &mContactPoints1 ? &mContactPoints2 : &mContactPoints1;
		for (std::vector< ContactPoint >::iterator i = mContactPoints->begin(); i < mContactPoints->end(); i++)
		{
			ContactPoint &xContactPoint = *i;
			mLines->position(xContactPoint.xFrom);
			mLines->colour(xContactPoint.xColor);
			mLines->position(xContactPoint.xTo);
			if (xNow <= xContactPoint.xDieTime) newCP->push_back(xContactPoint);
		}
		mContactPoints->clear();
		mContactPoints = newCP;

		mLines->end();
		mTriangles->end();

		return true;
	}

	bool PhysicsDebugDrawer::frameEnded(const float& xTimeSinceLastFrame)
	{
		if(mFrameStarted == true)
		{
			mLines->beginUpdate(0);
			mTriangles->beginUpdate(0);
			mFrameStarted = false;
		}

		return true;
	}

	void PhysicsDebugDrawer::reportErrorWarning(const char *xWarningString)
	{
		Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(xWarningString);
	}

	void PhysicsDebugDrawer::draw3dText(const btVector3 &xLocation, const char *xTextString)
	{

	}

	void PhysicsDebugDrawer::setDebugMode(int xDebugMode)
	{
		mDebugModes = (DebugDrawModes) xDebugMode;
	}

	int PhysicsDebugDrawer::getDebugMode() const
	{
		return mDebugModes;
	}
}