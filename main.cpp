#include "Types.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_scancode.h>
#define SDL_MAIN_USE_CALLBACKS

#include "Core.h"

static Core core;

static Vertex vertices[]{
    {0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f},
    {0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f}
};

SDL_AppResult SDL_AppInit(void **_appstate, i32 _argc, char **_argv){
    rt r = core.sdlw.init(SDL_INIT_VIDEO, &core.cfg); 
    if(r>=0) r = core.sdlw.create_window("sdl3-core",800,600,SDL_WINDOW_RESIZABLE|SDL_WINDOW_HIDDEN,true);
    if(r>=0) r = core.sdlw.create_gpu_device(SDL_GPU_SHADERFORMAT_SPIRV,true,nullptr,true);
    if(r>=0) r = core.sdlw.claim_window_for_gpu_device(core.cfg.gpu_dev,core.cfg.win);

    if(r>=0) SDL_ShowWindow(core.cfg.win);

    if(r>=0) return SDL_APP_CONTINUE;
    else{
	core.cfg.debug_ss.str("");
	core.cfg.debug_ss << r;
	return SDL_APP_FAILURE;
    }
}

SDL_AppResult SDL_AppIterate(void *_appstate){
    rt r = OKAY;
    r = core.render();
    if(r>=0) return SDL_APP_CONTINUE;
    else{
	core.cfg.debug_ss.str("");
	core.cfg.debug_ss << r;
	return SDL_APP_FAILURE;
    }
}

SDL_AppResult SDL_AppEvent(void *_appstate, SDL_Event *_event){
    // if(_event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED){
	// core.cfg.debug_ss.str("");
	// core.cfg.debug_ss << "window close requested";
	// return SDL_APP_SUCCESS;
	//    }
    switch(_event->type){
	case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
	    return SDL_APP_SUCCESS;
	case SDL_EVENT_KEY_DOWN:
	    break;
	case SDL_EVENT_KEY_UP:
	    switch(_event->key.scancode){
		case SDL_SCANCODE_CAPSLOCK:
		    return SDL_APP_SUCCESS;
		default:;
	    }
	    break;
	default:;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *_appstate, SDL_AppResult _result){
    core.sdlw.destroy();
    dbg(core.cfg.debug_ss,CORE_ET);

    // core.cfg.debug_ss.str("");
    // core.cfg.debug_ss << "swap_w: " << core.cfg.swap_w << " | swap_h: " << core.cfg.swap_h;
    // dbg(core.cfg.debug_ss,CORE_ET);
}

