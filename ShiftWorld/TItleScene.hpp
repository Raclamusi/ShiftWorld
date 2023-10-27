# pragma once

# include "App.hpp"
# include "ButtonList.hpp"

class TitleScene : public App::Scene
{
public:

	TitleScene(const InitData& init);

	void update() override;

	void draw() const override;

private:

	DrawableText m_title;

	ButtonList m_buttons;
};
