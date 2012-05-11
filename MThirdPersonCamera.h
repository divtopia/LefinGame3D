//v0.1 copyright Comine.com 20070403T11
#ifndef MThirdPersonCamera_h
#define MThirdPersonCamera_h

///////////////////////////////////////////////////////
#ifdef MLIBRARY
#include "MSGAEngine3D.h"
#else
#include "MSGA3D.h"
#endif // MLIBRARY

//////////////////////////////////////////////////////
bool MThirdPersonCameraCreate(MSGAEngine3D &engine,int objectid
		,float height=50,float back=100,float fronttarget=200);
bool MThirdPersonCameraDestroy(void);
int MThirdPersonCameraGetCameraNode(void);

#endif // MThirdPersonCamera