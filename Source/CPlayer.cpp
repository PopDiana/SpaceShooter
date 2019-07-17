//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//       such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "CPlayer.h"
#include <algorithm>
#include "../RectangleUtil.h"

//-----------------------------------------------------------------------------
// Name : CPlayer () (Constructor)
// Desc : CPlayer Class Constructor
//-----------------------------------------------------------------------------
CPlayer::CPlayer(const BackBuffer *pBackBuffer, std::vector<Bullet> & aFiredBullets)
  :mBackBuffer(pBackBuffer)
  ,mFacingDirection(DIRECTION::DIR_FORWARD)
  ,mLives(3)
  ,mScore(0)
  ,mFiredBullets(aFiredBullets)
{
	m_pSprite = new Sprite("data/planeimg.bmp", "data/planemask.bmp");
	//m_pSprite = new Sprite("data/planeimgandmask.bmp", RGB(0xff,0x00, 0xff));
	m_pSprite->setBackBuffer( pBackBuffer );
	m_eSpeedState = SPEED_STOP;
	m_fTimer = 0;

	// Animation frame crop rectangle
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 128;
	r.bottom = 128;

	m_pExplosionSprite	= new AnimatedSprite("data/explosion.bmp", "data/explosionmask.bmp", r, 16);
	m_pExplosionSprite->setBackBuffer( pBackBuffer );
	m_bExplosion		= false;
	m_iExplosionFrame	= 0;
}

//-----------------------------------------------------------------------------
// Name : ~CPlayer () (Destructor)
// Desc : CPlayer Class Destructor
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	delete m_pSprite;
	delete m_pExplosionSprite;
}

void CPlayer::Update(float dt, const RECT & rectangle)
{
  RECT playerRect;
  playerRect.left   = (LONG)m_pSprite->mPosition.x - m_pSprite->width() / 2;
  playerRect.right  = (LONG)m_pSprite->mPosition.x + m_pSprite->width() / 2;
  playerRect.top    = (LONG)m_pSprite->mPosition.y - m_pSprite->height() / 2;
  playerRect.bottom = (LONG)m_pSprite->mPosition.y + m_pSprite->height() / 2;

  if (m_pSprite->mVelocity.x < 0 && playerRect.left <= rectangle.left)
    this->ResetXVelocity();
  if (m_pSprite->mVelocity.x > 0 && playerRect.right >= rectangle.right)
    this->ResetXVelocity();

  if (m_pSprite->mVelocity.y < 0 && playerRect.top <= rectangle.top)
    this->ResetYVelocity();
  if (m_pSprite->mVelocity.y > 0 && playerRect.bottom >= rectangle.bottom)
    this->ResetYVelocity();

  mFiredBullets.erase(remove_if(begin(mFiredBullets), end(mFiredBullets), 
                                [&](const auto & aBullet)
                                { return !aBullet.IsInRectangle(rectangle); }),
                      mFiredBullets.end());

	// Update sprite
	m_pSprite->update(dt);

  for (auto & bullet : mFiredBullets)
  {
    bullet.Update(dt);
  }

	// Get velocity
	double v = m_pSprite->mVelocity.Magnitude();

	// NOTE: for each async sound played Windows creates a thread for you
	// but only one, so you cannot play multiple sounds at once.
	// This creation/destruction of threads also leads to bad performance
	// so this method is not recommanded to be used in complex projects.

	// update internal time counter used in sound handling (not to overlap sounds)
	m_fTimer += dt;

	// A FSM is used for sound manager 
	switch(m_eSpeedState)
	{
	case SPEED_STOP:
		if(v > 35.0f)
		{
			m_eSpeedState = SPEED_START;
			PlaySound("data/jet-start.wav", NULL, SND_FILENAME | SND_ASYNC);
			m_fTimer = 0;
		}
		break;
	case SPEED_START:
		if(v < 25.0f)
		{
			m_eSpeedState = SPEED_STOP;
			PlaySound("data/jet-stop.wav", NULL, SND_FILENAME | SND_ASYNC);
			m_fTimer = 0;
		}
		else
			if(m_fTimer > 1.f)
			{
				PlaySound("data/jet-cabin.wav", NULL, SND_FILENAME | SND_ASYNC);
				m_fTimer = 0;
			}
		break;
	}

	// NOTE: For sound you also can use MIDI but it's Win32 API it is a bit hard
	// see msdn reference: http://msdn.microsoft.com/en-us/library/ms711640.aspx
	// In this case you can use a C++ wrapper for it. See the following article:
	// http://www.codeproject.com/KB/audio-video/midiwrapper.aspx (with code also)
}

void CPlayer::Draw()
{
  for (auto & aBullet : mFiredBullets)
  {
    aBullet.Draw();
  }

	if(!m_bExplosion)
		m_pSprite->draw();
	else
		m_pExplosionSprite->draw();

}

void CPlayer::Move(ULONG ulDirection)
{
	if( ulDirection & DIRECTION::DIR_LEFT )
		m_pSprite->mVelocity.x -= .7;

	if( ulDirection & DIRECTION::DIR_RIGHT )
		m_pSprite->mVelocity.x += .7;

	if( ulDirection & DIRECTION::DIR_FORWARD )
		m_pSprite->mVelocity.y -= .7;

	if( ulDirection & DIRECTION::DIR_BACKWARD )
		m_pSprite->mVelocity.y += .7;
}


Vec2& CPlayer::Position()
{
	return m_pSprite->mPosition;
}

Vec2& CPlayer::Velocity()
{
	return m_pSprite->mVelocity;
}

void CPlayer::Explode()
{
	m_pExplosionSprite->mPosition = m_pSprite->mPosition;
	m_pExplosionSprite->SetFrame(0);
	PlaySound("data/explosion.wav", NULL, SND_FILENAME | SND_ASYNC);
	m_bExplosion = true;
}

bool CPlayer::AdvanceExplosion()
{
	if(m_bExplosion)
	{
		m_pExplosionSprite->SetFrame(m_iExplosionFrame++);
		if(m_iExplosionFrame==m_pExplosionSprite->GetFrameCount())
		{
			m_bExplosion = false;
			m_iExplosionFrame = 0;
			m_pSprite->mVelocity = Vec2(0,0);
			m_eSpeedState = SPEED_STOP;
			return false;
		}
	}

	return true;
}

bool CPlayer::IsExploding() const
{
  return m_bExplosion;
}

void CPlayer::Shoot()
{
  if (m_bExplosion)
    return;

  static auto lastFireTime = 0;
  auto currentFireTime = ::GetTickCount();

  if (currentFireTime - lastFireTime < 200)
    return;

  mFiredBullets.emplace_back(mBackBuffer, m_pSprite->mPosition, mFacingDirection, this);
  lastFireTime = currentFireTime;
}

void CPlayer::ShootEnemies(EnemyGroup & aEnemyGroup)
{
  std::vector<Bullet*> usedBullets;
  for (auto & aBullet : mFiredBullets)
  {
    bool isUsed = aEnemyGroup.HandleBullet(aBullet);
    if (isUsed)
    {
      usedBullets.push_back(&aBullet);
    }
  }

  mScore += usedBullets.size();

  auto newEnd = std::remove_if(mFiredBullets.begin(), mFiredBullets.end(),
                               [&](Bullet & aBullet)
                               {
                                 return std::find(usedBullets.begin(), usedBullets.end(), &aBullet)
                                        != usedBullets.end();
                               });

  mFiredBullets.erase(newEnd, mFiredBullets.end());
}

void CPlayer::ResetXVelocity()
{
  m_pSprite->mVelocity.x = 0;
}

void CPlayer::ResetYVelocity()
{
  m_pSprite->mVelocity.y = 0;
}

void CPlayer::RotateLeft()
{
  auto position = m_pSprite->mPosition;
  auto velocity = m_pSprite->mVelocity;

  delete m_pSprite;

  switch (mFacingDirection)
  {
  case DIRECTION::DIR_FORWARD:
    m_pSprite = new Sprite("data/leftPlaneImg.bmp", "data/leftPlaneMask.bmp");
    mFacingDirection = DIRECTION::DIR_LEFT;
    break;
  case DIRECTION::DIR_BACKWARD:
    m_pSprite = new Sprite("data/rightPlaneImg.bmp", "data/rightPlaneMask.bmp");
    mFacingDirection = DIRECTION::DIR_RIGHT;
    break;
  case DIRECTION::DIR_LEFT:
    mFacingDirection = DIRECTION::DIR_BACKWARD;
    m_pSprite = new Sprite("data/downPlaneImg.bmp", "data/downPlaneMask.bmp");
    break;
  case DIRECTION::DIR_RIGHT:
    m_pSprite = new Sprite("data/upPlaneImg.bmp", "data/upPlaneMask.bmp");
    mFacingDirection = DIRECTION::DIR_FORWARD;
    break;
  }
  m_pSprite->mPosition = position;
  m_pSprite->mVelocity = velocity;
  m_pSprite->setBackBuffer(mBackBuffer);
}

void CPlayer::RotateRight()
{
  auto position = m_pSprite->mPosition;
  auto velocity = m_pSprite->mVelocity;

  delete m_pSprite;

  switch (mFacingDirection)
  {
  case DIRECTION::DIR_FORWARD:
    m_pSprite = new Sprite("data/rightPlaneImg.bmp", "data/rightPlaneMask.bmp");
    mFacingDirection = DIRECTION::DIR_RIGHT;
    break;
  case DIRECTION::DIR_BACKWARD:
    m_pSprite = new Sprite("data/leftPlaneImg.bmp", "data/leftPlaneMask.bmp");
    mFacingDirection = DIRECTION::DIR_LEFT;
    break;
  case DIRECTION::DIR_LEFT:
    m_pSprite = new Sprite("data/upPlaneImg.bmp", "data/upPlaneMask.bmp");
    mFacingDirection = DIRECTION::DIR_FORWARD;
    break;
  case DIRECTION::DIR_RIGHT:
    m_pSprite = new Sprite("data/downPlaneImg.bmp", "data/downPlaneMask.bmp");
    mFacingDirection = DIRECTION::DIR_BACKWARD;
    break;
  }

  m_pSprite->mPosition = position;
  m_pSprite->mVelocity = velocity;
  m_pSprite->setBackBuffer(mBackBuffer);
}

int CPlayer::GetLives()
{
  return mLives;
}

size_t CPlayer::GetScore()
{
  return mScore;
}

bool CPlayer::GetShot(EnemyGroup & aEnemyGroup)
{
  if (m_bExplosion)
    return false;

  for (auto & aBullet : aEnemyGroup)
  {
    RECT bulletRect = aBullet->GetRectangle();
    RECT playerRect = m_pSprite->GetRectangle();

    auto isShot = RectangleUtil::AreIntersecting(bulletRect, playerRect) && 
                  m_pSprite->AreMasksOverlapping(*aBullet->GetSpritePtr());

    if (isShot) 
    {
      mLives--;
      Explode();
      return true;
    }
  }

  return false;
}

bool CPlayer::IsShot(const Bullet * aBullet) const
{
  if (aBullet->WasFiredBy(this) || m_bExplosion)
    return false;

  return m_pSprite->AreMasksOverlapping(*aBullet->GetSpritePtr());
}

void CPlayer::DecreaseLives()
{
  --mLives;
}
