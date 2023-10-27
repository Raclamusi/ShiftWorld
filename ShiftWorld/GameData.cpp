# include "GameData.hpp"

static constexpr FilePathView SavedataPath = U"savedata.json";

void GameData::load()
{
	const auto data = JSON::Load(SavedataPath);

	if (const auto value = data[U"lang"].getOpt<int64>())
	{
		lang = ToEnum<LanguageCode>(int32(*value));
	}

	if (const auto value = data[U"resolution"].getOpt<int64>())
	{
		resolution = ToEnum<Resolution>(uint8(*value));
	}

	if (const auto value = data[U"soundVolume"].getOpt<int64>())
	{
		soundVolume = int8(Clamp<int64>(*value, 0, 10));
	}
}

void GameData::save() const
{
	JSON data;

	data[U"lang"] = FromEnum(lang);
	data[U"resolution"] = FromEnum(resolution);
	data[U"soundVolume"] = soundVolume;

	data.save(SavedataPath);
}
