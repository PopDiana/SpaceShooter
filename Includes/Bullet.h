#pragma once
#include "Main.h"
#include "Sprite.h"
#include "../IPlayer.h"
#include <memory>

class Bullet
{
public:
  enum DIRECTION
  {
    DIR_FORWARD = 1,
    DIR_BACKWARD = 2,
    DIR_LEFT = 4,
    DIR_RIGHT = 8,
  };

  Bullet(const BackBuffer * pBackBuffer, const Vec2 & aPosition, DIRECTION aDirection, 
         const IPlayer * const aLauncher);

 
  void 	 Update(float aTimeElapsed);
  void   Draw() const;

  RECT GetRectangle() const;

  bool IsInRectangle(const RECT & aRectangle) const;

  const Sprite * GetSpritePtr() const;

  bool WasFiredBy(const IPlayer * aPlayer) const;

protected:
  std::unique_ptr<Sprite> mSprite;
  const IPlayer * mLauncher;
  DIRECTION mDirection;
};