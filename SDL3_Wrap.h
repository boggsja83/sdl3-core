#ifndef SDL3_WRAP_H
#define SDL3_WRAP_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_main.h>

#include <vector>

#include "Types.h"
#include "Config.h"

class SDL3_Wrap{
    public:

	SDL3_Wrap(){
	    win_lst.clear();
	    win_lst.reserve(1);
	    gpu_dev_lst.clear();
	    gpu_dev_lst.reserve(1);

	    cfg_ptr = nullptr;
	}
	
	// call SDL3_Wrap::destroy() externally using SDL3 call backs
	~SDL3_Wrap(){}

    public:
	std::vector<SDL_Window*>	    win_lst;
	std::vector<SDL_GPUDevice*>	    gpu_dev_lst;	
    public:
	Config*				    cfg_ptr;
    public:
	rt init(SDL_InitFlags _flags);
	rt create_window(const char *_title,i32 _width, i32 _height, SDL_WindowFlags _flags, bool _set_cfg=true); 	
	rt create_gpu_device(SDL_GPUShaderFormat _shader_fmt, bool _debug, const char *_name, bool _set_cfg=true);
	rt claim_window_for_gpu_device(SDL_GPUDevice *_device, SDL_Window *_win);
	rt acquire_gpu_command_buffer(SDL_GPUDevice *_device, SDL_GPUCommandBuffer **_cmd_buf);
	rt wait_acquire_swapchain_texture(SDL_GPUCommandBuffer *_cmd_buf, SDL_Window *_win, SDL_GPUTexture **_sc_text, ui32 *_width, ui32 *_height);
	void destroy();
    public:
	rt do_gpu_render_pass(SDL_GPUCommandBuffer *_cmd_buf, const SDL_GPUColorTargetInfo *_col_tgt_nfo, ui32 _num_col_tgt, const SDL_GPUDepthStencilTargetInfo *_dep_stencil_tgt_nfo);
};

#endif

