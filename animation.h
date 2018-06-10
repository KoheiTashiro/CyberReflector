#pragma once
#include "interface.h"

class Anime
{
public:
	Anime(const Texture& texture, int size, int frame);

	void update();
	

	void draw(const Vec2& pos) const;
	
	void draw(const Vec2& pos,double scale,double rotate) const;

	int m_frame;
	int m_size;
	int m_index;


private:
	Texture m_texture;
	int m_count;
};