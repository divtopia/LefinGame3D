//v0.1 copyright Comine.com 20070405R16
#include <stdio.h>

///////////////////////////////////////////////
#ifdef MLIBRARY
#include "MSGAEngine3D.h"
#else
#include "MSGA3D.h"
#endif // MLIBRARY

///////////////////////////////////////////////
#include "MGameKeyboardControl.h"

//*************************************************************
//**   Module Elements
//*************************************************************
static const int GModuleID=-12345623;
static const int GKeyboardPollTime=50;


////////////////////////////////////////////////////
static MSGAEngine3D *GRefEngine=NULL;
static int GObjectID=0;
static int GNodeID=0;
static float GMaxVelocity=200;
static EMD2_ANIMATION_TYPE GCurrentAnimation=EMAT_STAND;


////////////////////////////////////////////////////////////
static bool GOnTask(MSGAEngine3D &engine,int,void *)
	{
	float vel;
	engine.ObjectGetFrontVelocity(GObjectID,vel);

	// Check Left Arraow Key
	if(engine.KeyIsPressed(MSGA3D_KEY_LEFT,GKeyboardPollTime)==true)
		{
		engine.ObjectAddRotation(GObjectID,0,-5,0);
		engine.ObjectSetFrontVelocity(GObjectID,GMaxVelocity);
		
		if(GCurrentAnimation==EMAT_STAND)
			{
			GCurrentAnimation=EMAT_RUN;
			engine.NodeAnimationMD2(GNodeID,EMAT_RUN);
			}
		}

	// Check Right Arraow Key
	if(engine.KeyIsPressed(MSGA3D_KEY_RIGHT,GKeyboardPollTime)==true)
		{
		engine.ObjectAddRotation(GObjectID,0,+5,0);
		engine.ObjectSetFrontVelocity(GObjectID,GMaxVelocity);

		if(GCurrentAnimation==EMAT_STAND)
			{
			GCurrentAnimation=EMAT_RUN;
			engine.NodeAnimationMD2(GNodeID,EMAT_RUN);
			}
		}

	// Check Up Arrow Key
	if(engine.KeyIsPressed(MSGA3D_KEY_UP,GKeyboardPollTime)==true)
		{
		engine.ObjectSetFrontVelocity(GObjectID,GMaxVelocity);

		if(GCurrentAnimation!=EMAT_RUN)
			{
			GCurrentAnimation=EMAT_RUN;
			engine.NodeAnimationMD2(GNodeID,EMAT_RUN);
			}
		}

	// Check Right Down Key
	if(engine.KeyIsPressed(MSGA3D_KEY_DOWN,GKeyboardPollTime)==true)
		{
		if(vel>0)
			{
			engine.ObjectSetFrontVelocity(GObjectID,0);
			GCurrentAnimation=EMAT_STAND;
			engine.NodeAnimationMD2(GNodeID,EMAT_STAND);
			}
		}

	engine.ObjectPrint(GObjectID,"GameKeyboardControl:",false);
	return true;
	}


//*************************************************************
//**   Public Functions
//*************************************************************
bool MGameKeyboardControlCreate(MSGAEngine3D &engine,int objectid)
	{
	MGameKeyboardControlDestroy();

	// Init Members
	GRefEngine=&engine;
	GObjectID=objectid;
	GNodeID=GRefEngine->ObjectGetNode(GObjectID);

	// load up the key board handler
	engine.TaskLoad(GModuleID,GKeyboardPollTime,true,GOnTask);
	engine.NodeAnimationMD2(GNodeID,EMAT_STAND);
	return true;
	}


/////////////////////////////////////////////////////////////
bool MGameKeyboardControlDestroy(void)
	{
	if(GRefEngine==NULL) { return true; }
	GRefEngine->ObjectOnStop(GNodeID,NULL);
	GRefEngine->TaskUnload(GModuleID);
	GRefEngine=NULL;
	return true;
	}
