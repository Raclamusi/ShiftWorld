# include "SettingsScene.hpp"

SettingsScene::SettingsScene(const InitData& init)
	: IScene{ init }
	, m_texts(2)
	, m_buttons(4)
	, m_leftRightButtons(4)
{
	const Font& titleFont = FontAsset(U"Title");
	const Font& font = FontAsset(U"Text");

	m_buttons[0].text = font(ResolutionToString(getData().resolution));
	m_buttons[1].text = font(U"{:3d} %"_fmt(getData().soundVolume * 10));

	if (getData().lang == LanguageCode::Japanese)
	{
		m_title = titleFont(U"せってい");
		m_texts[0] = font(U"かいぞうど");
		m_texts[1] = font(U"こうかおん");
		m_buttons[2].text = font(U"ライセンスを表示");
		m_buttons[3].text = font(U"もどる");
	}
	else
	{
		m_title = titleFont(U"Settings");
		m_texts[0] = font(U"Resolution");
		m_texts[1] = font(U"Sound Volume");
		m_buttons[2].text = font(U"Show Licenses");
		m_buttons[3].text = font(U"Back");
	}

	for (auto&& [i, button] : IndexedRef(m_leftRightButtons))
	{
		button.text = font((i % 2 == 0) ? U"<" : U">");
	}

	m_leftRightButtons[0].region = RectF{ 16, 6, 1.5, 1.5 };
	m_buttons[0].region = RectF{ 18, 6, 6, 1.5 };
	m_leftRightButtons[1].region = RectF{ 24.5, 6, 1.5, 1.5 };

	m_leftRightButtons[2].region = RectF{ 16, 8, 1.5, 1.5 };
	m_buttons[1].region = RectF{ 18, 8, 6, 1.5 };
	m_leftRightButtons[3].region = RectF{ 24.5, 8, 1.5, 1.5 };

	m_buttons[2].region = RectF{ 12, 10, 8, 1.5 };
	m_buttons[3].region = RectF{ 12, 13, 8, 1.5 };

	m_leftRightButtons.focusedButton().blur();
}

void SettingsScene::update()
{
	const Font& font = FontAsset(U"Text");

	int32 diff = 0;

	if (KeyLeft.down())
	{
		diff = -1;
	}
	else if (KeyRight.down())
	{
		diff = 1;
	}

	for (auto&& [i, button] : IndexedRef(m_leftRightButtons))
	{
		if (button.region.leftClicked())
		{
			m_buttons.focus(i / 2);
			diff = ((i % 2 == 0) ? -1 : 1);
		}
	}

	if (diff != 0)
	{
		const auto index = m_buttons.focusIndex();

		if (index == 0)
		{
			getData().resolution = ToEnum<Resolution>((FromEnum(getData().resolution) + NumberOfResolution + diff) % NumberOfResolution);

			m_buttons[0].text = font(ResolutionToString(getData().resolution));
		}
		else if (index == 1)
		{
			getData().soundVolume = int8(Clamp((getData().soundVolume + diff), 0, 10));

			GlobalAudio::BusSetVolume(MixBus0, getData().soundVolume * 0.1);

			m_buttons[1].text = font(U"{:3d} %"_fmt(getData().soundVolume * 10));
		}

		AudioAsset(U"Select").playOneShot();
	}

	if (m_buttons.update())
	{
		const auto index = m_buttons.focusIndex();

		if (index == 0)
		{
			Window::Resize(ResolutionToSize(getData().resolution));
		}
		else if (index == 2)
		{
			LicenseManager::ShowInBrowser();
		}
		else if (index == 3)
		{
			Window::Resize(ResolutionToSize(getData().resolution));

			getData().save();

			changeScene(AppState::Title, TransitionTimeDefault);
		}
	}

	if (KeyNo.down())
	{
		Window::Resize(ResolutionToSize(getData().resolution));

		getData().save();

		changeScene(AppState::Title, TransitionTimeDefault);

		AudioAsset(U"Cancel").playOneShot();
	}
}

void SettingsScene::draw() const
{
	m_title.drawAt(1.5, 16, 3, Palette::White);

	m_texts[0].drawAt(0.8, 11, 6.75, Palette::White);
	m_texts[1].drawAt(0.8, 11, 8.75, Palette::White);

	m_buttons.draw();

	m_leftRightButtons.draw();
}
