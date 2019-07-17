#include "RectangleUtil.h"

namespace RectangleUtil
{
  bool AreIntersecting(const RECT & aFirst, const RECT & aSecond)
  {
    if (aFirst.right < aSecond.left || aSecond.right < aFirst.left)
      return false;

    if (aFirst.bottom < aSecond.top|| aSecond.bottom < aFirst.top)
      return false;

    return true;
  }
}