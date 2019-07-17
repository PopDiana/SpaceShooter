//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//	   such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _CPLAYER_H_
#define _CPLAYER_H_

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include <vector>
#include "Main.h"
#include "Sprite.h"
#include "Bullet.h"
#include "../EnemyGroup.h"
#include "../IPlayer.h"

//-----------------------------------------------------------------------------
// Main Class Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CPlayer (Class)
// Desc : Player class handles all player manipulation, update and management.
//-----------------------------------------------------------------------------
class CPlayer : public IPlayer
{
public:
	//-------------------------------------------------------------------------
	// Enumerators
	//-------------------------------------------------------------------------
	using DIRECTION = Bullet::DIRECTION;

	enum ESpeedStates
	{
		SPEED_START,
		SPEED_STOP
	};

	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
	CPlayer(const BackBuffer *pBackBuffer, std::vector<Bullet> & aFiredBullets);
	virtual ~CPlayer();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	void					Update(float dt, const RECT & rectangle);
	void					Draw();
	void					Move(ULONG ulDirection);
	Vec2&					Position();
	Vec2&					Velocity();

	void					Explode();
	bool					AdvanceExplosion();
	bool          IsExploding() const;

	void Shoot();

	void ShootEnemies(EnemyGroup & aEnemyGroup);

	void ResetXVelocity();
	void ResetYVelocity();

	void RotateLeft();
	void RotateRight();

	int GetLives();
	size_t GetScore();

	bool GetShot(EnemyGroup & aEnemyGroup);

	bool IsShot(const Bullet * aBullet) const;

	void DecreaseLives();

private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	Sprite*					m_pSprite;
	ESpeedStates			m_eSpeedState;
	float					m_fTimer;

	bool					m_bExplosion;
	AnimatedSprite*			m_pExplosionSprite;
	int						m_iExplosionFrame;

	const BackBuffer * mBackBuffer;
	std::vector<Bullet> & mFiredBullets;

	DIRECTION mFacingDirection;
	int mLives;
	size_t mScore;
};

#endif // _CPLAYER_H_