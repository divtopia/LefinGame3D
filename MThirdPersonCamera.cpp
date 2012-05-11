//v0.1 copyright Comine.com 20070403T11
#include <stdio.h>

///////////////////////////////////////////////////////
#ifdef MLIBRARY
#include "MSGAEngine3D.h"
#else
#include "MSGA3D.h"
#endif // MLIBRARY

#include "MThirdPersonCamera.h"

//********************************************
//** Module Elements
//********************************************
static const int GGameLoopActionID=-123456;
static const int GCameraID=-123456;
static MSGAEngine3D *GRefEngine=NULL;
static float GCameraHeight=0;
static float GCameraBack=0;
static float GTargetFront=0;
static int GGameObjectID=0;
static int GGameObjectNodeID=0;

////////////////////////////////////////////////
static bool GGameLoopAction(MSGAEngine3D &engine)
	{
	// Find Front Vector
	float fx,fy,fz;
	engine.ObjectGetFront(GGameObjectID,fx,fy,fz);

	// Find Up Vector
	float ux,uy,uz;
	engine.ObjectGetUp(GGameObjectID,ux,uy,uz);

	// Find Object Position
	float posx,posy,posz;
	engine.ObjectGetPosition(GGameObjectNodeID,posx,posy,posz);
		
	// Move Camera to behind object
	GRefEngine->NodeSetPosition(GCameraID
			,posx-GCameraBack*fx+GCameraHeight*ux
			,posy-GCameraBack*fy+GCameraHeight*uy
			,posz-GCameraBack*fz+GCameraHeight*uz);


	// Point Camera towards the target
	engine.NodeCameraLookAt(GCameraID
			,posx+GTargetFront*fx
			,posy+GTargetFront*fy
			,posz+GTargetFront*fz);

	return true;
	}


//********************************************
//** External Functions
//********************************************
bool MThirdPersonCameraCreate(MSGAEngine3D &engine,int objectid
		,float height,float back,float fronttarget)
	{
	MThirdPersonCameraDestroy();
	GRefEngine=&engine;

	// Store info into module private members
	GCameraHeight=height;
	GCameraBack=back;
	GTargetFront=fronttarget;
	GGameObjectID=objectid;

	// Get Node ID for Object
	GGameObjectNodeID=GRefEngine->ObjectGetNode(GGameObjectID);

	// Load up Camera
	GRefEngine->NodeLoadCamera(GCameraID);

	// Load Game Loop Action
	if(GRefEngine->GameLoopActionLoad(GGameLoopActionID,GGameLoopAction)==false)
		{
		printf("**MThirdPersonCamera: Unable to load game loop action(%d)\n",__LINE__);
		MThirdPersonCameraDestroy();
		return false;
		}

	return true;
	}


/////////////////////////////////////////////
bool MThirdPersonCameraDestroy(void)
	{
	if(GRefEngine==NULL) { return true; }
	GRefEngine->GameLoopActionUnload(GGameLoopActionID);
	GRefEngine->NodeUnload(GCameraID);
	GRefEngine=NULL;
	return true;
	}


/////////////////////////////////////////////
int MThirdPersonCameraGetCameraNode(void)
	{  return GCameraID;  }

