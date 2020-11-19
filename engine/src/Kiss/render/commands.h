#pragma once
#include "Kiss/pch.h"
#include "quadbatcher.h"

namespace kiss 
{
	namespace gfx2d 
	{
		//TODO paging
		//(I should be able to request a big amount of memory and return it in the same frame)
		template <typename VData>
		class commandBuffer
		{	
			struct sprite_t { sprId i; f32 x; f32 y;};
			struct sprite_scale_t : sprite_t { f32 sx; f32 sy; };
			struct sprite_rot_t : sprite_t { rot r; };
			struct sprite_rot_scale : sprite_t { rot r; f32 sx; f32 sy; };
			struct scale9_t { sprId i; aabb s; };
			struct text_t { const char* s; };
			struct caption_t :text_t { f32 x; f32 y; };
			struct scissor_t{ int x; int y; int w; int h; };
			struct VDataQuad { VData vd[4]; };

			u8*		data;
			size_t	size;
			size_t	write_pos = 0;
			size_t	read_pos = 0;

			template <typename T>
			inline T& read() 
			{
				T& r = *((T*)(data + (u32)read_pos));
				read_pos += sizeof(T);
				return r;
			}

			inline char* readString() 
			{
				auto a = read<u32>();//string lengh with terminator.
				char * string = (char*)(data + read_pos);
				read_pos += a;
				return string;
			}

			template <typename T>
			inline commandBuffer& operator << (T s) 
			{
				//G_ASSERT((write_pos + sizeof(T)) < size, "undersized buffer");
				*(T*)(data + write_pos) = s;
				write_pos += sizeof(T);
				return *this;
			}

			inline commandBuffer& operator << (const char * s)
			{
				u32 a = (u32)(strlen(s) + 1);
				//G_ASSERT((write_pos + sizeof(u32) + a) < size, "undersized buffer");
				*this << a;
				memcpy((char*)(data + write_pos), s, a);
				write_pos += a;
				return *this;
			}

			public:
				
			inline void reset() 
			{
				write_pos = 0;
				read_pos = 0;
			}

			enum class cmd : u8
			{
				sprite = 0,
				sprite_rot,
				sprite_scale,
				sprite_rot_scale,
				scale9,
				scale9x,
				scale9y,
				textCtx,
				text,
				caption,
				vertexData,
				vertexData4,
				font,
				scissor,
				atlas,
			};

			void execute() 
			{
				textCtx text_ctx(0,0);
				while (read_pos < write_pos) 
				{
					switch (read<cmd>()) 
					{
						case cmd::sprite: {
							auto& s = read<sprite_t>();
							batcher.sprite(s.i, s.x, s.y);
							break;
						}
						case cmd::scale9: {
							auto & s = read<scale9_t>();
							batcher.scale9(s.i, s.s);
							break;
						}
						case cmd::vertexData: {
							auto s = read<VData>();
							batcher.vertexdata(s);
							break;
						}
						case cmd::vertexData4: {
							auto s = read<VDataQuad>();
							batcher.vertexdata((VData*)&s);
							break;
						}
						case cmd::textCtx: {
							text_ctx = read<textCtx>();
							break;
						}
						case cmd::text: {
							batcher.text(text_ctx, readString());
							break;
						}
						case cmd::font: {
							batcher.font(read<u8>());
							break;
						}
						case cmd::scissor: {
							auto s = read<scissor_t>();
							batcher.scissor(s.x, s.y, s.w, s.h);
							break;
						}
						default:
							break;
					}
				}
				//G_ASSERT(read_pos == write_pos, "amount of data written and read mismatch");
				read_pos = 0;
			}
	
			commandBuffer(u8 * ptr, u32 size) :
				size(size), 
				read_pos(0),
				write_pos(0),
				data(ptr)
			{}

			commandBuffer& textblock(float x, float y, float w = 1000) {
				return *this << cmd::textCtx << textCtx(x,y,w);
			}

			commandBuffer& textblock(textCtx ctx) {
				return *this << cmd::textCtx << ctx;
			}

			commandBuffer& vertexdata(VData s) {
				return *this << cmd::vertexData << s;
			}

			commandBuffer& vertexdata(VData* v) {
				VDataQuad vd;
				memcpy(&vd, v, sizeof(VDataQuad));
				return *this << cmd::vertexData4 << vd;
			}

			commandBuffer& vertexdata(VData v0, VData v1, VData v2, VData v3) {
				VDataQuad vd;
				return *this << cmd::vertexData4 << vd;
			}

			commandBuffer& font(u8 s) {
				return *this << cmd::font << s;
			}

			commandBuffer& sprite(sprId id,f32 x,f32 y)
			{
				return *this << cmd::sprite << sprite_t{id,x,y};
			}

			commandBuffer& sprite(sprId id, f32 x, f32 y, rot r) {
				return *this << cmd::sprite_rot << sprite_rot_t{id, x, y, r};
			}

			commandBuffer& scale9(sprId id, f32 minx, f32 miny, f32 maxx, f32 maxy) {
				return *this << cmd::scale9 << scale9_t{ id, aabb(minx, miny, maxx, maxy)};
			}

			commandBuffer& scale9x(sprId id, f32 minx, f32 miny, f32 maxx, f32 maxy) {
				return *this << cmd::scale9x << scale9_t{ id, aabb(minx, miny, maxx, maxy) };
			}

			commandBuffer& scale9y(sprId id, f32 minx, f32 miny, f32 maxx, f32 maxy) {
				return *this << cmd::scale9y << scale9_t{ id, aabb(minx, miny, maxx, maxy) };
			}

			commandBuffer& text(const char* s) {
				return *this << cmd::text << s;
			}

			commandBuffer& caption(f32 x, f32 y, const char* s) {
				return *this << cmd::caption << v2(x, y) << s;
			}
				
			commandBuffer& scissor(int x, int y, int w, int h) {
				scissor_t s = { x, y, w, h };
				return *this << cmd::scissor << s;
			}
		};
	}
}