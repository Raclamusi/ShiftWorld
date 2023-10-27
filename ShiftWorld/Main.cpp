# include <Siv3D.hpp> // Siv3D v0.6.12
# include "App.hpp"
# include "TitleScene.hpp"
# include "LanguageSelectScene.hpp"
# include "SettingsScene.hpp"
# include "ShiftEncode.hpp"

void Main()
{
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);

	Window::SetTitle(U"Shift World");

	Scene::SetBackground(Palette::Black);

	FontAsset::Register(U"Title", FontMethod::MSDF, 48, U"example/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bitmap);
	FontAsset::Register(U"Text", FontMethod::MSDF, 48, U"example/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bitmap);

	const auto gameData = std::make_shared<GameData>();
	gameData->load();

	Window::Resize(ResolutionToSize(gameData->resolution));

	App manager{ gameData };
	manager
		.add<TitleScene>(AppState::Title)
		.add<LanguageSelectScene>(AppState::LanguageSelect)
		.add<SettingsScene>(AppState::Settings)
		.init(AppState::Title, 1000);

	while (System::Update())
	{
		const Transformer2D transformer = GetTransformer();

		if (not manager.update())
		{
			break;
		}
	}
}
