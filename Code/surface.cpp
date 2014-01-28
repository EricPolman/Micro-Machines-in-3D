// Template, major revision 3, beta
// IGAD/NHTV - Jacco Bikker - 2006-2009

#include "template.h"
#include "surface.h"
#include "freeimage.h"

namespace Tmpl8 {

void NotifyUser( char* s );

// -----------------------------------------------------------
// True-color surface class implementation
// -----------------------------------------------------------

Surface::Surface( int a_Width, int a_Height, Pixel* a_Buffer, int a_Pitch ) :
	m_Width( a_Width ),
	m_Height( a_Height ),
	m_Buffer( a_Buffer ),
	m_Pitch( a_Pitch )
{
}

Surface::Surface( int a_Width, int a_Height ) :
	m_Width( a_Width ),
	m_Height( a_Height ),
	m_Pitch( a_Width )
{
	m_Buffer = (Pixel*)MALLOC64( a_Width * a_Height * sizeof( Pixel ) );
}

Surface::Surface( const char* a_File ) :
	m_Buffer( NULL ),
	m_Width( 0 ), m_Height( 0 )
{
	FILE* f = fopen( a_File, "rb" );
	if (!f) 
	{
		char t[128];
		sprintf( t, "File not found: %s", a_File );
		NotifyUser( t ); 
		return;
	}
	else fclose( f );
	LoadImage( a_File );
}

void Surface::LoadImage( const char* a_File )
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType( a_File, 0 );
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename( a_File );
	FIBITMAP* tmp = FreeImage_Load( fif, a_File );
	FIBITMAP* dib = FreeImage_ConvertTo32Bits( tmp );
	FreeImage_Unload( tmp );
	m_Width = m_Pitch = FreeImage_GetWidth( dib );
	m_Height = FreeImage_GetHeight( dib );
	m_Buffer = (Pixel*)MALLOC64( m_Width * m_Height * sizeof( Pixel ) );
	for( int y = 0; y < m_Height; y++) 
	{
		unsigned char* line = FreeImage_GetScanLine( dib, m_Height - 1 - y );
		memcpy( m_Buffer + y * m_Pitch, line, m_Width * sizeof( Pixel ) );
	}
	FreeImage_Unload( dib );
}

Surface::~Surface()
{
	FREE64( m_Buffer );
}

void Surface::Clear( Pixel a_Color )
{
	int s = m_Width * m_Height;
	for ( int i = 0; i < s; i++ ) m_Buffer[i] = a_Color;
}

void Surface::Centre( char* a_String, int y1, Pixel color )
{
	int x = (m_Width - (int)strlen( a_String ) * 6) / 2;
	Print( a_String, x, y1, color );
}

void Surface::Print( const char* a_String, int x1, int y1, Pixel color )
{
	Pixel* t = m_Buffer + x1 + y1 * m_Pitch;
	int i;
	for ( i = 0; i < (int)(strlen( a_String )); i++ )
	{	
		long pos = 0;
		if ((a_String[i] >= 'A') && (a_String[i] <= 'Z')) pos = s_Transl[(unsigned short)(a_String[i] - ('A' - 'a'))];
													 else pos = s_Transl[(unsigned short)a_String[i]];
		Pixel* a = t;
		char* c = (char*)s_Font[pos];
		int h, v;
		for ( v = 0; v < 5; v++ ) 
		{
			for ( h = 0; h < 5; h++ ) if (*c++ == 'o') *(a + h) = color, *(a + h + m_Pitch) = 0;
			a += m_Pitch;
		}
		t += 6;
	}
}

void Surface::Resize( int a_Width, int a_Height, Surface* a_Orig )
{
	unsigned int newpitch = (a_Width + 16) & 0xffff0;
	Pixel* src = a_Orig->GetBuffer(), *dst = m_Buffer;
	int u, v, owidth = a_Orig->GetWidth(), oheight = a_Orig->GetHeight();
	int dx = (owidth << 10) / a_Width, dy = (oheight << 10) / a_Height;
	for ( v = 0; v < a_Height; v++ )
	{
		for ( u = 0; u < a_Width; u++ )
		{
			int su = u * dx, sv = v * dy;
			Pixel* s = src + (su >> 10) + (sv >> 10) * owidth;
			int ufrac = su & 1023, vfrac = sv & 1023;
			int w4 = (ufrac * vfrac) >> 12;
			int w3 = ((1023 - ufrac) * vfrac) >> 12;
			int w2 = (ufrac * (1023 - vfrac)) >> 12;
			int w1 = ((1023 - ufrac) * (1023 - vfrac)) >> 12;
			int x2 = ((su + dx) > ((owidth - 1) << 10))?0:1;
			int y2 = ((sv + dy) > ((oheight - 1) << 10))?0:1;
			Pixel p1 = *s, p2 = *(s + x2), p3 = *(s + owidth * y2), p4 = *(s + owidth * y2 + x2);
			unsigned int r = (((p1 & REDMASK) * w1 + (p2 & REDMASK) * w2 + (p3 & REDMASK) * w3 + (p4 & REDMASK) * w4) >> 8) & REDMASK;
			unsigned int g = (((p1 & GREENMASK) * w1 + (p2 & GREENMASK) * w2 + (p3 & GREENMASK) * w3 + (p4 & GREENMASK) * w4) >> 8) & GREENMASK;
			unsigned int b = (((p1 & BLUEMASK) * w1 + (p2 & BLUEMASK) * w2 + (p3 & BLUEMASK) * w3 + (p4 & BLUEMASK) * w4) >> 8) & BLUEMASK;
			*(dst + u + v * newpitch) = (Pixel)(r + g + b);
		}
	}
	m_Width = a_Width, m_Height = a_Height, m_Pitch = newpitch;
}

void Surface::Line( float x1, float y1, float x2, float y2, Pixel c )
{
	if ((x1 < 0) || (y1 < 0) || (x1 >= m_Width) || (y1 >= m_Height) ||
		(x2 < 0) || (y2 < 0) || (x2 >= m_Width) || (y2 >= m_Height))
	{
		return;
	}
	float b = x2 - x1;
	float h = y2 - y1;
	float l = fabsf( b );
	if (fabsf ( h ) > l) l = fabsf( h );
	int il = (int)l;
	float dx = b / (float)l;
	float dy = h / (float)l;
	for ( int i = 0; i <= il; i++ )
	{
		*(m_Buffer + (int)x1 + (int)y1 * m_Pitch) = c;
		x1 += dx, y1 += dy;
	}
}

void Surface::Plot( int x, int y, Pixel c )
{ 
	if ((x >= 0) && (y >= 0) && (x < m_Width) && (y < m_Height)) m_Buffer[x + y * m_Pitch] = c;
}

void Surface::Box( int x1, int y1, int x2, int y2, Pixel c )
{
	Line( (float)x1, (float)y1, (float)x2, (float)y1, c );
	Line( (float)x2, (float)y1, (float)x2, (float)y2, c );
	Line( (float)x1, (float)y2, (float)x2, (float)y2, c );
	Line( (float)x1, (float)y1, (float)x1, (float)y2, c );
}

void Surface::Bar( int x1, int y1, int x2, int y2, Pixel c )
{
	Pixel* a = x1 + y1 * m_Pitch + m_Buffer;
	for ( int y = y1; y <= y2; y++ )
	{
		for ( int x = 0; x <= (x2 - x1); x++ ) a[x] = c;
		a += m_Pitch;
	}
}


void Surface::CopyTo( Surface* a_Dst, int a_X, int a_Y, bool transparency)
{
	Pixel* dst = a_Dst->GetBuffer();
	Pixel* src = m_Buffer;
	if ((src) && (dst)) 
	{
		int srcwidth = m_Width;
		int srcheight = m_Height;
		int srcpitch = m_Pitch;
		int dstwidth = a_Dst->GetWidth();
		int dstheight = a_Dst->GetHeight();
		int dstpitch = a_Dst->GetPitch();
		if ((srcwidth + a_X) > dstwidth) srcwidth = dstwidth - a_X;
		if ((srcheight + a_Y) > dstheight) srcheight = dstheight - a_Y;
		if (a_X < 0) src -= a_X, srcwidth += a_X, a_X =0;
		if (a_Y < 0) src -= a_Y * srcpitch, srcheight += a_Y, a_Y = 0;
		if ((srcwidth > 0) && (srcheight > 0))
		{
			dst += a_X + dstpitch * a_Y;
			for ( int y = 0; y < srcheight; y++ )
			{
				if(transparency)
				{
					for(int x = 0; x < srcwidth; x++)
					{
						if((*(src+x) & 0x00FFFFFF) != 0xFF00FF)
						{
							memcpy( dst+x, src+x, sizeof(Pixel) );
						}
					}
				}
				else
				{
					memcpy( dst, src, srcwidth * 4 );
				}
				dst += dstpitch;
				src += srcpitch;
			}
		}
	}
}


void Surface::CopyPartTo( Surface* a_Dst, int a_X, int a_Y, int a_sourceX, int a_sourceY, int a_width, int a_height, bool transparency)
{
	//Credits to Max for figuring out how to select a rectangle to copy.

	if(a_width + a_sourceX > m_Width || a_height + a_sourceY > m_Height)
	{
		printf("Source out of range.\n");
		return;
	}

	Pixel* dst = a_Dst->GetBuffer();
	Pixel* src = m_Buffer;
	if ((src) && (dst)) 
	{
		if(a_width > m_Width)
		{
			a_width = m_Width;
		}
		if(a_height > m_Height)
		{
			a_height = m_Height;
		}

		int srcwidth = a_width;
		int srcheight = a_height;
		int srcpitch = m_Pitch;

		int dstwidth = a_Dst->GetWidth();
		int dstheight = a_Dst->GetHeight();
		int dstpitch = a_Dst->GetPitch();
		
		src += a_sourceX + (a_sourceY * srcpitch);

		if ((srcwidth + a_X) > dstwidth) 
			srcwidth = dstwidth - a_X;
		if ((srcheight + a_Y) > dstheight) 
			srcheight = dstheight - a_Y;

		if (a_X < 0) 
			src -= a_X, srcwidth += a_X, a_X =0;
		if (a_Y < 0) 
			src -= a_Y * srcpitch, srcheight += a_Y, a_Y = 0;

		if ((srcwidth > 0) && (srcheight > 0))
		{
			dst += a_X + dstpitch * a_Y;
			for ( int y = 0; y < srcheight; y++ )
			{
				for(int i=0; i<srcwidth; ++i)
				{
					if(transparency)
					{
						if((*(src+i) & 0x00FFFFFF) != 0xFF00FF)
						{
							memcpy(dst+i, src+i, sizeof(Pixel));
						}
					}
					else
					{
						memcpy(dst+i, src+i, sizeof(Pixel));
					}
				}
				dst += dstpitch;
				src += srcpitch;
			}
		}
	}
}

void Surface::CopyToWithRotation( int a_X, int a_Y, int a_Width, int a_Height, float a_fRotation, Surface* a_Target, int a_pivotX, int a_pivotY, bool transparency )
{
  
  //printf(m_Buffer[0] << std::endl;
	if ((a_Width == 0) || (a_Height == 0)) return; //Scaling it to zero isn't helpful, so it returns.
  
  float sine = sin(a_fRotation);
  float cosine = cos(a_fRotation);

	const int du = (m_Pitch << 10) / a_Width; //Old width over new width, multiplied by 2^10 to increase integer precision.
	const int dv = (m_Height << 10) / a_Height; //Old height over new height, multiplied by 2^10 to increase integer precision.
	Pixel* dest = a_Target->GetBuffer() + a_X + a_Y * a_Target->GetPitch(); //Destination buffer
  //Pixel* tempdest = dest;
	Pixel* src = GetBuffer(); //Source buffer (with support for frames, in sprites)
	int v = 0; //The V-coordinate, which is the Y-coordinate of a texture map.
	for ( int y = 0; y < a_Height; y++ )
	{
    int yOffset = y - a_pivotY;
		int u = 0; //The U-coordinate, which is the Y-coordinate of a texture map.
		int cv = (v >> 10) * m_Pitch; //This determines which row (V-coordinate/Y-coordinate) from the texture map (the original image) will be used
		for ( int x = 0; x < a_Width; x++ )
		{
      Pixel val = *(src + (u >> 10) + cv);
      //Pixel valAlphaMasked = ;
      if(transparency && (val == 0xFFFF00FF || 
        (val & 0xFF000000) == 0))
      {
        u += du;
        continue;
      }
      //This is setting the destination pixel (dest+x, since dest is the current row) to the UV-coordinate of the texture (original image). \
      cv is the current row (current V-coord) and u >> 10 is the U-coordinate back to its unshifted value.
      int xOffset = x - a_pivotX;
      int newX = xOffset * cosine - yOffset * sine;
      int newY = xOffset * sine + yOffset * cosine;
      if(a_Y + newY < 0 || a_Y + newY > a_Target->GetHeight()
      || a_X + newX < 0 || a_X +newX > a_Target->GetWidth())
      {
        u += du;
        continue;
      }
      if(!transparency || (val & 0xFF000000) == 0xFF000000)
      {
        dest[(int)(newX + newY * a_Target->GetPitch())] = *(src + (u >> 10) + cv);
      }
      else
      {
        Pixel destPix = dest[(int)(newX + newY * a_Target->GetPitch())];
        byte destred = ((destPix & 0xFF0000) >> 16);
        byte destgreen = (destPix & 0xFF00) >> 8;
        byte destblue = destPix & 0xFF;

        int temp = 0;
        
        float alpha = (float)((val & 0xFF000000) >> 24) / 256;
        byte red = ((val & 0xFF0000) >> 16);
        temp = red + destred;
        if(temp > 255) red = 255 - destred;
        temp = 0;
        byte green = (val & 0xFF00) >> 8;
        temp = green + destgreen;
        if(temp > 255) green = 255 - destgreen;
        temp = 0;
        byte blue = val & 0xFF;
        temp = blue + destblue;
        if(temp > 255) blue = 255 - destblue;
        temp = 0;
        Pixel newPix(
          (byte)(red) << 16 | 
          (byte)(green) << 8 | 
          (byte)(blue));

        dest[(int)(newX + newY * a_Target->GetPitch())] += newPix;
      }
      
      u += du; //Used for iterating through the texture's U-coordinates. \
      If the original is 100px wide and the scaled version is 200px wide, du will be 100/200=1/2. So for every pixel in the scaled version, the u-coordinate\
      is incremented by 0.5. Same goes for v with dv.
		}
		v += dv;
		//tempdest = dest + (int)(yOffset * sine); //Hop to the next row by adding the pitch.
    //The ints u and v are shifted >> 10 to get the numbers back to their normal values. We don't need the precision anymore.
	}
}


void Surface::BlendCopyTo( Surface* a_Dst, int a_X, int a_Y )
{
	Pixel* dst = a_Dst->GetBuffer();
	Pixel* src = m_Buffer;
	if ((src) && (dst)) 
	{
		int srcwidth = m_Width;
		int srcheight = m_Height;
		int srcpitch = m_Pitch;
		int dstwidth = a_Dst->GetWidth();
		int dstheight = a_Dst->GetHeight();
		int dstpitch = a_Dst->GetPitch();
		if ((srcwidth + a_X) > dstwidth) srcwidth = dstwidth - a_X;
		if ((srcheight + a_Y) > dstheight) srcheight = dstheight - a_Y;
		if (a_X < 0) src -= a_X, srcwidth += a_X, a_X =0;
		if (a_Y < 0) src -= a_Y * srcpitch, srcheight += a_Y, a_Y = 0;
		if ((srcwidth > 0) && (srcheight > 0))
		{
			dst += a_X + dstpitch * a_Y;
			for ( int y = 0; y < srcheight; y++ )
			{
				for ( int x = 0; x < srcwidth; x++ ) dst[x] = AddBlend( dst[x], src[x] );
				dst += dstpitch;
				src += srcpitch;
			}
		}
	}
}

void Surface::SetChar( int c, char* c1, char* c2, char* c3, char* c4, char* c5 )
{
	strcpy( s_Font[c][0], c1 );
	strcpy( s_Font[c][1], c2 );
	strcpy( s_Font[c][2], c3 );
	strcpy( s_Font[c][3], c4 );
	strcpy( s_Font[c][4], c5 );
}

void Surface::InitCharset()
{
	SetChar( 0, ":ooo:", "o:::o", "ooooo", "o:::o", "o:::o" );
	SetChar( 1, "oooo:", "o:::o", "oooo:", "o:::o", "oooo:" );
	SetChar( 2, ":oooo", "o::::", "o::::", "o::::", ":oooo" );
	SetChar( 3, "oooo:", "o:::o", "o:::o", "o:::o", "oooo:" );
	SetChar( 4, "ooooo", "o::::", "oooo:", "o::::", "ooooo" );
	SetChar( 5, "ooooo", "o::::", "ooo::", "o::::", "o::::" );
	SetChar( 6, ":oooo", "o::::", "o:ooo", "o:::o", ":ooo:" );
	SetChar( 7, "o:::o", "o:::o", "ooooo", "o:::o", "o:::o" );
	SetChar( 8, "::o::", "::o::", "::o::", "::o::", "::o::" );
	SetChar( 9, ":::o:", ":::o:", ":::o:", ":::o:", "ooo::" );
	SetChar(10, "o::o:", "o:o::", "oo:::", "o:o::", "o::o:" );
	SetChar(11, "o::::", "o::::", "o::::", "o::::", "ooooo" );
	SetChar(12, "oo:o:", "o:o:o", "o:o:o", "o:::o", "o:::o" );
	SetChar(13, "o:::o", "oo::o", "o:o:o", "o::oo", "o:::o" );
	SetChar(14, ":ooo:", "o:::o", "o:::o", "o:::o", ":ooo:" );
	SetChar(15, "oooo:", "o:::o", "oooo:", "o::::", "o::::" );
	SetChar(16, ":ooo:", "o:::o", "o:::o", "o::oo", ":oooo" );
	SetChar(17, "oooo:", "o:::o", "oooo:", "o:o::", "o::o:" );
	SetChar(18, ":oooo", "o::::", ":ooo:", "::::o", "oooo:" );
	SetChar(19, "ooooo", "::o::", "::o::", "::o::", "::o::" );
	SetChar(20, "o:::o", "o:::o", "o:::o", "o:::o", ":oooo" );
	SetChar(21, "o:::o", "o:::o", ":o:o:", ":o:o:", "::o::" );
	SetChar(22, "o:::o", "o:::o", "o:o:o", "o:o:o", ":o:o:" );
	SetChar(23, "o:::o", ":o:o:", "::o::", ":o:o:", "o:::o" );
	SetChar(24, "o:::o", "o:::o", ":oooo", "::::o", ":ooo:" );
	SetChar(25, "ooooo", ":::o:", "::o::", ":o:::", "ooooo" );
	SetChar(26, ":ooo:", "o::oo", "o:o:o", "oo::o", ":ooo:" );
	SetChar(27, "::o::", ":oo::", "::o::", "::o::", ":ooo:" );
	SetChar(28, ":ooo:", "o:::o", "::oo:", ":o:::", "ooooo" );
	SetChar(29, "oooo:", "::::o", "::oo:", "::::o", "oooo:" );
	SetChar(30, "o::::", "o::o:", "ooooo", ":::o:", ":::o:" );
	SetChar(31, "ooooo", "o::::", "oooo:", "::::o", "oooo:" );
	SetChar(32, ":oooo", "o::::", "oooo:", "o:::o", ":ooo:" );
	SetChar(33, "ooooo", "::::o", ":::o:", "::o::", "::o::" );
	SetChar(34, ":ooo:", "o:::o", ":ooo:", "o:::o", ":ooo:" );
	SetChar(35, ":ooo:", "o:::o", ":oooo", "::::o", ":ooo:" );
	SetChar(36, "::o::", "::o::", "::o::", ":::::", "::o::" );
	SetChar(37, ":ooo:", "::::o", ":::o:", ":::::", "::o::" );
	SetChar(38, ":::::", ":::::", "::o::", ":::::", "::o::" );
	SetChar(39, ":::::", ":::::", ":ooo:", ":::::", ":ooo:" );
	SetChar(40, ":::::", ":::::", ":::::", ":::o:", "::o::" );
	SetChar(41, ":::::", ":::::", ":::::", ":::::", "::o::" );
	SetChar(42, ":::::", ":::::", ":ooo:", ":::::", ":::::" );
	SetChar(43, ":::o:", "::o::", "::o::", "::o::", ":::o:" );
	SetChar(44, "::o::", ":::o:", ":::o:", ":::o:", "::o::" );
	SetChar(45, ":::::", ":::::", ":::::", ":::::", ":::::" );
	SetChar(46, "ooooo", "ooooo", "ooooo", "ooooo", "ooooo" );
	SetChar(47, "::o::", "::o::", ":::::", ":::::", ":::::" ); // Tnx Ferry
	SetChar(48, "o:o:o", ":ooo:", "ooooo", ":ooo:", "o:o:o" );
	SetChar(49, "::::o", ":::o:", "::o::", ":o:::", "o::::" );
	char c[] = "abcdefghijklmnopqrstuvwxyz0123456789!?:=,.-() #'*/";
	int i;
	for ( i = 0; i < 256; i++ ) s_Transl[i] = 45;
	for ( i = 0; i < 50; i++ ) s_Transl[(unsigned char)c[i]] = i;
}

void Surface::ScaleColor( unsigned int a_Scale )
{
	int s = m_Pitch * m_Height;
	for ( int i = 0; i < s; i++ )
	{
		Pixel c = m_Buffer[i];
		unsigned int rb = (((c & (REDMASK|BLUEMASK)) * a_Scale) >> 5) & (REDMASK|BLUEMASK);
		unsigned int g = (((c & GREENMASK) * a_Scale) >> 5) & GREENMASK;
		m_Buffer[i] = rb + g;
	}
}

Sprite::Sprite( Surface* a_Surface, unsigned int a_NumFrames ) :
	m_Width(  a_Surface->GetWidth() / a_NumFrames ),
	m_Height( a_Surface->GetHeight() ),
	m_Pitch(  a_Surface->GetWidth() ),
	m_NumFrames( a_NumFrames ),
	m_CurrentFrame( 0 ),
	m_Flags( 0 ),
	m_Start( new unsigned int*[a_NumFrames] ),
	m_Surface( a_Surface )
{
	InitializeStartData();
}

Sprite::~Sprite()
{
	delete m_Surface;
	for ( unsigned int i = 0; i < m_NumFrames; i++ ) delete m_Start[i];
	delete m_Start;
}

void Sprite::Draw( int a_X, int a_Y, Surface* a_Target )
{
	if ((a_X < -m_Width) || (a_X > (a_Target->GetWidth() + m_Width))) return;
	if ((a_Y < -m_Height) || (a_Y > (a_Target->GetHeight() + m_Height))) return;
	int x1 = a_X, x2 = a_X + m_Width;
	int y1 = a_Y, y2 = a_Y + m_Height;
	Pixel* src = GetBuffer() + m_CurrentFrame * m_Width;
	if (x1 < 0)
	{
		src += -x1;
		x1 = 0;
	}
	if (x2 > a_Target->GetWidth()) x2 = a_Target->GetWidth();
	if (y1 < 0) 
	{ 
		src += -y1 * m_Pitch;
		y1 = 0;
	}
	if (y2 > a_Target->GetHeight()) y2 = a_Target->GetHeight();
	Pixel* dest = a_Target->GetBuffer();
	int xs;
	const int dpitch = a_Target->GetPitch();
	if ((x2 > x1) && (y2 > y1))
	{
		unsigned int addr = y1 * dpitch + x1;
		const int width = x2 - x1;
		const int height = y2 - y1;
		for ( int y = 0; y < height; y++ )
		{
			const int line = y + (y1 - a_Y);
			const int lsx = m_Start[m_CurrentFrame][line] + a_X;
			if (m_Flags & FLARE)
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					const Pixel c1 = *(src + x);
					if (c1) 
					{
						const Pixel c2 = *(dest + addr + x);
						*(dest + addr + x) = AddBlend( c1, c2 );
					}
				}
			}
			else 
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					const Pixel c1 = *(src + x);
					if (c1) *(dest + addr + x) = c1;
				}
			}
			addr += dpitch;
			src += m_Pitch;
		}
	}
}

void Sprite::DrawScaled( int a_X, int a_Y, int a_Width, int a_Height, Surface* a_Target )
{
	if ((a_Width == 0) || (a_Height == 0)) return; //Scaling it to zero isn't helpful, so it returns.
	const int du = (m_Pitch << 10) / a_Width; //Old width over new width, multiplied by 2^10 to increase integer precision.
	const int dv = (m_Height << 10) / a_Height; //Old height over new height, multiplied by 2^10 to increase integer precision.
	Pixel* dest = a_Target->GetBuffer() + a_X + a_Y * a_Target->GetPitch(); //Destination buffer
	Pixel* src = GetBuffer() + m_CurrentFrame * m_Pitch; //Source buffer (with support for frames, in sprites)
	int v = 0; //The V-coordinate, which is the Y-coordinate of a texture map.
	for ( int y = 0; y < a_Height; y++ )
	{
		int u = 0; //The U-coordinate, which is the X-coordinate of a texture map.
		int cv = (v >> 10) * m_Pitch; //This determines which row (V-coordinate/Y-coordinate) from the texture map (the original image) will be used
		for ( int x = 0; x < a_Width; x++ )
		{
      //This is setting the destination pixel (dest+x, since dest is the current row) to the UV-coordinate of the texture (original image). \
      cv is the current row (current V-coord) and u >> 10 is the U-coordinate back to its unshifted value.
			*(dest + x) = *(src + (u >> 10) + cv); 
			
      u += du; //Used for iterating through the texture's U-coordinates. \
      If the original is 100px wide and the scaled version is 200px wide, du will be 100/200=1/2. So for every pixel in the scaled version, the u-coordinate\
      is incremented by 0.5. Same goes for v with dv.
		}
		v += dv;
		dest += a_Target->GetPitch(); //Hop to the next row by adding the pitch.
    //The ints u and v are shifted >> 10 to get the numbers back to their normal values. We don't need the precision anymore.
	}
}

void Sprite::InitializeStartData()
{
    for ( unsigned int f = 0; f < m_NumFrames; ++f )
    {
        m_Start[f] = new unsigned int[m_Height];
     	for ( int y = 0; y < m_Height; ++y )
     	{
      	    m_Start[f][y] = m_Width;
			Pixel* addr = GetBuffer() + f * m_Width + y * m_Pitch;
     	    for ( int x = 0; x < m_Width; ++x )
     	    {
                if (addr[x])
     	        {
     	            m_Start[f][y] = x;
                    break;
                }
            }
		}
	}
}

Font::Font( char* a_File, char* a_Chars )
{
	m_Surface = new Surface( a_File );
	Pixel* b = m_Surface->GetBuffer();
	int w = m_Surface->GetWidth();
	int h = m_Surface->GetHeight();
	unsigned int charnr = 0, width = 0, start = 0;
	m_Trans = new int[256];
	memset( m_Trans, 0, 1024 );
	unsigned int i;
	for ( i = 0; i < strlen( a_Chars ); i++ ) m_Trans[(unsigned char)a_Chars[i]] = i;
	m_Offset = new int[strlen( a_Chars )];
	m_Width = new int[strlen( a_Chars )];
	m_Height = h;
	m_CY1 = 0, m_CY2 = 1024;
	int x, y;
	bool lastempty = true;
	for ( x = 0; x < w; x++ )
	{
		bool empty = true;
		for ( y = 0; y < h; y++ ) if (*(b + x + y * w) & 0xffffff) 
		{
			if (lastempty)
			{
				width = 0;
				start = x;
			}
			empty = false;
		}
		if ((empty) && (!lastempty))
		{
			m_Width[charnr] = x - start;
			m_Offset[charnr] = start;
			if (++charnr == strlen( a_Chars )) break;
		}
		lastempty = empty;
	}
}

Font::~Font()
{
	delete m_Surface;
	delete m_Trans;
	delete m_Width;
	delete m_Offset;
}

int Font::Width( char* a_Text )
{
	int w = 0;
	unsigned int i;
	for ( i = 0; i < strlen( a_Text ); i++ )
	{
		unsigned char c = (unsigned char)a_Text[i];
		if (c == 32) w += 4; else w += m_Width[m_Trans[c]] + 2;
	}
	return w;
}

void Font::Centre( Surface* a_Target, char* a_Text, int a_Y )
{
	int x = (a_Target->GetPitch() - Width( a_Text )) / 2;
	Print( a_Target, a_Text, x, a_Y );
}
 
void Font::Print( Surface* a_Target, char* a_Text, int a_X, int a_Y, bool clip )
{
	Pixel* b = a_Target->GetBuffer() + a_X + a_Y * a_Target->GetPitch();
	Pixel* s = m_Surface->GetBuffer();
	unsigned int i, cx;
	int x, y;
	if (((a_Y + m_Height) < m_CY1) || (a_Y > m_CY2)) return;
	for ( cx = 0, i = 0; i < strlen( a_Text ); i++ )
	{
		if (a_Text[i] == ' ') cx += 4; 
		else
		{
			int c = m_Trans[(unsigned char)a_Text[i]];
			Pixel* t = s + m_Offset[c], *d = b + cx;
			if (clip)
			{
				for ( y = 0; y < m_Height; y++ )
				{
					if (((a_Y + y) >= m_CY1) && ((a_Y + y) <= m_CY2))
					{
						for ( x = 0; x < m_Width[c]; x++ ) 
							if ((t[x]) && ((x + (int)cx + a_X) < a_Target->GetPitch())) 
							{
								if(t[x] != 0xFF000000)
									d[x] = t[x];//AddBlend( t[x], d[x] );
							}
					}
					t += m_Surface->GetPitch(), d += a_Target->GetPitch();
				}
			}
			else
			{
				for ( y = 0; y < m_Height; y++ )
				{
					if (((a_Y + y) >= m_CY1) && ((a_Y + y) <= m_CY2))
						for ( x = 0; x < m_Width[c]; x++ ) if (t[x]) d[x] = AddBlend( t[x], d[x] );
					t += m_Surface->GetPitch(), d += a_Target->GetPitch();
				}
			}
			cx += m_Width[c] + 2;
			if ((int)(cx + a_X) >= a_Target->GetPitch()) break;
		}
	}
}

}; // namespace Tmpl8
