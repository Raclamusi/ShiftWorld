# include "GameScene.hpp"

# include "ShiftEncode.hpp"

static constexpr LanguageCode LanguageIndices[]
{
	LanguageCode::EnglishUS,
	LanguageCode::Japanese,
};
static constexpr size_t NumberOfLanguage = std::ranges::size(LanguageIndices);

struct SpeechOption
{
	std::array<StringView, NumberOfLanguage> text;
	char32 nextConv;
};

struct Speech
{
	std::array<StringView, NumberOfLanguage> speaker;
	std::array<StringView, NumberOfLanguage> text;
	Array<SpeechOption> options;
};

static const HashTable<StringView, std::array<StringView, SceneHeight>> GameFields
{
	{
		U"Test1",
		{
			U"###########b####################",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................A",
			U"#..............................#",
			U"#..............ｱ...............#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"########################C#######",
		},
	},
	{
		U"Test2",
		{
			U"########################C#######",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"A..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#................ｲ.............#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"###########B####################",
		},
	},
	{
		U"Template",
		{
			U"################################",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"################################",
		},
	},
};

static const HashTable<char32, std::pair<StringView, StringView>> WarpPoints
{
	{ U'A', { U"Test1", U"Test2" } },
	{ U'B', { U"Test1", U"Test2" } },
	{ U'C', { U"Test1", U"Test2" } },
};

static const HashTable<char32, std::pair<int32, Array<Speech>>> Conversations
{
	{
		U'ｱ',
		{
			0,
			{
				{
					{ U"test1", U"てすと1" },
					{
						U"test test",
						U"てすと　てすと",
					},
				},
				{
					{ U"test1", U"てすと1" },
					{
						U"Do you love me?",
						U"すき？",
					},
					{
						{ { U"Yes", U"すき" }, U'ア' },
						{ { U"No", U"きらい" }, 0 },
					},
				},
				{
					{ U"test1", U"てすと1" },
					{
						U"OK...",
						U"そう……",
					},
				},
			},
		},
	},
	{
		U'ア',
		{
			0,
			{
				{
					{ U"test1", U"てすと1" },
					{
						U"I'm grad.",
						U"うれしい。",
					},
				},
			},
		},
	},
	{
		U'ｲ',
		{
			1,
			{
				{
					{ U"test2", U"てすと2" },
					{
						U"test test",
						U"てすと　てすと",
					},
				},
				{
					{ U"test2", U"てすと2" },
					{
						U"test test test",
						U"てすと　てすと　てすと",
					},
				},
			},
		},
	},
};

static char32 GetFieldChar(const std::array<StringView, SceneHeight>& field, const Point& pos)
{
	return field[pos.y][pos.x];
}

GameScene::GameScene(const InitData& init)
	: IScene{ init }
	, m_player{ 15, 8 }
	, m_field{ U"Test1" }
	, m_nextField{}
	, m_fieldChangeTimer{ 0.2s }
	, m_blockedWarpPoints{ U'C' }
	, m_inConversation{ false }
	, m_conversation{}
	, m_convIndex{}
	, m_convShift{}
	, m_convStopwatch{}
	, m_convSpeaker{}
	, m_convText{}
	, m_convButtons{}
	, m_unlockedShifts{}
	, m_inPause{ false }
	, m_pauseButtons{ 2 }
{
	const Font& font = FontAsset(U"Text");

	if (getData().lang == LanguageCode::Japanese)
	{
		m_languageIndex = 1;
		m_convSpeed = 80ms;
		m_pauseText = font(U"タイトルに　もどりますか？\n（ゲームの　じょうたいは　ほぞんされません。）");
		m_pauseButtons[0].text = font(U"タイトルに　もどる");
		m_pauseButtons[1].text = font(U"とじる");
	}
	else
	{
		m_languageIndex = 0;
		m_convSpeed = 40ms;
		m_pauseText = font(U"Do you go back to the title screen?\n(Game state is not saved.)");
		m_pauseButtons[0].text = font(U"Back to Title");
		m_pauseButtons[1].text = font(U"Close");
	}

	m_pauseButtons[0].region = RectF{ 12, 9, 8, 1.5 };
	m_pauseButtons[1].region = RectF{ 12, 11, 8, 1.5 };
}

void GameScene::update()
{
	if (m_nextField)
	{
		if (m_fieldChangeTimer.reachedZero())
		{
			const auto& field = GameFields.at(m_field);
			const auto& nextField = GameFields.at(m_nextField);
			const char32 warpChar = GetFieldChar(field, m_player.pos());
			for (const auto [y, x] : step({ SceneHeight, SceneWidth }))
			{
				if (ToUpper(nextField[y][x]) == warpChar)
				{
					m_player.setPos(x, y);
					m_player.walkForward();
				}
			}

			m_field = m_nextField;
			m_nextField = U"";
			m_fieldChangeTimer.restart();
		}
	}
	else if (m_inPause)
	{
		if (m_pauseButtons.update())
		{
			const size_t index = m_pauseButtons.focusIndex();

			if (index == 0)
			{
				changeScene(AppState::Title, TransitionTimeDefault);
			}
			else if (index == 1)
			{
				m_inPause = false;
			}
		}

		if (KeyNo.down())
		{
			m_inPause = false;
		}
	}
	else if (m_inConversation)
	{
		if (m_convStopwatch.isPaused() || (m_convText.size() <= (m_convStopwatch.elapsed() / m_convSpeed)))
		{
			if (m_convButtons)
			{
				if (m_convButtons->update())
				{
					const size_t index = m_convButtons->focusIndex();
					const auto& options = Conversations.at(m_conversation).second[m_convIndex].options;
					const char32 nextConv = options[index].nextConv;

					if (nextConv == U'\0')
					{
						nextConversation();
					}
					else
					{
						startConversation(nextConv);
					}
				}
			}
			else
			{
				if ((KeyYes | MouseL).down())
				{
					nextConversation();
				}
			}
		}
		else
		{
			if ((KeyYes | MouseL).down())
			{
				m_convStopwatch.pause();
			}
		}
	}
	else
	{
		const auto& field = GameFields.at(m_field);

		if (m_player.update(field))
		{
			const char32 fieldChar = GetFieldChar(field, m_player.pos());

			if (IsUpper(fieldChar) && (not m_blockedWarpPoints.contains(fieldChar)))
			{
				auto&& [f1, f2] = WarpPoints.at(fieldChar);
				m_nextField = ((f1 == m_field) ? f2 : f1);
				m_fieldChangeTimer.restart();
			}
		}

		if (KeyYes.down())
		{
			const char32 fieldChar = GetFieldChar(field, m_player.viewpoint());

			if (not IsASCII(fieldChar))
			{
				startConversation(fieldChar);
			}
		}

		if (KeyNo.down())
		{
			m_inPause = true;
			m_pauseButtons.focus(0);
		}
	}
}

void GameScene::draw() const
{
	const Font& font = FontAsset(U"Text");
	const Texture& npcTexture = TextureAsset(U"NPC");

//……ゆうしゃよ、わたしの　ことばが　わかるか？
//
//……よかった。やっと　ことばの　つうじる　ゆうしゃに　であえた。
//
//このせかいに　あくまが　すくうようになってから
//みなのことばが　おかしくなった。
//
//きみのように　ことばの　つうじるものも　いれば
//まったく　つうじないものも　いる。
//
//
//
//ゆうしゃよ、せかいを　すくってくれ！
//
// よかった。やっと　ことばの　つうじるひとを　みつけた。

	const auto& field = GameFields.at(m_field);

	for (const auto [y, x] : step({ SceneHeight, SceneWidth }))
	{
		if ((field[y][x] == U'#') || m_blockedWarpPoints.contains(field[y][x]))
		{
			Rect{ x, y, 1, 1 }.draw(ColorF{ 0.8 });
		}
		else if (not IsASCII(field[y][x]))
		{
			const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

			npcTexture.resized(1).draw(Arg::topLeft(x, y));
		}
	}

	m_player.draw();

	if (m_inConversation)
	{
		const bool ready = (m_convStopwatch.isPaused() || (m_convText.size() <= (m_convStopwatch.elapsed() / m_convSpeed)));

		RectF{ 0, 12, 32, 6 }.draw(Palette::Black);
		RectF{ 1, 10, 10, 3 }.draw(Palette::Black);
		if (ready && m_convButtons)
		{
			RectF{ 23, (11 - m_convButtons->size() * 1.2), 8, (2 + m_convButtons->size() * 1.2) }.draw(Palette::Black);
		}
		RectF{ 0, 12, 32, 6 }.stretched(-0.5).drawFrame(0.1, Palette::White);
		RectF{ 1, 10, 10, 3 }.stretched(-0.5).drawFrame(0.1, Palette::White);
		if (ready && m_convButtons)
		{
			RectF{ 23, (11 - m_convButtons->size() * 1.2), 8, (2 + m_convButtons->size() * 1.2) }.stretched(-0.5).drawFrame(0.1, Palette::White);
		}

		m_convSpeaker.drawAt(0.8, 6, 11.5, Palette::White);

		if (ready)
		{
			font(m_convText).draw(0.8, 1, 13, Palette::White);
			RectF{ 30.8, 16.8, 0.2, 0.2 }.draw(Palette::White);
			if (m_convButtons)
			{
				m_convButtons->draw();
			}
		}
		else
		{
			font(m_convText.substr(0, static_cast<size_t>(m_convStopwatch.elapsed() / m_convSpeed))).draw(0.8, 1, 13, Palette::White);
		}
	}

	if (m_nextField)
	{
		Rect{ 0, 0, SceneSize }.draw(ColorF{ Palette::Black, m_fieldChangeTimer.progress0_1() });
	}
	else if (m_fieldChangeTimer.isRunning())
	{
		Rect{ 0, 0, SceneSize }.draw(ColorF{ Palette::Black, m_fieldChangeTimer.progress1_0() });
	}

	if (m_inPause)
	{
		RectF{ 3, 4, 26, 10 }
			.draw(Palette::Black)
			.stretched(-0.5)
			.drawFrame(0.1, Palette::White);
		m_pauseText.drawAt(0.8, 16, 7, Palette::White);
		m_pauseButtons.draw();
	}
}

void GameScene::startConversation(char32 conv)
{
	const Font& font = FontAsset(U"Text");

	const auto& [shift, speeches] = Conversations.at(conv);
	m_inConversation = true;
	m_conversation = conv;
	m_convIndex = 0;
	m_convShift = (m_unlockedShifts.contains(shift) ? 0 : shift);
	m_convStopwatch.restart();
	m_convSpeaker = font(ShiftEncode(speeches[m_convIndex].speaker[m_languageIndex], m_convShift, getData().lang));
	m_convText = ShiftEncode(speeches[m_convIndex].text[m_languageIndex], m_convShift, getData().lang);

	const auto& options = speeches[m_convIndex].options;
	if (options)
	{
		m_convButtons = ButtonList(options.size());
		for (auto&& [i, option] : Indexed(options))
		{
			auto& button = (*m_convButtons)[i];
			button.text = font(option.text[m_languageIndex]);
			button.region = RectF{ 24, (12.5 - (options.size() - i) * 1.5), 6, 1.2 };
		}
	}
	else
	{
		m_convButtons = none;
	}
}

void GameScene::nextConversation()
{
	const Font& font = FontAsset(U"Text");

	const auto& [shift, speeches] = Conversations.at(m_conversation);
	++m_convIndex;
	if (speeches.size() <= m_convIndex)
	{
		m_inConversation = false;
		return;
	}

	m_convStopwatch.restart();
	m_convSpeaker = font(speeches[m_convIndex].speaker[m_languageIndex]);
	m_convText = ShiftEncode(speeches[m_convIndex].text[m_languageIndex], m_convShift, getData().lang);

	const auto& options = speeches[m_convIndex].options;
	if (options)
	{
		m_convButtons = ButtonList(options.size());
		for (auto&& [i, option] : Indexed(options))
		{
			auto& button = (*m_convButtons)[i];
			button.text = font(ShiftEncode(option.text[m_languageIndex], m_convShift, getData().lang));
			button.region = RectF{ 24, (12.5 - (options.size() - i) * 1.5), 6, 1.2 };
		}
	}
	else
	{
		m_convButtons = none;
	}
}
