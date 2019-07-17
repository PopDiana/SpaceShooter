#pragma once

#include <Windows.h>

namespace RectangleUtil
{
  bool AreIntersecting(const RECT & aFirst, const RECT & aSecond);
}