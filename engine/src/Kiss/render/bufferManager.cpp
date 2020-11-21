#include <Kiss/pch.h>
#include <kinc/graphics4/vertexbuffer.h>
#include <kinc/graphics4/indexbuffer.h>
#include "bufferManager.h"

namespace kiss 
{
	void vbRing::init(kinc_g4_vertex_structure_t* type, u32 size, u16 num)
	{
		this->size = size;
		this->num = num;
		buffers = new kinc_g4_vertex_buffer_t[num];
		for (int i = 0; i < num; ++i)
		{
			kinc_g4_vertex_buffer_init(&buffers[i], size, type, KINC_G4_USAGE_DYNAMIC, 0);
		}
	}

	void vbRing::release() {
		if (buffers != nullptr) {
			for (int i = 0; i < num; ++i)
			{
				kinc_g4_vertex_buffer_destroy(&buffers[i]);
			}
			delete[] buffers;
		}
	}

	kinc_g4_vertex_buffer_t* vbRing::get() {
		lastused = (lastused + 1) % num;
		return &buffers[lastused];
	}

	void ibRing::init(u32 size, u16 num)
	{
		this->size = size;
		this->num = num;
		buffers = new kinc_g4_index_buffer_t[num];
		for (int i = 0; i < num; ++i)
		{
			kinc_g4_index_buffer_init(&buffers[i], size, KINC_G4_INDEX_BUFFER_FORMAT_16BIT);
		}
	}

	void ibRing::release() {
		if (buffers != nullptr) {
			for (int i = 0; i < num; ++i)
			{
				kinc_g4_index_buffer_destroy(&buffers[i]);
			}
			delete[] buffers;
		}
	}

	kinc_g4_index_buffer_t* ibRing::get() {
		lastused = (lastused + 1) % num;
		return &buffers[lastused];
	}
}