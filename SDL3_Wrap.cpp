#include "SDL3_Wrap.h"
#include "Types.h"
#include <SDL3/SDL_gpu.h>

rt SDL3_Wrap::init(SDL_InitFlags _flags, Config *_cfg_ptr){
    if(!SDL_Init(_flags)){
	dbg("SDL Init failed", SDL_ET);
	return FAIL_SDL_INIT;
    }
    else{
	cfg_ptr = _cfg_ptr;
	return OKAY;
    }
}

void SDL3_Wrap::destroy(){
    std::cerr << "Releasing " << vert_shdr_lst.size() << " vertex shaders\n";
    for(i16 i=0; i<tx_buf_lst.size(); ++i){ SDL_ReleaseGPUShader(cfg_ptr->gpu_dev, vert_shdr_lst[i]); }

    std::cerr << "Releasing " << frag_shdr_lst.size() << " fragment shaders\n";
    for(i16 i=0; i<tx_buf_lst.size(); ++i){ SDL_ReleaseGPUShader(cfg_ptr->gpu_dev, frag_shdr_lst[i]); }

    std::cerr << "Releasing " << tx_buf_lst.size() << " transfer buffers\n";
    for(i16 i=0; i<tx_buf_lst.size(); ++i){ SDL_ReleaseGPUTransferBuffer(cfg_ptr->gpu_dev, tx_buf_lst[i]); }

    std::cerr << "Releasing " << vertex_buf_lst.size() << " vertex buffers\n";
    for(i16 i=0; i<vertex_buf_lst.size(); ++i){ SDL_ReleaseGPUBuffer(cfg_ptr->gpu_dev, vertex_buf_lst[i]); }

    std::cerr << "Releasing " << gpu_pl_lst.size() << " gpu pipelines\n";
    for(i16 i=0; i<gpu_pl_lst.size(); ++i){ SDL_ReleaseGPUGraphicsPipeline(cfg_ptr->gpu_dev,gpu_pl_lst[i]); }

    std::cerr << "Deleting " << gpu_dev_lst.size() << " gpu devices\n";
    for(i16 i=0; i<gpu_dev_lst.size(); ++i){ SDL_DestroyGPUDevice(gpu_dev_lst[i]); }

    std::cerr << "Deleting " << win_lst.size() << " windows\n";
    for(i16 i=0; i<win_lst.size(); ++i){ SDL_DestroyWindow(win_lst[i]); }

    SDL_Quit();
    std::cerr << "SDL3 sytems released\n";
}

rt SDL3_Wrap::create_window(const char *_title, i32 _width, i32 _height, SDL_WindowFlags _flags, bool _set_cfg){
    SDL_Window* temp = SDL_CreateWindow(_title,_width,_height,_flags);
    if(!temp){
	dbg("CreateWindow failed",SDL_ET);
	return FAIL_CREATE_WINDOW;
    }
    else{
	win_lst.push_back(temp);
	if(_set_cfg) cfg_ptr->win = temp;
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
	if(_set_cfg) cfg_ptr->gpu_dev = temp; 
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

rt SDL3_Wrap::create_vertex_buffer(SDL_GPUDevice *_device, SDL_GPUBufferCreateInfo *_buf_nfo){
   SDL_GPUBuffer* temp = nullptr;
   temp = SDL_CreateGPUBuffer(cfg_ptr->gpu_dev,_buf_nfo);
   if(!temp){
       dbg("CreateGPUBuffer failed",SDL_ET);
       return FAIL_CREATE_VERTEX_BUFFER;
   }
   else{
       vertex_buf_lst.push_back(temp);
       return vertex_buf_lst.size()-1;
   }
}

rt SDL3_Wrap::create_transfer_buffer(SDL_GPUDevice *_device, SDL_GPUTransferBufferCreateInfo *_buf_nfo){
   SDL_GPUTransferBuffer* temp = nullptr;
   temp = SDL_CreateGPUTransferBuffer(cfg_ptr->gpu_dev,_buf_nfo);
   if(!temp){
       dbg("CreateGPUBuffer failed",SDL_ET);
       return FAIL_CREATE_TRANSFER_BUFFER;
   }
   else{
       tx_buf_lst.push_back(temp);
       return tx_buf_lst.size()-1;
   }
}

rt SDL3_Wrap::do_copy_pass(){
    SDL_GPUCommandBuffer *cmd_buf = nullptr;
    SDL_GPUCopyPass *copy_pass = nullptr;
    SDL_GPUTransferBufferLocation location{};
    SDL_GPUBufferRegion region{};

    rt r = OKAY;

    r = acquire_gpu_command_buffer(cfg_ptr->gpu_dev,&cmd_buf);
    
    copy_pass = SDL_BeginGPUCopyPass(cmd_buf);

    location.transfer_buffer = tx_buf_lst.size()?tx_buf_lst[0]:nullptr;
    location.offset = 0;

    region.buffer = vertex_buf_lst.size()?vertex_buf_lst[0]:nullptr;
    region.size = sizeof(triangle_verts);
    region.offset = 0;

    SDL_UploadToGPUBuffer(copy_pass, &location, &region, true);

    SDL_EndGPUCopyPass(copy_pass);
    if(r<0) SDL_SubmitGPUCommandBuffer(cmd_buf);
    else if(!SDL_SubmitGPUCommandBuffer(cmd_buf)) r = FAIL_SUBMIT_GPU_COMMAND;

    return r;
}

rt SDL3_Wrap::do_render_pass(){
    SDL_GPUCommandBuffer *cmd_buf = nullptr;
    SDL_GPUTexture *swapchain = nullptr;
    SDL_GPUColorTargetInfo col_target_nfo{};
    SDL_GPURenderPass* rend_pass = nullptr;
    // ui32 width=0; ui32 height=0;
    rt r = OKAY;

    r = acquire_gpu_command_buffer(cfg_ptr->gpu_dev,&cmd_buf);
    if(r>=0) r = wait_acquire_swapchain_texture(cmd_buf,cfg_ptr->win,&swapchain,&cfg_ptr->swap_w,&cfg_ptr->swap_h); 

    if(!swapchain){
	SDL_SubmitGPUCommandBuffer(cmd_buf);
	cfg_ptr->debug_rt = r;
	// cfg_ptr->debug_ss.str("");
	// cfg_ptr->debug_ss << r;
	return SDL_APP_FAILURE;
	// return SDL_APP_CONTINUE;
    }

    col_target_nfo.clear_color = {000/255.f,100/255.f,255/255.f,255/255.f};
    col_target_nfo.load_op = SDL_GPU_LOADOP_CLEAR;
    col_target_nfo.store_op = SDL_GPU_STOREOP_STORE;
    col_target_nfo.texture = swapchain;

    // begin render pass
    rend_pass = SDL_BeginGPURenderPass(cmd_buf,&col_target_nfo,1,nullptr);



    // draw something



    // end render pass
    SDL_EndGPURenderPass(rend_pass);

    if(r<0) SDL_SubmitGPUCommandBuffer(cmd_buf);
    else if(!SDL_SubmitGPUCommandBuffer(cmd_buf)) r = FAIL_SUBMIT_GPU_COMMAND;

    return r;
}
