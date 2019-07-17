#include "Enemy.h"
#include "RectangleUtil.h"

Enemy::Enemy(const BackBuffer * pBackBuffer, const Vec2 & aPosition)
  :mSprite(new Sprite("data/enemy.bmp", "data/enemyMask.bmp"))
  ,mBackBuffer(pBackBuffer)
{
  mSprite->mPosition = aPosition;
  mSprite->setBackBuffer(pBackBuffer);
}

void Enemy::Update(float aTimeElapsed)
{
  mSprite->update(aTimeElapsed);

  
}

void Enemy::Draw() const
{
  mSprite->draw();
}

RECT Enemy::GetRectangle() const
{
  return mSprite->GetRectangle();
}

bool Enemy::IsShot(const Bullet & aBullet)
{
  RECT bulletRect = aBullet.GetRectangle();
  RECT enemyRect  = GetRectangle();

  if (RectangleUtil::AreIntersecting(bulletRect, enemyRect))
    return mSprite->AreMasksOverlapping(*aBullet.GetSpritePtr());

  return false;
}

std::unique_ptr<EnemyBullet> Enemy::Shoot()
{
  return std::make_unique<EnemyBullet>(mBackBuffer, mSprite->mPosition);
}
