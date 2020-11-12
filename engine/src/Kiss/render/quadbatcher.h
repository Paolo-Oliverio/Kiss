#pragma once
#include <Kiss/pch.h>
#include <Kiss/data/atlas.h>
#include <Kiss/math/v2.h>
#include <Kiss/math/aabb.h>
#include <Kiss/math/rot.h>
#include <kinc/graphics4/graphics.h>
#include <kinc/graphics4/vertexbuffer.h>
#include <kinc/graphics4/vertexstructure.h>
#include <kinc/graphics4/indexbuffer.h>
#include <kinc/graphics4/pipeline.h>
#include <kinc/graphics4/texture.h>
#include <cstring>
#include "Kiss/render/gfx2dDefs.h"

//TODO assert add_sprite out of range

namespace kiss 
{
	typedef u16 sprId;
	struct atlas;

	constexpr int	max_quads_vtx		= 0xFFFF;				//Max vertices in vb.
	constexpr int	max_quads			= (max_quads_vtx / 4);
	constexpr int	quad_buffers_num	= 4;

	static inline aabb add_position(const tile::posRect& vp, const f32 x, const f32 y) 
	{
		return aabb
		(
			v2(vp.min.x + x, vp.min.y + y),
			v2(vp.max.x + x, vp.max.y + y)
		);
	}

	static inline aabb scale_and_move(const tile::posRect& vp, const f32 x, const f32 y, const f32 sx, const f32 sy)
	{
		return aabb
		(
			v2(vp.min.x * sx + x, vp.min.y * sy + y),
			v2(vp.max.x * sx + x, vp.max.y * sy + y)
		);
	}

	template<typename Vtx, typename VData>
	inline void quad_blit(Vtx* vtx, const aabb& p, const  tile::uvRect t, const VData d);

	template<typename Vtx, typename VData>
	inline void quad_blit_rotated(Vtx* vtx, const aabb& p, const  tile::uvRect t, const  v2 pos, const  rot r, const VData d);

	//template by vertex type.
	template <typename Vtx, typename VData>
	class quadBatcher {
	private:
		struct {//Hot
			Vtx*	vertices		= nullptr;
			atlas*	atlas			= nullptr;
			u16		start			= 0;
			u16		index			= 0;// u16 ?!?
			s16		texel_ratio_x	= 0;
			s16		texel_ratio_y	= 0;
			u8		actual_buffer	= 0;
			u8		font			= 0;
		};
		gfx2d::basicPipe* Pipe;
		kinc_g4_vertex_buffer_t* vbuffer;

	private:

		inline void check_capacity(int new_vertices) 
		{
			if (index + new_vertices >= max_quads_vtx) private_flush();
		}
		
		inline u16 maxIndex()
		{
			return index / 4 * 6;
		}

		void private_flush() 
		{
			const auto vb = &vbuffer[actual_buffer];
			kinc_g4_vertex_buffer_lock(vb, 0, index);
			//TODO check other unlock
			//kinc_g4_vertex_buffer_unlock_all(vb);
			kinc_g4_vertex_buffer_unlock(&vbuffer[actual_buffer], index);
			kinc_g4_set_vertex_buffer(vb);
			kinc_g4_set_index_buffer(gfx2d::quad::ibuffer);
			kinc_g4_set_texture(Pipe->texture_unit, atlas->texture);
			kinc_g4_set_texture_magnification_filter(Pipe->texture_unit, KINC_G4_TEXTURE_FILTER_POINT);
			kinc_g4_set_texture_minification_filter(Pipe->texture_unit, KINC_G4_TEXTURE_FILTER_POINT);
			kinc_g4_draw_indexed_vertices_from_to(0, maxIndex());
			SwapBuffer();
		}

		void SwapBuffer() 
		{
			actual_buffer = (actual_buffer + 1) % quad_buffers_num;
			index = 0;
			vertices = (Vtx*)kinc_g4_vertex_buffer_lock_all(&vbuffer[actual_buffer]);
		}

	public:
		quadBatcher(gfx2d::basicPipe& pipe) :
			Pipe(&pipe), index(0), atlas(0) 
		{
			vbuffer = new kinc_g4_vertex_buffer_t[quad_buffers_num];
			for (int i = 0; i < quad_buffers_num; ++i) 
			{
				auto vb = &vbuffer[i];
				kinc_g4_vertex_buffer_init(vb, max_quads_vtx, &pipe.vertexLayout, KINC_G4_USAGE_DYNAMIC, 0);
				kinc_g4_vertex_buffer_lock_all(vb);
				kinc_g4_vertex_buffer_unlock_all(vb);
			}
		}

		~quadBatcher() 
		{
			for (int i = 0; i < quad_buffers_num; ++i) 
			{
				kinc_g4_vertex_buffer_destroy(&vbuffer[i]);
			}
			delete[] vbuffer;
		}

		void begin() 
		{
			//using namespace gfx2d;
			kinc_g4_set_pipeline(&Pipe->pipe);
			kinc_g4_set_index_buffer(gfx2d::quad::ibuffer);
			//kinc_matrix3x3_t matrix;
			//memcpy(&matrix.m, &Pipe->spriteProjection.data, sizeof(float) * 9);
			kinc_g4_set_matrix3(Pipe->proj_location, &Pipe->spriteProjection);
			index = 0;
			vertices = (Vtx*)kinc_g4_vertex_buffer_lock_all(&vbuffer[actual_buffer]);
		}

		inline void end() 
		{ 
			if (index > 0) private_flush();
		}

		void set_atlas(kiss::atlas* new_atlas) {
			auto tex = new_atlas->texture;
			if (atlas == nullptr || atlas->texture != tex) {
				if (index > 0) private_flush();
				kinc_g4_set_texture(Pipe->texture_unit, tex);
				texel_ratio_x = 0x8000 / tex->tex_width;
				texel_ratio_y = 0x8000 / tex->tex_height;
			}
			atlas = new_atlas;
		}

		inline void setFont(u8 new_font) 
		{
			font = new_font;
		}

		//Sprite Rendering
		void sprite(const sprId id, const f32 x, const f32 y, const VData d)
		{
			check_capacity(4);
			const auto& spr = atlas->sprites[id];
			quad_blit(&vertices[index], add_position(spr.P, x, y), spr.T, d);
			index += 4;
		}

		void sprite(const sprId id, const f32 x, const f32 y, const f32 sx, const f32 sy, const VData d) 
		{
			check_capacity(4);
			const auto& spr = atlas->sprites[id];
			quad_blit(&vertices[index], scale_and_move(spr.P, x, y, sx, sy), spr.T, d);
			index += 4;
		}

		void sprite(const sprId id, const f32 x, const f32 y, const rot r, const VData d) 
		{
			check_capacity(4);
			const auto& spr = atlas->sprites[id];
			quad_blit_rotated(&vertices[index], aabb(spr.P.min.x, spr.P.min.y, spr.P.max.x, spr.P.max.y), spr.T, x, y, r, d);
			index += 4;
		}

		void sprite(const sprId id, const f32 x, const f32 y, const f32 sx, const f32 sy, const  rot r, const VData d) 
		{
			check_capacity(4);
			const auto& spr = atlas->sprites[id];
			quad_blit_rotated(&vertices[index],
				aabb(spr.P.min.x * sx, spr.P.min.y * sy, spr.P.max.x * sx, spr.P.max.y * sy),
				spr.T, x, y, r, d);
			index += 4;
		}

		void scale9(const sprId id, const aabb& b, const VData d) 
		{
			check_capacity(4 * 9);
			const auto& s = atlas->scale9s[id];//load from atlas.
			const s16 u0 = s.t.min.u;
			const s16 u3 = s.t.max.u;
			const s16 v0 = s.t.min.v;
			const s16 v3 = s.t.max.v;
			const s16 nox = s.offset.left;
			const s16 pox = s.offset.right;
			const s16 noy = s.offset.up;
			const s16 poy = s.offset.down;
			const float x0 = b.min.x - nox;
			const float x1 = b.min.x;
			const float x2 = b.max.x;
			const float x3 = b.max.x + pox;
			const float y0 = b.min.y - noy;
			const float y1 = b.min.y;
			const float y2 = b.max.y;
			const float y3 = b.max.y + poy;
			const s16 u1 = (u0 + nox * texel_ratio_x);
			const s16 u2 = (u3 - pox * texel_ratio_x);
			const s16 v1 = (v0 + noy * texel_ratio_y);
			const s16 v2 = (v3 - poy * texel_ratio_y);
			volatile auto vtx = &vertices[index];
			//up
			quad_blit(vtx, aabb(x0, y0, x1, y1), { u0, v0, u1, v1 }, d);//6
			quad_blit(vtx + 4, aabb(x1, y0, x2, y1), { u1, v0, u2, v1 }, d);//7
			quad_blit(vtx + 8, aabb(x2, y0, x3, y1), { u2, v0, u3, v1 }, d);//8
			//center
			quad_blit(vtx + 12, aabb(x0, y1, x1, y2), { u0, v1, u1, v2 }, d);//3
			quad_blit(vtx + 16, aabb(x1, y1, x2, y2), { u1, v1, u2, v2 }, d);//4
			quad_blit(vtx + 20, aabb(x2, y1, x3, y2), { u2, v1, u3, v2 }, d);//5
			//bottom
			quad_blit(vtx + 24, aabb(x0, y2, x1, y3), { u0, v2, u1, v3 }, d);//0
			quad_blit(vtx + 28, aabb(x1, y2, x2, y3), { u1, v2, u2, v3 }, d);//1
			quad_blit(vtx + 32, aabb(x2, y2, x3, y3), { u2, v2, u3, v3 }, d);//2
			//update quad index.
			index += 36;
		}

		void scale9X(const sprId id, const aabb& b, const VData d)
		{
			check_capacity(12);
			const auto& s = atlas->scale9s[id];
			const s16 u0 = s.t.min.u;
			const s16 u3 = s.t.max.u;
			const s16 v0 = s.t.min.v;
			const s16 v1 = s.t.max.v;
			const s16 nox = s.offset.left;
			const s16 pox = s.offset.right;
			const float x0 = b.min.x - nox;
			const float x1 = b.min.x;
			const float x2 = b.max.x;
			const float x3 = b.max.x + pox;
			const float y0 = b.min.y;
			const float y1 = (y0 + ((v1 - v0) / texel_ratio_y));
			const s16 u1 = (u0 + nox * texel_ratio_x);
			const s16 u2 = (u3 - pox * texel_ratio_x);
			volatile auto vtx = &vertices[index];
			quad_blit(vtx, aabb(x0, y0, x1, y1), { u0, v0, u1, v1 }, d);
			quad_blit(vtx + 4, aabb(x1, y0, x2, y1), { u1, v0, u2, v1 }, d);
			quad_blit(vtx + 8, aabb(x2, y0, x3, y1), { u2, v0, u3, v1 }, d);
			index += 12;
		}

		void scale9Y(const sprId id, const aabb& b, const VData d) {
			check_capacity(12);
			const auto& s = atlas->scale9s[id];
			const s16 u0 = s.t.min.u; s16 u1 = s.t.max.u; s16 v0 = s.t.min.v; s16 v3 = s.t.max.v;
			const s16 noy = s.offset.up; s16 poy = s.offset.down;
			const float y0 = b.min.y - noy;
			const float y1 = b.min.y;
			const float y2 = b.max.y;
			const float y3 = b.max.y + poy;
			const float x0 = b.min.x;
			const float x1 = (b.min.x + ((u1 - u0) / texel_ratio_x));
			const s16 v1 = (v0 + noy * texel_ratio_y);
			const s16 v2 = (v3 - poy * texel_ratio_y);
			volatile auto vtx = &vertices[index];
			quad_blit(vtx, aabb(x0, y2, x1, y3), { u0, v2, u1, v3 }, d);
			quad_blit(vtx + 4, aabb(x0, y1, x1, y2), { u0, v1, u1, v2 }, d);
			quad_blit(vtx + 8, aabb(x0, y0, x1, y1), { u0, v0, u1, v1 }, d);
			index += 12;
		}

		void text(textCtx& ts, const char* text, const VData d) 
		{
			int character = *text++;
			if (character == 0) return;
			const atlas::font& f = atlas->fonts[font];
			const atlas::sprite* s = &atlas->chars[f.firstChar];
			const float lineHeight = (float)f.lineHeight;
			auto vtx = &vertices[index];
			int id;
			do {
				if (character > 127) 
				{
					//New Line
					ts.x = ts.start_x;
					ts.y += f.lineHeight;
				}
				else if (character >= f.start) 
				{
					if (index >= max_quads_vtx) 
					{
						private_flush();
						vtx = &vertices[index];
					}
					id = character - f.start;
					auto& rune = s[id];
					const aabb runePos = add_position(rune.P, ts.x ,ts.y);
					quad_blit(vtx, runePos, rune.T, d);
					vtx += 4;
					index += 4;
					ts.x += f.kerning;
				}
				else if (ts.x > ts.w) 
				{
					ts.x = ts.start_x;
					ts.y += f.lineHeight;
				}
				else 
				{
					ts.x += f.kerning;
				}
				character = *text++;
			} 
			while (character != 0);
		}

		void caption(const char* text, v2 pos, const VData d) 
		{
			int character = *text++;
			if (character == 0) return;
			const atlas::font& f = atlas->fonts[font];
			volatile auto vtx = &vertices[index];
			const atlas::sprite* s = &atlas->chars[f.firstChar];
			float dx = pos.x; int id;
			do {
				if (character >= f.start) 
				{
					if (index >= max_quads_vtx) 
					{
						private_flush();
						vtx = &vertices[index];
					}
					id = character - f.start;
					auto& rune = s[id];
					aabb runePos = add_position(rune.P, dx, pos.y);
					quad_blit(vtx, runePos, rune.T, d);
					vtx += 4;
					index += 4;
				}
				dx += f.kerning;
				character = *text++;
			}
			while (character != 0);
		}

		int textLineLenght(const char* text) const {
			int i = 0;
			while (*text++ != 0) i++;
			auto f = atlas->fonts[font];
			return i * f.kerning;
		}
	};

	template<typename Vtx, typename VData>
	void quad_blit(Vtx* vtx, const aabb& p, const  tile::uvRect t, const VData d) 
	{
		const Vtx vty[4] = 
		{
			{p.min.x, p.min.y, t.min.u, t.min.v, d},
			{p.max.x, p.min.y, t.max.u, t.min.v, d},
			{p.min.x, p.max.y, t.min.u, t.max.v, d},
			{p.max.x, p.max.y, t.max.u, t.max.v, d}
		};
		std::memcpy(vtx, vty, sizeof(Vtx) * 4);
	}

	template<typename Vtx, typename VData>
	void quad_blit_rotated(Vtx* vtx, aabb& pos, const  tile::uvRect t, const float x, const float y, const rot r, const VData d)
	{
		const aabb cos = pos * r.c;
		const aabb sin = pos * r.s;
		const Vtx vty[4] =
		{
			{ (cos.min.x - sin.min.y) + x, (cos.min.y + sin.min.x) + y, t.min.u, t.min.v, d},
			{ (cos.max.x - sin.min.y) + x, (cos.min.y + sin.max.x) + y, t.max.u, t.min.v, d },
			{ (cos.min.x - sin.max.y) + x, (cos.max.y + sin.min.x) + y, t.min.u, t.max.v, d },
			{ (cos.max.x - sin.max.y) + x, (cos.max.y + sin.max.x) + y, t.max.u, t.max.v, d }
		};
		std::memcpy(vtx, vty, sizeof(Vtx) * 4);
	}
}
