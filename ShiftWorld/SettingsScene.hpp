# pragma once

# include "App.hpp"
# include "ButtonList.hpp"

class SettingsScene : public App::Scene
{
public:

	SettingsScene(const InitData& init);

	void update() override;

	void draw() const override;

private:

	DrawableText m_title;

	Array<DrawableText> m_texts;

	ButtonList m_buttons;

	ButtonList m_leftRightButtons;
};
