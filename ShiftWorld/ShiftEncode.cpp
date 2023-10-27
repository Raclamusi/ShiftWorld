# include "ShiftEncode.hpp"

namespace
{
	static constexpr int32 JapaneseSmallCharactersSize = 20;

	static constexpr StringView JapaneseCharacters =
		U"ぁぃぅぇぉゃぃゅぇょァィゥェォャィュェョ"
		U"あいうえおかきくけこさしすせそたちつてとなにぬねの"
		U"はひふへほまみむめもやいゆえよらりるれろわゐうゑを"
		U"がぎぐげござじずぜぞだぢづでどばびぶべぼぱぴぷぺぽ"
		U"アイウエオカキクケコサシスセソタチツテトナニヌネノ"
		U"ハヒフヘホマミムメモヤイユエヨラリルレロワヰウヱヲ"
		U"ガギグゲゴザジズゼゾダヂヅデドバビブベボパピプペポ";

	static String ShiftEncodeJapanese(StringView s, int32 shift)
	{
		String result(s.size(), U'\0');
		bool yoon = false;

		for (int64 i = int64(s.size() - 1); i >= 0 ; --i)
		{
			if (yoon)
			{
				result[i] = s[i];

				yoon = false;
			}
			else if (auto pos = JapaneseCharacters.indexOf(s[i]); pos != StringView::npos)
			{
				result[i] = JapaneseCharacters[pos / 5 * 5 + (pos + shift) % 5];

				if (pos < JapaneseSmallCharactersSize)
				{
					yoon = true;
				}
			}
			else
			{
				result[i] = s[i];
			}
		}

		return result;
	}

	static String ShiftEncodeEnglishUS(StringView s, int32 shift)
	{
		String result(s.size(), U'\0');

		for (size_t i = 0; i < s.size(); ++i)
		{
			if (IsUpper(s[i]))
			{
				result[i] = (s[i] - U'A' + shift) % 26 + U'A';
			}
			else if (IsLower(s[i]))
			{
				result[i] = (s[i] - U'a' + shift) % 26 + U'a';
			}
			else
			{
				result[i] = s[i];
			}
		}

		return result;
	}
}

String ShiftEncode(StringView s, int32 shift, LanguageCode lang)
{
	shift %= 5;
	if (shift < 0)
	{
		shift += 5;
	}

	if (lang == LanguageCode::Japanese)
	{
		return ShiftEncodeJapanese(s, shift);
	}

	if (lang == LanguageCode::EnglishUS)
	{
		return ShiftEncodeEnglishUS(s, shift);
	}

	return String{ s };
}
