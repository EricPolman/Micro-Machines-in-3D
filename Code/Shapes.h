#pragma once
namespace GameLayer
{
  class Shapes
  {
  public:
    static void DrawLine(float beginX, float beginY, float endX, float endY);
    static void DrawCircle(float x, float y, float radius, int density);
    static void DrawBox(float beginX, float beginY, float endX, float endY);
  };
};