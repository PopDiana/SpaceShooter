#pragma once
#include "Main.h"
#include "Sprite.h"
#include "Bullet.h"
#include "EnemyBullet.h"
#include <memory>
#include <vector>

class Enemy
{
public:
  Enemy(const BackBuffer * pBackBuffer, const Vec2 & aPosition);

  void 	 Update(float aTimeElapsed);
  void   Draw() const;

  RECT GetRectangle() const;

  bool IsShot(const Bullet & aBullet);

  std::unique_ptr<EnemyBullet> Shoot();

private:
  bool mShot;
  const BackBuffer * mBackBuffer;
  std::unique_ptr<Sprite> mSprite;
  std::vector<std::unique_ptr<EnemyBullet>> mFiredBullets;
};
