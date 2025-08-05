#include "Core.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_oldnames.h>

rt Core::loop(){
    rt r = OKAY;
    bool running = true;

    while(running){
    
	r = input();
	// if(r<0) running = false;

	if(r>=0) r = update();
	// if(r<0) running = false;

	if(r>=0) r = render();

	if(r<0) running = false;

    }

    return r;
}

rt Core::input(){
	//    SDL_Event event;
	//    while(SDL_PollEvent(&event)){
	// switch(event.type){
	// case SDL_EVENT_QUIT:
	//     return QUIT;
	// default:;
	// }
	//    }
    return OKAY;
}

rt Core::update(){
    return OKAY;
}

rt Core::render(){
    rt r = OKAY;

    r = sdlw.do_render_pass();

    return r;
}

