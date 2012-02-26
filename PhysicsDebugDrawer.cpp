#include "PhysicsDebugDrawer.h"
#include "Utils.h"
using namespace JGC;

PhysicsDebugDrawer::PhysicsDebugDrawer( Ogre::SceneManager *scm )
{
	mFrameStarted = false;
	mContactPoints = &mContactPoints1;
	mLines = new Ogre::ManualObject("physics lines");
	mTriangles = new Ogre::ManualObject("physics triangles");
	mLines->setDynamic(true);
	mTriangles->setDynamic(true);
	//mLines->estimateVertexCount( 100000 );
	//mLines->estimateIndexCount( 0 );

	scm->getRootSceneNode()->attachObject( mLines );
	scm->getRootSceneNode()->attachObject( mTriangles );

	static const char * matName = "OgreBulletCollisionsDebugDefault";
	Ogre::MaterialPtr mtl = Ogre::MaterialManager::getSingleton().getDefaultSettings()->clone(matName);
	mtl->setReceiveShadows(false);
	mtl->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
	mtl->setDepthBias( 0.1, 0 );
	Ogre::TextureUnitState * tu = mtl->getTechnique(0)->getPass(0)->createTextureUnitState();
	tu->setColourOperationEx( Ogre::LBX_SOURCE1, Ogre::LBS_DIFFUSE );
	mtl->getTechnique(0)->setLightingEnabled(true);
	mtl->getTechnique(0)->setSelfIllumination( Ogre::ColourValue::White ); 

	mLines->begin( matName, Ogre::RenderOperation::OT_LINE_LIST );
	mLines->position( Ogre::Vector3::ZERO );
	mLines->colour( Ogre::ColourValue::Blue );
	mLines->position( Ogre::Vector3::ZERO );
	mLines->colour( Ogre::ColourValue::Blue );

	mTriangles->begin( matName, Ogre::RenderOperation::OT_TRIANGLE_LIST );
	mTriangles->position(Ogre::Vector3::ZERO );
	mTriangles->colour(Ogre::ColourValue::Blue );
	mTriangles->position( Ogre::Vector3::ZERO );
	mTriangles->colour( Ogre::ColourValue::Blue );
	mTriangles->position( Ogre::Vector3::ZERO );
	mTriangles->colour( Ogre::ColourValue::Blue );

	mDebugModes = btIDebugDraw::DBG_DrawWireframe;
	//Ogre::Root::getSingleton().addFrameListener(this);
}

PhysicsDebugDrawer::~PhysicsDebugDrawer()
{
	//Ogre::Root::getSingleton().removeFrameListener(this);
	delete mLines;
	delete mTriangles;
}

void PhysicsDebugDrawer::drawLine( const btVector3 &from, const btVector3 &to, const btVector3 &color )
{
	Ogre::ColourValue c( color.getX(), color.getY(), color.getZ() );  
	c.saturate();
	mLines->position( JGC::toOgreVector3(from) );
	mLines->colour( c );
	mLines->position( JGC::toOgreVector3(to) );
	mLines->colour( c );
}

void PhysicsDebugDrawer::drawTriangle( const btVector3 &v0, const btVector3 &v1, const btVector3 &v2, const btVector3 &color, btScalar alpha )
{
	Ogre::ColourValue c( color.getX(), color.getY(), color.getZ(), alpha );  
	c.saturate();
	mTriangles->position( JGC::toOgreVector3(v0) );
	mTriangles->colour( c );
	mTriangles->position( JGC::toOgreVector3(v1) );
	mTriangles->colour( c );
	mTriangles->position( JGC::toOgreVector3(v2) );
	mTriangles->colour( c );
}

void PhysicsDebugDrawer::drawContactPoint( const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color )
{
	mContactPoints->resize( mContactPoints->size() + 1 );
	ContactPoint p = mContactPoints->back();
	p.from = JGC::toOgreVector3( PointOnB );
	p.to = p.from + JGC::toOgreVector3( normalOnB ) * distance;
	p.dieTime = Ogre::Root::getSingleton().getTimer()->getMilliseconds() + lifeTime;
	p.color.r = color.x();
	p.color.g = color.y();
	p.color.b = color.z();
}

bool PhysicsDebugDrawer::frameStarted( const Ogre::FrameEvent& evt )
{
	mFrameStarted = true;
	size_t xNow = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
	std::vector< ContactPoint > *newCP = mContactPoints == &mContactPoints1 ? &mContactPoints2 : &mContactPoints1;
	for ( std::vector< ContactPoint >::iterator i = mContactPoints->begin(); i < mContactPoints->end(); i++ )
	{
		ContactPoint &cp = *i;
		mLines->position( cp.from );
		mLines->colour( cp.color );
		mLines->position( cp.to );
		if ( xNow <= cp.dieTime  ) newCP->push_back( cp );
	}
	mContactPoints->clear();
	mContactPoints = newCP;

	mLines->end();
	mTriangles->end();

	return true;
}

bool PhysicsDebugDrawer::frameEnded( const Ogre::FrameEvent& evt )
{
	if(mFrameStarted == true)
	{
		mLines->beginUpdate(0);
		mTriangles->beginUpdate(0);
		mFrameStarted = false;
	}

	return true;
}

void PhysicsDebugDrawer::reportErrorWarning( const char *warningString )
{
	Ogre::LogManager::getSingleton().getDefaultLog()->logMessage( warningString );
}

void PhysicsDebugDrawer::draw3dText( const btVector3 &location, const char *textString )
{

}

void PhysicsDebugDrawer::setDebugMode( int debugMode )
{
	mDebugModes = (DebugDrawModes) debugMode;
}

int PhysicsDebugDrawer::getDebugMode() const
{
	return mDebugModes;
}