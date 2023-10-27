# include "LanguageSelectScene.hpp"

LanguageSelectScene::LanguageSelectScene(const InitData& init)
	: IScene{ init }
	, m_buttons(2)
{
	const Font& font = FontAsset(U"Text");

	m_buttons[0].text = font(U"にほんご");
	m_buttons[1].text = font(U"English");

	for (auto&& [i, button] : IndexedRef(m_buttons))
	{
		button.region = RectF{ 12, (7 + i * 2), 8, 1.5 };
	}

	if (getData().lang == LanguageCode::Japanese)
	{
		m_buttons.focus(0);
	}
	else if (getData().lang == LanguageCode::EnglishUS)
	{
		m_buttons.focus(1);
	}
}

void LanguageSelectScene::update()
{
	if (m_buttons.update())
	{
		const auto index = m_buttons.focusIndex();

		if (index == 0)
		{
			getData().lang = LanguageCode::Japanese;
		}
		else if (index == 1)
		{
			getData().lang = LanguageCode::EnglishUS;
		}

		getData().save();

		changeScene(AppState::Title, TransitionTimeDefault);
	}

	if (KeyNo.down())
	{
		changeScene(AppState::Title, TransitionTimeDefault);
	}
}

void LanguageSelectScene::draw() const
{
	m_buttons.draw();
}
