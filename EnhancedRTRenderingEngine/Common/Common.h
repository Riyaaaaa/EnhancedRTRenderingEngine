#pragma once
#include <stdio.h>  

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define D3DX_PI ((float) 3.141592654f) 
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))
#define D3DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))

#ifdef UNICODE 
typedef wchar_t ERTREChar;
#else
typedef char ERTREChar;
#endif

#ifdef _DEBUG
#   define ERTREDebug( str, ... ) \
      { \
        ERTREChar c[256]; \
        _stprintf_s( c, str, __VA_ARGS__ ); \
        OutputDebugString( c ); \
      }
#else
#    define ERTREDebug( str, ... ) // ‹óŽÀ‘•
#endif
