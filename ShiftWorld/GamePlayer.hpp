# pragma once

# include "App.hpp"

class GamePlayer
{
public:

	GamePlayer(int32 x, int32 y);

	const Point& pos() const noexcept;

	const Point& dir() const noexcept;

	Point viewpoint() const noexcept;

	void setPos(int32 x, int32 y) noexcept;
	void setPos(const Point& pos) noexcept;

	void walkForward();

	bool update(const std::array<StringView, SceneHeight>& field);

	void draw() const;

private:

	Point m_pos;
	Point m_dir{ 0, -1 };
	Timer m_inputTimer{ 0.2s };
};

inline GamePlayer::GamePlayer(int32 x, int32 y)
	: m_pos{ x, y } {}

inline const Point& GamePlayer::pos() const noexcept
{
	return m_pos;
}

inline const Point& GamePlayer::dir() const noexcept
{
	return m_dir;
}

inline Point GamePlayer::viewpoint() const noexcept
{
	return m_pos + m_dir;
}

inline void GamePlayer::setPos(int32 x, int32 y) noexcept
{
	m_pos.set(x, y);
}

inline void GamePlayer::setPos(const Point& pos) noexcept
{
	m_pos.set(pos);
}

inline void GamePlayer::walkForward()
{
	m_pos += m_dir;
	m_inputTimer.restart();
}

inline bool GamePlayer::update(const std::array<StringView, SceneHeight>& field)
{
	bool moved = false;

	if (not m_inputTimer.isRunning())
	{
		moved = true;

		if (KeyUp.pressed())
		{
			m_dir = Point{ 0, -1 };
		}
		else if (KeyDown.pressed())
		{
			m_dir = Point{ 0, 1 };
		}
		else if (KeyLeft.pressed())
		{
			m_dir = Point{ -1, 0 };
		}
		else if (KeyRight.pressed())
		{
			m_dir = Point{ 1, 0 };
		}
		else
		{
			moved = false;
		}

		if (moved)
		{
			const Point p = m_pos + m_dir;
			if ((field[p.y][p.x] == U'.') || IsUpper(field[p.y][p.x]))
			{
				m_inputTimer.restart();
				m_pos = p;
			}
			else
			{
				moved = false;
			}
		}
	}

	return moved;
}

inline void GamePlayer::draw() const
{
	const Texture& texture = TextureAsset(U"Player");

	Vec2 pos = m_pos;
	if (m_inputTimer.isRunning())
	{
		pos -= m_dir * m_inputTimer.progress1_0();
	}

	{
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

		texture.resized(1).rotated(m_dir.getAngle()).draw(pos);
	}
}
