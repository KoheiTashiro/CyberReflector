#include "interface.h"

	Anime::Anime(const Texture& texture, int size, int frame) :
		m_texture(texture),
		m_size(size),
		m_frame(frame),
		m_index(0),
		m_count(0) {}

	void Anime::update()
	{
		++m_count;

		if (m_count > m_frame)
		{
			m_count = 0;
			++m_index;

			if (m_index >= m_size)
			{
				m_index = 0;
			}
		}
	}

	void Anime::draw(const Vec2& pos) const
	{
		m_texture.uv(static_cast<double>(m_index) / m_size, 0.0, 1.0 / m_size, 1.0).drawAt(pos);
	}

	void Anime::draw(const Vec2& pos,double scale,double rotate) const
	{
		m_texture.uv(static_cast<double>(m_index) / m_size, 0.0, 1.0 / m_size, 1.0).scale(scale).rotate(rotate).drawAt(pos);
	}
