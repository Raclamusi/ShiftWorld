# pragma once

# include "App.hpp"
# include "ButtonList.hpp"

class LanguageSelectScene : public App::Scene
{
public:

	LanguageSelectScene(const InitData& init);

	void update() override;

	void draw() const override;

private:

	ButtonList m_buttons;
};
