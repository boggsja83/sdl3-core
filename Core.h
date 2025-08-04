#ifndef CORE_H
#define CORE_H

#include "Types.h"
#include "Config.h"
#include "SDL3_Wrap.h"

class Core {
    public:
	Core(){}
	~Core(){}
    public:
	SDL3_Wrap   sdlw;
	Config	    cfg;
    public:
	rt loop();
	rt input();
	rt update();
	rt render();
};

#endif

