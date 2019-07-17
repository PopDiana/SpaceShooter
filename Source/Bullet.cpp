#include "Bullet.h"
using namespace std;

Bullet::Bullet(const BackBuffer * pBackBuffer, 
               const Vec2 & aPosition, 
               DIRECTION aDirection,
               const IPlayer * const aLauncher)
  : mLauncher(aLauncher)
  , mDirection(aDirection)
{
  switch (aDirection)
  {
  case DIRECTION::DIR_FORWARD:
    mSprite = make_unique<Sprite>("data/upBullet.bmp", "data/upBulletMask.bmp");
    mSprite->mVelocity.y = -300;
    break;
  case DIRECTION::DIR_BACKWARD:
    mSprite = make_unique<Sprite>("data/downBullet.bmp", "data/downBulletMask.bmp");
    mSprite->mVelocity.y = +300;
    break;
  case DIRECTION::DIR_LEFT:
    mSprite = make_unique<Sprite>("data/leftBullet.bmp", "data/leftBulletMask.bmp");
    mSprite->mVelocity.x = -300;
    break;
  case DIRECTION::DIR_RIGHT:
    mSprite = make_unique<Sprite>("data/rightBullet.bmp", "data/rightBulletMask.bmp");
    mSprite->mVelocity.x = +300;
    break;
  }
  mSprite->setBackBuffer(pBackBuffer);
  mSprite->mPosition = aPosition;
}

void Bullet::Update(float aTimeElapsed)
{
  mSprite->update(aTimeElapsed);
}

void Bullet::Draw() const
{
  mSprite->draw();
}

RECT Bullet::GetRectangle() const
{
  return mSprite->GetRectangle();
}

bool Bullet::IsInRectangle(const RECT & aRectangle) const
{
  RECT bulletRect;
  bulletRect.left   = (LONG)mSprite->mPosition.x - mSprite->width() / 2;
  bulletRect.top    = (LONG)mSprite->mPosition.y - mSprite->height() / 2;
  bulletRect.right  = (LONG)mSprite->mPosition.x + mSprite->width() / 2;
  bulletRect.bottom = (LONG)mSprite->mPosition.y + mSprite->height() / 2;

  if (bulletRect.right < aRectangle.left || bulletRect.left > aRectangle.right)
    return false;

  if (bulletRect.bottom < aRectangle.top || bulletRect.top > aRectangle.bottom)
    return false;

  return true;
}

const Sprite * Bullet::GetSpritePtr() const
{
  return mSprite.get();
}

bool Bullet::WasFiredBy(const IPlayer * aPlayer) const
{
  return mLauncher == aPlayer;
}
