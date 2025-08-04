#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <sstream>
#include <string>

typedef __int16_t   i16;
typedef __int32_t   i32;
typedef __int64_t   i64;

typedef __uint16_t  ui16;
typedef __uint32_t  ui32;
typedef __uint64_t  ui64;

typedef i16	    rt;
typedef ui16	    urt;

typedef std::string str;
typedef std::stringstream ss;

typedef struct Vertex{
    float x=0.f, y=0.f, z=0.f;
    float r=0.f, g=0.f, b=0.f, a=0.f;
} Vertex;

typedef enum{
    OKAY		    = 0,
    QUIT		    = -1
} CORE_Enum;

typedef enum {
    NONE_ET		    = 0,
    CORE_ET		    = 1,
    SDL_ET		    = 2
} Error_Enum;

typedef enum {
    FAIL_SDL_INIT	    = -100,
    FAIL_CREATE_WINDOW	    = -101,
    FAIL_CREATE_GPU_DEVICE  = -102,
    FAIL_CLAIM_WINDOW	    = -103,
    FAIL_ACQUIRE_GPU_COMMAND_BUFFER = -104,
    FAIL_ACQUIRE_GPU_SWAPCHAIN_TEXT = -105
} SDL_Enum;

#endif

