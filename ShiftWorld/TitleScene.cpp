# include "TitleScene.hpp"

TitleScene::TitleScene(const InitData& init)
	: IScene{ init }
	, m_title{}
	, m_buttons(4)
{
	const Font& titleFont = FontAsset(U"Title");
	const Font& font = FontAsset(U"Text");

	m_buttons[2].text = font(U"Language");

	if (getData().lang == LanguageCode::Japanese)
	{
		m_title = titleFont(U"シフトワールド");
		m_buttons[0].text = font(U"あそぶ");
		m_buttons[1].text = font(U"せってい");
		m_buttons[3].text = font(U"おわる");
	}
	else
	{
		m_title = titleFont(U"Shift World");
		m_buttons[0].text = font(U"Play");
		m_buttons[1].text = font(U"Settings");
		m_buttons[3].text = font(U"Quit");
	}

	for (auto&& [i, button] : IndexedRef(m_buttons))
	{
		button.region = RectF{ 12, (8 + i * 2), 8, 1.5 };
	}
}

void TitleScene::update()
{
	if (m_buttons.update())
	{
		const auto index = m_buttons.focusIndex();

		if (index == 0)
		{

		}
		else if (index == 1)
		{
			changeScene(AppState::Settings, TransitionTimeDefault);
		}
		else if (index == 2)
		{
			changeScene(AppState::LanguageSelect, TransitionTimeDefault);
		}
		else if (index == 3)
		{
			System::Exit();
		}
	}
}

void TitleScene::draw() const
{
	m_title.drawAt(2, 16, 4, Palette::White);

	m_buttons.draw();
}
