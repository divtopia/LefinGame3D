#include "MSGA3D.h"
#include "MThirdPersonCamera.h"
#include "MGameScore.h"
#include "MGameKeyboardControl.h"

///////////////////////////////////////
#define		TEX_FLOOR			2000
#define		TEX_BUSH1			2001                     
#define		TEX_BUSH2			2002                       
#define		TEX_TREE1			2003                       
#define		TEX_TREE2			2004
#define		TEX_ANIM_CHICKEN	2005 
#define		TEX_UP				2006
#define		TEX_DN				2007
#define		TEX_BK				2008
#define		TEX_FT				2009
#define		TEX_LF				2010
#define		TEX_RT				2011

#define		MESH_RECT			2012
#define		MESH_PRIZE			2013
#define		MESH_BOX			2014
#define		MESH_ANIM			2015              
#define		MESH_BUSH1			2016                    
#define		MESH_BUSH2			2017                      
#define		MESH_TREE1			2018              
#define		MESH_BSP			2019

#define		NODE_FLOOR			2020
#define		NODE_BOX			2021
#define		NODE_SKYBOX			2022
#define		NODE_BSP			2023
#define		NODE_CAM2			2024
#define		NODE_ANIM			2025	

#define		SOUND_CLUCK			2026
#define		SOUND_AMBIENCE		2027

#define		TASK_UNLOAD_TARGET	2028
#define		TASK_HIT_CHECK		2029
#define		TASK_DIE			2030

#define		ANIM_COIN			2031

/////////////////////////////////////////
int GTargets[200];
int GHarmful[200];
int GCoins[200];
///////////////////////////////////////////////
char GObjectsMapOut[18][21]=
	{
	"gg b          E b ",
	"gg     s sgb B t T",
	"gg  p G sccccc B  ",
	"ggcccc  gsps B t T",
	"gg  p Mgp sTst Bc ",
	"gg c ccccccc s c b",
	"ggt  c csgggsEc g ",
	"ggg p g cPc Tc g b",
	"ggt p Ggpc Tc g   ",
	"ggH g pscsccssss g",
	"tgg p  c gccg g g ",
	"ggc   c  cggcg g b",
	"ggt  c  cggggc    ",
	"ggg cggcggggggcg b",
	"ggt sscsssgssssc  ",
	"tgg p gggggggggsg ",
	"   gg    Pt T     ",
	0
	};

//	H	BSP
//	M	main character
//	b	small bush
//	B	big bush
//	p	path way
//	G	giant donun
//	g	small donut
//	l	extra life
//	t	small tree
//	T	big tree
//	c	copper coin 30 points
//	e	left Gun enemy
//	E	Right gun enemy
//	s	censor turns on guns
//	r	reseter turns off guns


//////////////////////////////////////////
bool GTargetsInit(void);
bool PutTargets(int nodeid);
bool PutCoins(int nodeid);
bool PutHarmful(int nodeid);
bool TargetHit(MSGAEngine3D &e,int nodeid, void *);
int TargetsLength(void);
bool TargetHitTest(MSGAEngine3D &e, int no, void *);
bool CopperCoins(MSGAEngine3D &e,int nodeids);	
bool LoadObjects(MSGAEngine3D &e);
bool AddZips(MSGAEngine3D &e);
bool LoadSounds(MSGAEngine3D &e);
bool LoadTextures(MSGAEngine3D &e);
bool LoadMeshes(MSGAEngine3D &e);
bool DieOrJump(MSGAEngine3D &e,int,void *);
bool GameOverTask(MSGAEngine3D &e,int, void *);
bool GameOver(MSGAEngine3D &e);


///////////////////////////////////////////////////////////
int main(void)
	{
	MSGAEngine3D e(1024,768,32,false);
	
	e.WindowSetCaption("Another Try...");
	e.WindowSetColor(0,0,30);
	
	AddZips(e);
	LoadTextures(e);
	LoadSounds(e);
	LoadMeshes(e);
		
	e.AnimationLoad(ANIM_COIN,"coppercoin.bmp",3,3);
	//Set Color Key by pixel
	e.AnimationColorKeyPixel(ANIM_COIN,1,1);
	
	GTargetsInit();

	LoadObjects(e);

	//e.NodeAABBView(true);

	e.AnimatorCollisionResponse(NODE_ANIM,NODE_BSP,MESH_BSP,64,25,-1);

	MGameScoreCreate(e,"Power Quest");

	e.NodeLoadSkyBox(NODE_SKYBOX,TEX_UP,TEX_DN,TEX_FT,TEX_BK,TEX_LF,TEX_RT);	
	e.TaskLoad(TASK_HIT_CHECK,60,true,TargetHitTest);
	e.TaskLoad(TASK_DIE,60,true,DieOrJump);

	e.SoundPlay(SOUND_AMBIENCE,true);

	e.GameLoop();	
	MThirdPersonCameraDestroy();
	MGameScoreDestroy();
	MGameKeyboardControlDestroy();
	return 0;
	}


//////////////////////////////////////////////////////////
bool LoadObjects(MSGAEngine3D &e)
	{
	int i,j;

	for(i=0;i<18;i++)
		{
		for(j=0;j<21;j++)
			{
			//	load bsp 
			if(GObjectsMapOut[i][j]=='H')
				{
				e.NodeLoadOctTree(NODE_BSP,MESH_BSP);
				e.NodeSetPosition(NODE_BSP,i*100,0,j*100);
				}
			
			//	load character
			if(GObjectsMapOut[i][j]=='M')
				{
				e.NodeLoadAnimated(NODE_ANIM,MESH_ANIM);
				e.NodeSetPosition(NODE_ANIM,i*100,90,j*100);
				e.NodeSetTexture(NODE_ANIM,TEX_ANIM_CHICKEN);
				e.ObjectLoad(NODE_ANIM,NODE_ANIM,0,90,0);
				MGameKeyboardControlCreate(e,NODE_ANIM);
				MThirdPersonCameraCreate(e,NODE_ANIM);
				}
			
			//	load
			if(GObjectsMapOut[i][j]=='g')
				{
				int nodeid=(i*j+400000);
				e.MeshLoadSphere(MESH_PRIZE,20,3,40,3,200,100,200);
				e.NodeLoadMesh(nodeid,MESH_PRIZE);
				e.NodeSetPosition(nodeid,i*100,80,j*100);
				PutTargets(nodeid);
				}
			
			//	load
			if(GObjectsMapOut[i][j]=='s')
				{
				int nodeid=(i*j+500000);
				e.MeshLoadSphere(nodeid,40,13,2,255,255,0,0);
				e.MeshLoadSphere(-nodeid,20,20,20,10,200,200,200);
				e.NodeLoadMesh(nodeid,MESH_PRIZE);
				e.NodeLoadMesh(-nodeid,nodeid);
				e.NodeSetPosition(nodeid,i*100,270,j*100);
				e.NodeSetPosition(-nodeid,i*100,280,j*100);
				e.AnimatorRotation(nodeid,0,18,0);
				PutTargets(nodeid);
				PutHarmful(-nodeid);
				}
			
			
			//	load
			if(GObjectsMapOut[i][j]=='b')
				{
				int nodeid=(i*j+600000);
				e.NodeLoadMesh(nodeid,MESH_BUSH1);
				e.NodeSetPosition(nodeid,i*100,-20,j*100);
				e.NodeSetScale(nodeid,0.8,0.8,0.8);
				}
			
			//	load
			if(GObjectsMapOut[i][j]=='B')
				{
				int nodeid=(i*j+700000);
				e.NodeLoadMesh(nodeid,MESH_BUSH2);
				e.NodeSetPosition(nodeid,i*100,-2,j*100);
				}
			/*
			//	load
			if(GObjectsMapOut[i][j]=='c')
				{
				int nodeid=(i*j+5000000);
				CopperCoins(e,nodeid);
				e.NodeSetPosition(nodeid,i*100,60,j*100);
				PutCoins(nodeid);
				}
			*/

			//	load floor
			if(GObjectsMapOut[i][j]=='f')
				{
				int nodeid=(i*j+6000000);
				e.NodeLoadMesh(NODE_FLOOR,MESH_RECT);
				e.NodeSetTexture(NODE_FLOOR,TEX_FLOOR);
				e.NodeSetPosition(nodeid,i*100,-2,j*100);

				}
			}
		}

	return true;
	}

//////////////////////////////////////////
bool GTargetsInit(void)
	{
	int j;
	for(j=0;j<200;j++)
		{
		GTargets[j]=0;
		GHarmful[j]=0;
		GCoins[j]=0;
		printf("entered 0 into array[%d]\n",j);
		}

	return true;
	}


//////////////////////////////////////////
bool PutTargets(int nodeid)
	{
	int k;
	for(k=0;k<200;k++)
		{
		if(GTargets[k]==0)
			{
			GTargets[k]=nodeid;
			//printf("entered node %d into array[%d]\n",nodeid,k);
			break;
			}
		}
	
	return true;
	}

//////////////////////////////////////////
bool PutCoins(int nodeid)
	{
	int k;
	for(k=0;k<200;k++)
		{
		if(GCoins[k]==0)
			{
			GCoins[k]=nodeid;
			//printf("entered node %d into array[%d]\n",nodeid,k);
			break;
			}
		}
	
	return true;
	}

//////////////////////////////////////////
bool PutHarmful(int nodeid)
	{
	int k;
	for(k=0;k<200;k++)
		{
		if(GHarmful[k]==0)
			{
			GHarmful[k]=nodeid;
			//printf("entered node %d into array[%d]\n",nodeid,k);
			break;
			}
		}
	
	return true;
	}
///////////////////////////////////////////
bool TargetHit(MSGAEngine3D &e,int nodeid, void *)
	{
	e.SoundPlay(SOUND_CLUCK);
	e.NodeUnload(nodeid,true);
	
	return true;
	}


///////////////////////////////////////////
int TargetsLength(void)
	{
	int i,count=0;
	for(i=0;i<200;i++)
		{
		if(GTargets[i]!=0)
			{
			count++;
			}
		}

	return count;
	}


///////////////////////////////////////////
bool TargetHitTest(MSGAEngine3D &e, int, void *)
	{
	int i;
	for(i=0;i<200;i++)
		{
		int nodeid = GTargets[i];
		int coinid = GCoins[i];
		int harmid = GHarmful[i];
		if(nodeid!=0)
			{
			if(e.NodeAABBHitTest(nodeid,NODE_ANIM)==true)
				{
				//printf("your hit node %d\n",nodeid);
				MGameScoreAdd(2);
				e.TaskLoad(TASK_UNLOAD_TARGET,60,false,TargetHit,nodeid);
			
				GTargets[i]=0;
				}
			}

		if(harmid!=0)
			{
			if(e.NodeAABBHitTest(harmid,NODE_ANIM)==true)
				{
				//printf("your hit node %d\n",nodeid);
				MGameScoreAdd(-8);
				e.TaskLoad(TASK_UNLOAD_TARGET,60,false,TargetHit,nodeid);
				
				GHarmful[i]=0;
				}
			}
		
		if(coinid!=0)
			{	
			if(e.NodeAABBHitTest(coinid,NODE_ANIM)==true)
				{
				//printf("your hit node %d\n",nodeid);
				MGameScoreAdd(20);
				e.TaskLoad(TASK_UNLOAD_TARGET,60,false,TargetHit,nodeid);
				
				GCoins[i]=0;
				}
			}
		}

	return true;
	}
	
		
////////////////////////////////////////////////////////
bool CopperCoins(MSGAEngine3D &e,int nodeids)
	{	

	e.NodeLoadBillboard(nodeids);
	
	// Add Transparency effect
	e.NodeTransparentAlphaChannel(nodeids);

	e.TaskLoadAnimation(nodeids,ANIM_COIN,nodeids);
	
	return true;
	}	


////////////////////////////////////////////////////////
bool AddZips(MSGAEngine3D &e)
	{
	e.AddZipFile("duckman.zip");
	e.AddZipFile("map-badmojo.pk3");
	e.AddZipFile("m_plnt3.zip");
	e.AddZipFile("m_plnt4.zip");
	e.AddZipFile("m_tree3.zip");
	e.AddZipFile("m_tree4.zip");
	return true;
	}


////////////////////////////////////////////////////////
bool LoadSounds(MSGAEngine3D &e)
	{
	e.SoundLoad(SOUND_CLUCK,"10sillyd.wav");
	e.SoundLoad(SOUND_AMBIENCE,"corridoramb.wav");
	return true;
	}


////////////////////////////////////////////////////////
bool LoadTextures(MSGAEngine3D &e)
	{
	e.TextureLoad(TEX_FLOOR,"rock4.jpg");
	e.TextureLoad(TEX_BUSH1,"m_plnt3.gif");
	e.TextureLoad(TEX_BUSH2,"m_plant4.gif");
	e.TextureLoad(TEX_TREE1,"m_tree3.jpg");
	e.TextureLoad(TEX_TREE2,"m_tree4.gif");
	e.TextureLoad(TEX_ANIM_CHICKEN,"duck.pcx");
	e.TextureLoad(TEX_UP,"DESERT_UP.JPG");
	e.TextureLoad(TEX_DN,"DESERT_DN.JPG");
	e.TextureLoad(TEX_RT,"DESERT_RT.JPG");
	e.TextureLoad(TEX_LF,"DESERT_LF.JPG");
	e.TextureLoad(TEX_BK,"DESERT_BK.JPG");
	e.TextureLoad(TEX_FT,"DESERT_FT.JPG");
	return true;
	}


////////////////////////////////////////////////////////
bool LoadMeshes(MSGAEngine3D &e)
	{
	e.MeshLoadRectangle(MESH_RECT,3800,2600);
	e.MeshLoadBox(MESH_BOX,200,200,200,0,0,0,0);
	e.MeshLoad(MESH_ANIM,"tris.md2");
	e.MeshLoad(MESH_BUSH1,"m_plnt3.3ds");
	e.MeshLoad(MESH_BUSH2,"m_plant4.3ds");
	e.MeshLoad(MESH_TREE1,"m_tree3.3ds");
	e.MeshLoad(MESH_BSP,"badmojo.bsp");
	return true;
	}

/////////////////////////////////////////////////////////
bool DieOrJump(MSGAEngine3D &e,int,void *)
	{
	float x,y,z;
	e.NodeGetPosition(NODE_ANIM,x,y,z);
	if(e.KeyIsPressed(MSGA3D_KEY_J)==true)
		{
		e.NodeAddPosition(NODE_ANIM,0,30,0);
		}

	if(y<-100.0)
		{
		if(GameOver(e)==false)
			{
			return true;
			}
		}
	return true;
	}
	

/////////////////////////////////////////////////////
bool GameOverTask(MSGAEngine3D &e,int,void *)
	{	
	e.SoundLoad(4424,"menace to sobriety.mp3");
	e.TextureLoad(4419,"credits.jpg");
	e.NodeLoadBillboard(4422,300,2000);
	e.NodeSetTexture(4422,4419);
	e.NodeSetPosition(4422,0,-1500,300);
	
	e.NodeLoadCamera(4423);
	e.AnimatorMoveTo(4422,90000,0,3000,200);

	e.SoundPlay(4424);
	
	if(e.GameLoop()==0)
		{
		e.UnloadAll();
		return false;
		}
	e.UnloadAll();
	return true;
	}

///////////////////////////////////////////////////////////////
bool GameOver(MSGAEngine3D &e)
	{
	// Clear Screen to Red
	e.WindowSetColor(0,0,0);

	// Load the Cam
	e.NodeLoadCamera(1);
	e.SoundLoad(8888,"voc29.mp3");
	e.NodeLoadText(100,255,255,255,255,"Game Over... Goodbye...");
	e.NodeAddPosition(100,0,0,100);
	e.SoundPlay(8888);
	e.TaskLoad(100,100,false,GameOverTask);

	// if user presses escape, GameLoop Returns 0
	if(e.GameLoop()==0)
		{
		e.UnloadAll();
		return false;
		}

	e.UnloadAll();
	return true;
	}
