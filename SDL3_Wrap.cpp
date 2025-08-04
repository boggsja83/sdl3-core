#include "SDL3_Wrap.h"

rt SDL3_Wrap::init(SDL_InitFlags _flags){
    if(!SDL_Init(_flags)){
	dbg("SDL Init failed", SDL_ET);
	return FAIL_SDL_INIT;
    }
    else{
	return OKAY;
    }
}

rt SDL3_Wrap::create_window(const char *_title, i32 _width, i32 _height, SDL_WindowFlags _flags, bool _set_cfg){
    SDL_Window* temp = SDL_CreateWindow(_title,_width,_height,_flags);
    if(!temp){
	dbg("CreateWindow failed",SDL_ET);
	return FAIL_CREATE_WINDOW;
    }
    else{
	win_lst.push_back(temp);
	if(!cfg_ptr->win && _set_cfg) cfg_ptr->win = temp;
	return win_lst.size()-1;
    }
}

rt SDL3_Wrap::create_gpu_device(SDL_GPUShaderFormat _shader_fmt, bool _debug, const char *_name, bool _set_cfg){
    SDL_GPUDevice* temp = SDL_CreateGPUDevice(_shader_fmt,_debug,_name);
    if(!temp){
	dbg("CreateGPUDevice failed", SDL_ET);
	return FAIL_CREATE_GPU_DEVICE;
    }
    else{
	gpu_dev_lst.push_back(temp);
	if(!cfg_ptr->gpu_dev && _set_cfg) cfg_ptr->gpu_dev = temp; 
	return gpu_dev_lst.size()-1;
    }
}

rt SDL3_Wrap::claim_window_for_gpu_device(SDL_GPUDevice *_device, SDL_Window *_win){
    if(!SDL_ClaimWindowForGPUDevice(_device,_win)){
	dbg("ClaimWindowForGPUDevice failed", SDL_ET);
	return FAIL_CLAIM_WINDOW;
    }
    else return OKAY;
}

rt SDL3_Wrap::acquire_gpu_command_buffer(SDL_GPUDevice *_device, SDL_GPUCommandBuffer **_cmd_buf){
    // command buffer is managed by SDL and should not be deleted in any way
    SDL_GPUCommandBuffer* temp = SDL_AcquireGPUCommandBuffer(_device);
    if(!temp){
	dbg("AcquireGPUCommandBuffer failed",SDL_ET); 
	return FAIL_ACQUIRE_GPU_COMMAND_BUFFER;
    }
    else{
	*_cmd_buf = temp;
	return OKAY;
    }
}

rt SDL3_Wrap::wait_acquire_swapchain_texture(SDL_GPUCommandBuffer *_cmd_buf, SDL_Window *_win, SDL_GPUTexture **_sc_text, ui32 *_width, ui32 *_height){
    // swapchain texture is managed by SDL and should not be deleted in any way
    // check for nullptr. all 'filled' parameters may become nullptr during lifespan
    if(!SDL_WaitAndAcquireGPUSwapchainTexture(_cmd_buf,_win,_sc_text,_width,_height)){
	dbg("WaitAndAcquireGPUSwapchainTexture failed",SDL_ET);
	return FAIL_ACQUIRE_GPU_SWAPCHAIN_TEXT;
    }
    else return OKAY;
}

void SDL3_Wrap::destroy(){
    std::cerr << "Deleting " << gpu_dev_lst.size() << " gpu devices\n";
    for(i16 i=0; i<gpu_dev_lst.size(); ++i){
	SDL_DestroyGPUDevice(gpu_dev_lst[i]);
    }
    std::cerr << "Deleting " << win_lst.size() << " windows\n";
    for(i16 i=0; i<win_lst.size(); ++i){
	SDL_DestroyWindow(win_lst[i]);
    }

    SDL_Quit();
}

rt SDL3_Wrap::do_gpu_render_pass(SDL_GPUCommandBuffer *_cmd_buf, const SDL_GPUColorTargetInfo *_col_tgt_nfo, ui32 _num_col_tgt, const SDL_GPUDepthStencilTargetInfo *_dep_stencil_tgt_nfo){

    return OKAY;
}
