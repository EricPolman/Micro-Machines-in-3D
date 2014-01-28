#include "Shapes.h"
#include "Core.h"
#include <assert.h>
#include "surface.h"

using namespace GameLayer;

/* TMPL8-specific Draw-methods. */
#ifdef GAMELAYER_USE_TMPL8
void DrawLine_Tmpl8(float beginX, float beginY, float endX, float endY)
{
  ((Tmpl8::Surface*)Core::g_pRenderTarget)->Line(
    beginX, beginY, endX, endY, 0xFFFFFFFF);
}

void DrawCircle_Tmpl8(float x, float y, float radius, int density)
{
  float stepsize = 2 * 3.141528 / density;
  float currentRotation = 0;
  for(int i = 0; i < density; i++)
  {
    ((Tmpl8::Surface*)Core::g_pRenderTarget)->Line(
      x + cos(currentRotation) * radius, y + sin(currentRotation) * radius, 
      x + cos(currentRotation + stepsize) * radius, y + sin(currentRotation + stepsize) * radius,
      0xFFFFFFFF);
    currentRotation += stepsize;
  }
}

void DrawBox_Tmpl8(float beginX, float beginY, float endX, float endY)
{
  ((Tmpl8::Surface*)Core::g_pRenderTarget)->Box(
    beginX, beginY, endX, endY, 0xFFFFFFFF);
}
#endif
/* End of TMPL8-specific Draw-methods. */


void Shapes::DrawLine(float beginX, float beginY, float endX, float endY)
{
#ifdef GAMELAYER_USE_TMPL8
    DrawLine_Tmpl8(beginX, beginY, endX, endY);
#else
  assert(false && "Unimplemented platform.");
#endif
}


void Shapes::DrawCircle(float x, float y, float radius, int density)
{
#ifdef GAMELAYER_USE_TMPL8
    DrawCircle_Tmpl8(x,y,radius,density);
#else
  assert(false && "Unimplemented platform.");
#endif
}


void Shapes::DrawBox(float beginX, float beginY, float endX, float endY)
{
#ifdef GAMELAYER_USE_TMPL8
    DrawBox_Tmpl8(beginX, beginY, endX, endY);
#else
    //Can have more implementations here.
    assert(false && "Unimplemented platform.");
#endif
}