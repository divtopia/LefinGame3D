//v0.1 copyright Comine.com 20070403T14
#ifndef MGameKayboardControl_h
#define MGameKeyboardControl_h

////////////////////////////////////////////////////
#ifdef MLIBRARY
#include "MSGAEngine3D.h"
#else
#include "MSGA3D.h"
#endif // MLIBRARY

////////////////////////////////////////////////////
bool MGameKeyboardControlCreate(MSGAEngine3D &engine,int nodeid);
bool MGameKeyboardControlDestroy(void);


#endif // MGameKeyboardControl_h