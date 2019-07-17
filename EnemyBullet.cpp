#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(const BackBuffer * pBackBuffer, const Vec2 & aPosition)
  :Bullet(pBackBuffer, aPosition, DIRECTION::DIR_BACKWARD, nullptr)
{
}