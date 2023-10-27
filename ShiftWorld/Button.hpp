# pragma once

# include <Siv3D.hpp>

class Button
{
public:

	DrawableText text;

	RectF region;

	void focus();

	void blur();

	void draw() const;

private:

	Stopwatch m_stopwatch;
};

inline void Button::focus()
{
	m_stopwatch.start();
}

inline void Button::blur()
{
	m_stopwatch.reset();
}

inline void Button::draw() const
{
	if (region.mouseOver())
	{
		region.draw(ColorF{ Palette::White, 0.2 });
	}

	text.drawAt(0.8, region.center());

	if (m_stopwatch.isStarted())
	{
		region.drawFrame(0.1, ColorF{ Palette::White, 0.7 * Periodic::Jump0_1(1, m_stopwatch.sF() + 0.5) });
	}
}
