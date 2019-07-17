#pragma once
#include "Bullet.h"
#include "BackBuffer.h"

class EnemyBullet :public Bullet
{
public:
  EnemyBullet(const BackBuffer * pBackBuffer, const Vec2 & aPosition);
};