#pragma once
#include "Kiss/pch.h"
#include "quadbatcher.h"
//#include <string>

//#include "entt/core/family.hpp"

//using ctype = entt::family<struct commandtype>;

namespace kiss 
{
	namespace gfx2d 
	{
		namespace command 
		{

			struct sprite_t 
			{
				sprId i;
				f32 x;
				f32 y;
			};

			struct sprite_scale_t : sprite_t 
			{
				f32 sx;
				f32 sy;
			};

			struct sprite_rot_t : sprite_t 
			{
				rot r;
			};

			struct sprite_rot_scale : sprite_t 
			{
				rot r;
				f32 sx;
				f32 sy;
			};

			struct scale9_t 
			{
				sprId i;
				aabb s;
			};

			struct scale9_horizontal_t : scale9_t {};
			struct scale9_vertical_t : scale9_t {};

			struct text_t 
			{
				const char* s;
			};

			struct caption_t :text_t 
			{
				f32 x;
				f32 y;
			};

			enum class cmd : u8 
			{
				sprite = 0,
				sprite_rot,
				sprite_scale,
				sprite_rot_scale,
				scale9,
				scale9_horizontal,
				scale9_vertical,
				textCtx,
				text,
				caption,
				vertexData,
				vertexData4,
				font,
				scissor,
			};



			//TODO paging
			//(I should be able to request a big amount of memory and return it in the same frame)
			template <typename VData>
			class buffer
			{
				u8*		data;
				size_t	size;
				size_t	write_pos = 0;
				size_t	read_pos = 0;

				struct VDataQuad {
					VData vd[4];
				};
				
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
				inline buffer& operator << (T s) 
				{
					//G_ASSERT((write_pos + sizeof(T)) < size, "undersized buffer");
					*(T*)(data + write_pos) = s;
					//memcpy(b.data + b.write_pos, &s, sizeof(T));
					write_pos += sizeof(T);
					return *this;
				}

				buffer& operator << (const char * s) 
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

				void execute() 
				{
					auto & quad = *quad::batcher;
					textCtx text_ctx(0,0);
					while (read_pos < write_pos) 
					{
						switch (read<cmd>()) 
						{
							case cmd::sprite :
							{	
								auto & s = read<sprite_t>();
								quad.sprite(s.i, s.x, s.y);
								break;
							}
							case cmd::scale9: 
							{
								auto & s = read<scale9_t>();
								quad.scale9(s.i, s.s);
								break;
							}
							case cmd::vertexData:
							{
								auto s = read<VData>();
								quad.setVData(s);
								break;
							}
							case cmd::vertexData4:
							{
								auto s = read<VDataQuad>();
								quad.setVData((VData*)&s);
								break;
							}
							case cmd::textCtx: 
							{
								text_ctx = read<textCtx>();
								break;
							}
							case cmd::text: {
								quad.text(text_ctx, readString());
							}break;
							case cmd::font: {
								quad.setFont(read<u8>());
							}break;
							default:
								break;
							/*case cmd::sprite_pos_rot: {
								auto & s = read<sprite_pos_rot>();
								quad.add_sprite(s.i, s.p, s.r, color);
							}break;
							case cmd::sprite_pos_scale: {
								auto & s = read<sprite_pos_scale>();
								quad.add_sprite(s.i, s.p, s.s, color);
							}break;
							case cmd::sprite_pos_rot_scale: {
								auto & s = read<sprite_pos_rot_scale>();
								quad.add_sprite(s.i, s.p, s.r, s.s, color);
							}break;
							
							case cmd::scale9_vertical: {
								auto & s = read<scale9>();
								quad.add_vertical_scale9(s.i, s.s, color);
							}break;
							case cmd::scale9_horizontal: {
								auto & s = read<scale9>();
								quad.add_horizontal_scale9(s.i, s.s, color);
							}break;
							case cmd::caption:{
								quad.caption(readString(), read<v2>(), color);
							}break;
							
							case cmd::scissor:{
								//G4::SetScissor(r<aabb>());
							}break;*/
						}
					}
					//G_ASSERT(read_pos == write_pos, "amount of data written and read mismatch");
					read_pos = 0;
				}

				/*~buffer() {
					if (data)delete[] data;
				}*/
	
				buffer(u8 * ptr, u32 size) :
					size(size), 
					read_pos(0),
					write_pos(0),
					data(ptr)
				{}

				inline buffer& textblock(float x, float y, float w = 1000){
					return *this << cmd::textCtx << textCtx(x,y,w);
				}
				inline buffer& textblock(textCtx ctx) {
					return *this << cmd::textCtx << ctx;
				}
				inline buffer& color(VData s) {
					return *this << cmd::vertexData << s;
				}
				inline buffer& color(VData* v) {
					VDataQuad vd;
					memcpy(&vd, v, sizeof(VDataQuad));
					return *this << cmd::vertexData4 << vd;
				}
				inline buffer& color(VData v0, VData v1, VData v2, VData v3) {
					VDataQuad vd;
					return *this << cmd::vertexData4 << vd;
				}
				inline buffer& font(u8 s) {
					return *this << cmd::font << s;
				}
				inline buffer& sprite(sprId id,f32 x,f32 y) {
					return *this << cmd::sprite << sprite_t{id,x,y};
				}
				inline buffer& sprite(sprId id, f32 x, f32 y, rot r) {
					return *this << cmd::sprite_rot << sprite_rot_t{id, x, y, r};
				}
			/*	buffer& add(sprite_scale s) {
					return *this << cmd::sprite_scale << s;
				}
				buffer& add(sprite_rot_scale s) {
					return *this << cmd::sprite_rot_scale << s;
				}*/
				inline buffer& scale9(sprId id, f32 minx, f32 miny, f32 maxx, f32 maxy){
					return *this << cmd::scale9 << scale9_t{ id, aabb(minx, miny, maxx, maxy)};
				}
				inline buffer& scale9x(sprId id, f32 minx, f32 miny, f32 maxx, f32 maxy){
					return *this << cmd::scale9_horizontal << scale9_t{ id, aabb(minx, miny, maxx, maxy) };
				}
				inline buffer& scale9y(sprId id, f32 minx, f32 miny, f32 maxx, f32 maxy) {
					return *this << cmd::scale9_vertical << scale9_t{ id, aabb(minx, miny, maxx, maxy) };
				}
				inline buffer& text(const char* s) {
					return *this << cmd::text << s;
				}
				inline buffer& caption(f32 x, f32 y, const char* s) {
					return *this << cmd::caption << v2(x,y) << s;
				}
			};
		};
	}
}