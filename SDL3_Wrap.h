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
	    gpu_dev_lst.clear();
	    vertex_buf_lst.clear();
	    tx_buf_lst.clear();
	    gpu_pl_lst.clear();

	    win_lst.reserve(1);
	    gpu_dev_lst.reserve(1);
	    vertex_buf_lst.reserve(5);
	    tx_buf_lst.reserve(5);
	    gpu_pl_lst.reserve(5);

	    cfg_ptr = nullptr;
	}
	
	~SDL3_Wrap(){}

    public:
	std::vector<SDL_Window*>	    win_lst;
	std::vector<SDL_GPUDevice*>	    gpu_dev_lst;	
	std::vector<SDL_GPUBuffer*>	    vertex_buf_lst;
	std::vector<SDL_GPUTransferBuffer*> tx_buf_lst;
	std::vector<SDL_GPUGraphicsPipeline*> gpu_pl_lst;
	std::vector<SDL_GPUShader*>	    vert_shdr_lst;
	std::vector<SDL_GPUShader*>	    frag_shdr_lst;
    public:
	Config*				    cfg_ptr;
    public:
	rt init(SDL_InitFlags _flags, Config *_cfg_ptr);
	void destroy();
    public:
	rt create_window(const char *_title,i32 _width, i32 _height, SDL_WindowFlags _flags, bool _set_cfg=true); 	
	rt create_gpu_device(SDL_GPUShaderFormat _shader_fmt, bool _debug, const char *_name, bool _set_cfg=true);
	rt claim_window_for_gpu_device(SDL_GPUDevice *_device, SDL_Window *_win);
	rt acquire_gpu_command_buffer(SDL_GPUDevice *_device, SDL_GPUCommandBuffer **_cmd_buf);
	rt wait_acquire_swapchain_texture(SDL_GPUCommandBuffer *_cmd_buf, SDL_Window *_win, SDL_GPUTexture **_sc_text, ui32 *_width, ui32 *_height);
    public:
	rt create_vertex_buffer(SDL_GPUDevice* _device, SDL_GPUBufferCreateInfo* _buf_nfo);
	rt create_transfer_buffer(SDL_GPUDevice* _device, SDL_GPUTransferBufferCreateInfo* _buf_nfo);
    public:
	// rt do_gpu_render_pass(SDL_GPUCommandBuffer *_cmd_buf, const SDL_GPUColorTargetInfo *_col_tgt_nfo, ui32 _num_col_tgt, const SDL_GPUDepthStencilTargetInfo *_dep_stencil_tgt_nfo);
	rt do_copy_pass();
	rt do_render_pass();
};

#endif

