#include "Core.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_oldnames.h>

rt Core::loop(){
    rt r = OKAY;
    bool running = true;

    while(running){
    
	r = input();
	if(r<0) running = false;

	if(r>=0) r = update();
	if(r<0) running = false;

	if(r>=0) r = render();
	if(r<0) running = false;

    }

    return r;
}

rt Core::input(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
	switch(event.type){
	case SDL_EVENT_QUIT:
	    return QUIT;
	default:;
	}
    }
    return OKAY;
}

rt Core::update(){
    return OKAY;
}

rt Core::render(){
    SDL_GPUCommandBuffer *cmd_buf = nullptr;
    SDL_GPUTexture *swapchain = nullptr;
    SDL_GPUColorTargetInfo clr_target_nfo{};
    ui32 width=0; ui32 height=0;
    SDL_GPURenderPass* rend_pass = nullptr;
    rt r = OKAY;

    r = sdlw.acquire_gpu_command_buffer(cfg.gpu_dev,&cmd_buf);
    if(r>=0) r = sdlw.wait_acquire_swapchain_texture(cmd_buf,cfg.win,&swapchain,&width,&height); 

    if(!swapchain){
	SDL_SubmitGPUCommandBuffer(cmd_buf);
	return SDL_APP_CONTINUE;
    }

    clr_target_nfo.clear_color = {000/255.f,100/255.f,255/255.f,255/255.f};
    clr_target_nfo.load_op = SDL_GPU_LOADOP_CLEAR;
    clr_target_nfo.store_op = SDL_GPU_STOREOP_STORE;
    clr_target_nfo.texture = swapchain;

    rend_pass = SDL_BeginGPURenderPass(cmd_buf,&clr_target_nfo,1,nullptr);

    // draw something
    
    SDL_EndGPURenderPass(rend_pass);

    SDL_SubmitGPUCommandBuffer(cmd_buf);

    return r;
}
