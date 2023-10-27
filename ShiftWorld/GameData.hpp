# pragma once

# include <Siv3D.hpp>
# include "Resolution.hpp"

struct GameData
{
	LanguageCode lang = LanguageCode::Japanese;

	Resolution resolution = Resolution::Res720p;

	int8 soundVolume = 10;

	void load();

	void save() const;
};
