//v0.1 copyright Comine.com 20070321W20
#ifndef MGameScore_h
#define MGameScore_h

///////////////////////////////////////////////////////////////
#include "MSGA3D.h"

///////////////////////////////////////////////////////////////
bool MGameScoreCreate(MSGAEngine3D &engine,const char title[]="Game One");
bool MGameScoreDestroy(void);						// Destroy MGameScore
bool MGameScoreAdd(int value);						// Add to Score
bool MGameScoreAddSlowly(int value,int times);		// Add to Score
int MGameScoreGetScore(void);						// Get Current Score


#endif // MGameScore_h
