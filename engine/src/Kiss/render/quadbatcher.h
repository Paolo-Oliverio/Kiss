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
#include "gfx2dDefs.h"
#include "bufferManager.h"

//TODO assert add_sprite out of range

#define NEWBUFFERONDRAW false

namespace kiss 
{
	typedef u16 sprId;
	struct atlas;

	static inline aabb add_position(const tile::posRect& vp, const f32 x, const f32 y) 
	{
		return aabb (
			v2(vp.min.x + x, vp.min.y + y),
			v2(vp.max.x + x, vp.max.y + y)
		);
	}

	static inline aabb scale_and_move(const tile::posRect& vp, const f32 x, const f32 y, const f32 sx, const f32 sy)
	{
		return aabb (
			v2(vp.min.x * sx + x, vp.min.y * sy + y),
			v2(vp.max.x * sx + x, vp.max.y * sy + y)
		);
	}

	//template by vertex type.
	template <typename Vtx, typename VData,VData vDataDef>
	class quadBatcher {
	private:
		
		VData	vdata[4];
		Vtx*	vertices		= nullptr;
		atlas*	actual_atlas	= nullptr;
		u16		start			= 0;
		u16		index			= 0;
		u8		actual_font		= 0;

		gfx2d::pipeline2d* pipe = nullptr;
		kinc_g4_vertex_buffer_t* vbuffer;

		s16		texel_ratio_x	= 0;
		s16		texel_ratio_y	= 0;

	private:

		inline void check_capacity(u32 new_vertices) 
		{
			if ((u32)index + new_vertices >= pipe->bufferManager->size) private_flush ();
		}
		
		inline u16 maxIndex()
		{
			return index / 4 * 6;
		}

		void private_flush(bool full = true) 
		{
			kinc_g4_vertex_buffer_lock(vbuffer, 0, index);
			kinc_g4_vertex_buffer_unlock(vbuffer, index);

			kinc_g4_set_vertex_buffer(vbuffer);
			//kinc_g4_set_index_buffer(gfx2d::quad::ibuffer);

			kinc_g4_set_texture(pipe->texture_unit, actual_atlas->texture);
			kinc_g4_set_texture_magnification_filter(pipe->texture_unit, KINC_G4_TEXTURE_FILTER_POINT);
			kinc_g4_set_texture_minification_filter(pipe->texture_unit, KINC_G4_TEXTURE_FILTER_POINT);
			auto end = index / 4 * 6;
			kinc_g4_draw_indexed_vertices_from_to(start, end);
			if (full) {
				swapBuffer();
			} else {
				start = end;
			}
		}

		void swapBuffer() 
		{
			vbuffer = pipe->bufferManager->get();
			start = 0;
			index = 0;
			vertices = (Vtx*)kinc_g4_vertex_buffer_lock_all(vbuffer);
		}

	public:
		quadBatcher() : actual_atlas(nullptr), index(0) {}

		void begin(gfx2d::pipeline2d* pipe, atlas* textureAtlas)
		{
			using namespace gfx2d;
			atlas(textureAtlas);
			this->pipe = pipe;
			start = 0;
			index = 0;
			vdata[0] = vdata[1] = vdata[2] = vdata[3] = vDataDef;
			kinc_g4_set_pipeline(&pipe->pipe);
			kinc_g4_set_index_buffer(&ibuffer::quads);
			kinc_g4_set_matrix3(pipe->proj_location, &xform::sprite);
			vbuffer = pipe->bufferManager->get();
			vertices = (Vtx*)kinc_g4_vertex_buffer_lock_all(vbuffer);
		}

		inline void end() 
		{ 
			if (index > 0) private_flush();
		}

		void vertexdata(VData vd) 
		{
			vdata[0] = vdata[1] = vdata[2] = vdata[3] = vd;
		}

		void vertexdata(VData vd0, VData vd1, VData vd2, VData vd3)
		{
			vdata[0] = vd0;
			vdata[1] = vd1;
			vdata[2] = vd2;
			vdata[3] = vd3;
		}

		void vertexdata(VData* vd)
		{
			memcpy(vdata, vd, sizeof(VData) * 4);
		}

		void atlas(kiss::atlas* new_atlas) 
		{
			auto tex = new_atlas->texture;
			if (actual_atlas != nullptr && actual_atlas->texture == tex)
			{
				actual_atlas = new_atlas;
				return;
			}
			texel_ratio_x = 0x8000 / tex->tex_width;
			texel_ratio_y = 0x8000 / tex->tex_height;
			if (actual_atlas != nullptr && index > 0) private_flush(NEWBUFFERONDRAW);
			actual_atlas = new_atlas;
		}

		inline void font(u8 new_font) 
		{
			actual_font = new_font;
		}

		//Sprite Rendering
		void sprite(const sprId id, const f32 x, const f32 y)
		{
			check_capacity(4);
			const auto& spr = actual_atlas->sprites[id];
			quad_blit(&vertices[index], add_position(spr.P, x, y), spr.T);
			index += 4;
		}

		void sprite(const sprId id, const f32 x, const f32 y, const f32 sx, const f32 sy) 
		{
			check_capacity(4);
			const auto& spr = actual_atlas->sprites[id];
			quad_blit(&vertices[index], scale_and_move(spr.P, x, y, sx, sy), spr.T);
			index += 4;
		}

		void sprite(const sprId id, const f32 x, const f32 y, const rot r) 
		{
			check_capacity(4);
			const auto& spr = actual_atlas->sprites[id];
			quad_blit_rotated(&vertices[index], aabb(spr.P.min.x, spr.P.min.y, spr.P.max.x, spr.P.max.y), spr.T, x, y, r);
			index += 4;
		}

		void sprite(const sprId id, const f32 x, const f32 y, const f32 sx, const f32 sy, const  rot r) 
		{
			check_capacity(4);
			const auto& spr = actual_atlas->sprites[id];
			quad_blit_rotated(&vertices[index],
				aabb(spr.P.min.x * sx, spr.P.min.y * sy, spr.P.max.x * sx, spr.P.max.y * sy),
				spr.T, x, y, r);
			index += 4;
		}

		void scale9(const sprId id, const aabb& b) 
		{
			check_capacity(4 * 9);
			const auto& s = actual_atlas->scale9s[id];//load from atlas.
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
			quad_blit(vtx, aabb(x0, y0, x1, y1),		{{u0, v0}, {u1, v1}}, 0, 0, 0, 0);//6
			quad_blit(vtx, aabb(x0, y0, x1, y1),		{{u0, v0}, {u1, v1}}, 0, 0, 0, 0);//6
			quad_blit(vtx + 4, aabb(x1, y0, x2, y1),	{{u1, v0}, {u2, v1}}, 0, 1, 0, 1);//7
			quad_blit(vtx + 8, aabb(x2, y0, x3, y1),	{{u2, v0}, {u3, v1}}, 1, 1, 1, 1);//8
			//center
			quad_blit(vtx + 12, aabb(x0, y1, x1, y2),	{{u0, v1}, {u1, v2}}, 0, 0, 2, 2);//3
			quad_blit(vtx + 16, aabb(x1, y1, x2, y2),	{{u1, v1}, {u2, v2}}, 0, 1, 2, 3);//4
			quad_blit(vtx + 20, aabb(x2, y1, x3, y2),	{{u2, v1}, {u3, v2}}, 1, 1, 3, 3);//5
			//bottom
			quad_blit(vtx + 24, aabb(x0, y2, x1, y3),	{{u0, v2}, {u1, v3}}, 2, 2, 2, 2);//0
			quad_blit(vtx + 28, aabb(x1, y2, x2, y3),	{{u1, v2}, {u2, v3}}, 2, 3, 2, 3);//1
			quad_blit(vtx + 32, aabb(x2, y2, x3, y3),	{{u2, v2}, {u3, v3}}, 3, 3, 3, 3);//2
			//update quad index.
			index += 36;
		}

		void scale9x(const sprId id, const aabb& b)
		{
			check_capacity(12);
			const auto& s = actual_atlas->scale9s[id];
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
			quad_blit(vtx, aabb(x0, y0, x1, y1), { {u0, v0}, {u1, v1} }, 0, 0, 2, 2);
			quad_blit(vtx + 4, aabb(x1, y0, x2, y1), { {u1, v0}, {u2, v1} }, 0, 1, 2, 3);
			quad_blit(vtx + 8, aabb(x2, y0, x3, y1), { {u2, v0}, {u3, v1} }, 1, 1, 3, 3);
			index += 12;
		}

		void scale9y(const sprId id, const aabb& b) {
			check_capacity(12);
			const auto& s = actual_atlas->scale9s[id];
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
			quad_blit(vtx, aabb(x0, y2, x1, y3), { {u0, v2}, {u1, v3} }, 0, 1, 0, 1);
			quad_blit(vtx + 4, aabb(x0, y1, x1, y2), { {u0, v1}, {u1, v2} }, 0, 1, 2, 3);
			quad_blit(vtx + 8, aabb(x0, y0, x1, y1), { {u0, v0}, {u1, v1} }, 2, 3, 2, 3);
			index += 12;
		}

		void text(textCtx& ts, const char* text) 
		{
			int character = *text++;
			if (character == 0) return;
			const atlas::font& f = actual_atlas->fonts[actual_font];
			const atlas::sprite* s = &actual_atlas->chars[f.firstChar];
			const float lineHeight	= (float)f.lineHeight;
			const float kerning		= (float)f.kerning;
			auto vtx = &vertices[index];
			int id;
			do {
				if (character > 127) 
				{
					//New Line
					ts.x = ts.start_x;
					ts.y += lineHeight;
				}
				else if (character >= f.start) 
				{
					if (index >= pipe->bufferManager->size)
					{
						private_flush();
						vtx = &vertices[index];
					}
					id = character - f.start;
					auto& rune = s[id];
					const aabb runePos = add_position(rune.P, ts.x ,ts.y);
					quad_blit(vtx, runePos, rune.T);
					vtx += 4;
					index += 4;
					ts.x += kerning;
				}
				else if (ts.x > ts.w) 
				{
					ts.x = ts.start_x;
					ts.y += lineHeight;
				}
				else 
				{
					ts.x += kerning;
				}
				character = *text++;
			} 
			while (character != 0);
		}

		void caption(const char* text, v2 pos) 
		{
			int character = *text++;
			if (character == 0) return;
			const atlas::font& f = actual_atlas->fonts[actual_font];
			volatile auto vtx = &vertices[index];
			const atlas::sprite* s = &actual_atlas->chars[f.firstChar];
			const float kerning = (float)f.kerning;
			float dx = pos.x; int id;
			do {
				if (character >= f.start) 
				{
					if (index >= pipe->bufferManager->size)
					{
						private_flush();
						vtx = &vertices[index];
					}
					id = character - f.start;
					auto& rune = s[id];
					aabb runePos = add_position(rune.P, dx, pos.y);
					quad_blit(vtx, runePos, rune.T);
					vtx += 4;
					index += 4;
				}
				dx += kerning;
				character = *text++;
			}
			while (character != 0);
		}

		int textLineLenght(const char* text) const {
			int i = 0;
			while (*text++ != 0) i++;
			auto f = actual_atlas->fonts[actual_font];
			return i * f.kerning;
		}

		void scissor(int x, int y, int w, int h) {
			private_flush(NEWBUFFERONDRAW);
			kinc_g4_scissor(x, y, w, h);
		}
	
	private:

		void quad_blit(Vtx* vtx, const aabb& p, const  tile::uvRect t) 
		{
			const Vtx vty[4] = 
			{
				{p.min.x, p.min.y, t.min.u, t.min.v, vdata[0]},
				{p.max.x, p.min.y, t.max.u, t.min.v, vdata[1]},
				{p.min.x, p.max.y, t.min.u, t.max.v, vdata[2]},
				{p.max.x, p.max.y, t.max.u, t.max.v, vdata[3]}
			};
			memcpy(vtx, vty, sizeof(Vtx) * 4);
		}

		void quad_blit(Vtx* vtx, const aabb& p, const  tile::uvRect t, int v0, int v1, int v2, int v3)
		{
			const Vtx vty[4] =
			{
				{p.min.x, p.min.y, t.min.u, t.min.v, vdata[v0]},
				{p.max.x, p.min.y, t.max.u, t.min.v, vdata[v1]},
				{p.min.x, p.max.y, t.min.u, t.max.v, vdata[v2]},
				{p.max.x, p.max.y, t.max.u, t.max.v, vdata[v3]}
			};
			memcpy(vtx, vty, sizeof(Vtx) * 4);
		}

		void quad_blit_rotated(Vtx* vtx, const aabb& pos, const  tile::uvRect t, const float x, const float y, const rot r)
		{
			const aabb cos = pos * r.c;
			const aabb sin = pos * r.s;
			const Vtx vty[4] =
			{
				{ (cos.min.x - sin.min.y) + x, (cos.min.y + sin.min.x) + y, t.min.u, t.min.v, vdata[0]},
				{ (cos.max.x - sin.min.y) + x, (cos.min.y + sin.max.x) + y, t.max.u, t.min.v, vdata[1]},
				{ (cos.min.x - sin.max.y) + x, (cos.max.y + sin.min.x) + y, t.min.u, t.max.v, vdata[2]},
				{ (cos.max.x - sin.max.y) + x, (cos.max.y + sin.max.x) + y, t.max.u, t.max.v, vdata[3]}
			};
			memcpy(vtx, vty, sizeof(Vtx) * 4);
		}
	};
}