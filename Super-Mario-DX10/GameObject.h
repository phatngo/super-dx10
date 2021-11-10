#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"


using namespace std;

#define STATIC	0
#define MOVING	1
#define IGNORE_DEFINE	2

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define PUSHBACK 0.4f
#define VIEWPORT_PUSHBACK 16

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;

	// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{

public:

	float x; 
	float y;
	float start_X;
	float start_Y;

	float dx=0;	// dx = vx*dt
	float dy=0;	// dy = vy*dt
	float ax = 0; 
	float ay = 0;
	float vx=0;
	float vy=0;
	int tag;
	int type;
	int nx;
	int ny;
	bool isDestroyed;
	int state;

	DWORD dt; 

	LPANIMATION_SET animation_set;

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; start_X = x; start_Y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	float GetStartX() { return start_X; }
	float GetStartY() { return start_Y; }
	int GetState() { return this->state; }
	void SetTag(int tag) { this->tag = tag; }
	void SetType(int type) { this->type = type; }

	void RenderBoundingBox(int alpha = 32);

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	virtual bool isColliding(float friend_left, float friend_top, float friend_right, float friend_bottom);
	bool IsDestroyed() { return isDestroyed; }
	void SetIsDestroyed() { isDestroyed = true; }

	//Collision

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);

	CGameObject();
	~CGameObject();
};

