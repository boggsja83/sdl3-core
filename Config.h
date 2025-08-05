#ifndef CONFIG_H
#define CONFIG_H

#include <SDL3/SDL.h>
#include <iostream>

#include "Types.h"

static void dbg(ss& _message, Error_Enum _et=NONE_ET){
    rt r = _message.str().size();
    switch(_et){
	case CORE_ET:
	    if(r) std::cerr << "CORE Debug: " << _message.str() << '\n';
	    break;
	case SDL_ET:
	    if(r) std::cerr << _message.str() << '\n';
	    std::cerr << "SDL Error: " << SDL_GetError() << '\n';
	    break;
	case NONE_ET:
	default:
	    if(r) std::cerr << _message.str() << '\n';
	    break;
    }
}

static void dbg(str& _message, Error_Enum _et=NONE_ET){
    rt r = _message.size();
    switch(_et){
	case CORE_ET:
	    if(r) std::cerr << "CORE Debug: " << _message << '\n';
	    break;
	case SDL_ET:
	    if(r) std::cerr << _message << '\n';
	    std::cerr << "SDL Error: " << SDL_GetError() << '\n';
	    break;
	case NONE_ET:
	default:
	    if(r) std::cerr << _message << '\n';
	    break;
    }
}

static void dbg(const char* _message, Error_Enum _et=NONE_ET){
    rt r = strlen(_message);
    switch(_et){
	case CORE_ET:
	    if(r) std::cerr << "CORE Debug: " << _message << '\n';
	    break;
	case SDL_ET:
	    if(r) std::cerr << _message << '\n';
	    std::cerr << "SDL Error: " << SDL_GetError() << '\n';
	    break;
	case NONE_ET:
	default:
	    if(r) std::cerr << _message << '\n';
	    break;
    }
}

typedef struct Config
{

    SDL_Window*	    win		= nullptr;
    SDL_GPUDevice*  gpu_dev	= nullptr;

    ui32	    swap_w	= 0;
    ui32	    swap_h	= 0;

    ss		    debug_ss;
    str		    debug_str	= "";
    const char*	    debug_cstr	= "";

} Config;

#endif

