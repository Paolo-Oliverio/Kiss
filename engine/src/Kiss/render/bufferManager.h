#pragma once
#include <Kiss/pch.h>

FWD_KINC(g4_vertex_structure);
FWD_KINC(g4_vertex_buffer);
FWD_KINC(g4_index_buffer);

namespace kiss 
{
	struct vbRing
	{
		void init(kinc_g4_vertex_structure_t* type, u32 size, u16 num);
		void release();
		kinc_g4_vertex_buffer_t* get();	
		u32 size = 0;

	private:
		kinc_g4_vertex_buffer_t* buffers = nullptr;
		
		u16 lastused = 0;
		u16 num = 0;
	};

	struct ibRing
	{
		void init(u32 size, u16 num);
		void release();
		kinc_g4_index_buffer_t* get();
		u32 size = 0;
	private:
		kinc_g4_index_buffer_t* buffers = nullptr;
		
		u16 lastused = 0;
		u16 num = 0;
	};
}