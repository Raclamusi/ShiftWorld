# pragma once

# include <Siv3D.hpp>

enum class Resolution : uint8
{
	Res360p,
	Res720p,
	Res1080p,
	Res1440p,

	HD = Res720p,
	FHD = Res1080p,
};

constexpr uint8 NumberOfResolution = 4;

[[nodiscard]]
constexpr Size ResolutionToSize(Resolution resolution) noexcept
{
	constexpr Size Sizes[NumberOfResolution]
	{
		Size{ 640, 360 }, Size{ 1280, 720 }, Size{ 1920, 1080 }, Size{ 2560, 1440 }
	};

	const auto index = FromEnum(resolution);

	if (index >= NumberOfResolution)
	{
		return {};
	}

	return Sizes[index];
}

[[nodiscard]]
constexpr StringView ResolutionToString(Resolution resolution) noexcept
{
	constexpr StringView Strings[NumberOfResolution]
	{
		U"640 × 360", U"1280 × 720", U"1920 × 1080", U"2560 × 1440"
	};

	const auto index = FromEnum(resolution);

	if (index >= NumberOfResolution)
	{
		return {};
	}

	return Strings[index];
}
