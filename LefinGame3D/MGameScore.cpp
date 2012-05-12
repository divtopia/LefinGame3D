//v0.1 copyright Comine.com 20070321W20
#include <stdio.h>
#include <string.h>
#include "MSGA3D.h"
#include "MGameScore.h"

//*************************************************************
//**   Module Elements
//*************************************************************
const static int GFontID=20070321;
const static int GSoundID=20070321;
static MSGAEngine3D *GRefEngine=0;
static int GGameScoreOne=0;					// Hold the Game Score for Player
static char GTitle[100]="MGameScore";
static int GTitlePosX=0;
static int GTitlePosY=0;
static int GTitleSize=30;
static int GScorePosX=0;
static int GScorePosY=30;
static int GScoreSize=20;

///////////////////////////////////////////////////////////////
static bool GScoreRenderer(MSGAEngine3D &engine)
	{
	GRefEngine->WindowWrite(GFontID,GTitlePosX,GTitlePosY,GTitleSize,true,"%s",GTitle);
	GRefEngine->WindowWrite(GFontID,GScorePosX,GScorePosY,GScoreSize,true
			,"Score: %05d",GGameScoreOne);
	return true;
	}


///////////////////////////////////////////////////////////////
static bool GAddTask(MSGAEngine3D &engine,int value,void *)
	{
	MGameScoreAdd(value);
	return true;
	}


//*************************************************************
//**   Module Interface Functions
//*************************************************************
bool MGameScoreCreate(MSGAEngine3D &engine,const char title[])
	{
	MGameScoreDestroy();
	GRefEngine=&engine;

	// Copy String to Array
	strcpy(GTitle,title);

	// Load up the Game Font
	GRefEngine->FontLoad(GFontID,"MGameScore.bmp",3,15
			," .0123456789:<>ABCDEFGHIJKLMNOPQRSTUVWXYZ    ");

	// Load up the Game Sound
	GRefEngine->SoundLoad(GSoundID,"MGameScore.wav");

	// Init Game Scores
	GGameScoreOne=0;

	GRefEngine->WindowRenderer(0,GScoreRenderer);
	return true;
	}


///////////////////////////////////////////////////////////////
bool MGameScoreDestroy(void)
	{
	if(GRefEngine==0)
		{ return true; }

	// Remove Renderer
	GRefEngine->WindowRenderer(0);

	// Unload the SOund
	GRefEngine->SoundUnload(GSoundID);

	// Unload the font
	GRefEngine->FontUnload(GFontID);
	GRefEngine=0;
	
	return true;
	}


//////////////////////////////////////////////////////////////
bool MGameScoreAdd(int value)
	{
	if(GRefEngine==0)
		{
		printf("MGameScore: Engine not initialized(%d)\n",__LINE__);
		return false;
		}

	// Make Sure that value is positive
	if(value<=0)
		{
		printf("MGameScore: Got bad arguments(%d)\n",__LINE__);
		return false;
		}

	// Play Once
	GRefEngine->SoundPlay(GSoundID,false);

	GGameScoreOne = GGameScoreOne + value;
	return true;
	}


//////////////////////////////////////////////////////////////
bool MGameScoreAddSlowly(int value,int times)
	{
	if(GRefEngine==0)
		{
		printf("Module MGameScore is not Initialized\n");
		return false;
		}

	if(value<=0 || times<=0)
		{
		printf("MGameScore: Got bad arguments(%d)\n",__LINE__);
		return false;
		}

	int i;
	for(i=0;i<times;++i)
		{
		int taskid=GRefEngine->TaskGetUnusedID();
		GRefEngine->TaskLoad(taskid,100*i,false,GAddTask,value);
		}

	return true;
	}


//////////////////////////////////////////////////////////////
int MGameScoreGetScore(void)
	{
	if(GRefEngine==NULL)
		{
		printf("MGameScore: Engine not initialized(%d)\n",__LINE__);
		return 0;
		}

	return GGameScoreOne;
	}
