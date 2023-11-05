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
		U"Prologue",
		{
			U"################################",
			U"##............................##",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............A...............#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"##............................##",
			U"################################",
		},
	},
	{
		U"Stage1",
		{
			U"################################",
			U"##............................##",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............ｱ...............#",
			U"#..............................B",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"##............................##",
			U"###############a################",
		},
	},
	{
		U"Stage2",
		{
			U"################################",
			U"################################",
			U"################################",
			U"################################",
			U"################################",
			U"###############E################",
			U"#####......................#####",
			U"###..........................###",
			U"#...................ｲ..........#",
			U"B..............................C",
			U"#..............................#",
			U"###..........................###",
			U"#####......................#####",
			U"###############d################",
			U"################################",
			U"################################",
			U"################################",
			U"################################",
		},
	},
	{
		U"Stage2-1",
		{
			U"################################",
			U"################################",
			U"################################",
			U"################################",
			U"################################",
			U"########F######D######D#########",
			U"#####......................#####",
			U"###..........................###",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"###..........................###",
			U"#####......................#####",
			U"###############e################",
			U"################################",
			U"################################",
			U"################################",
			U"################################",
		},
	},
	{
		U"Stage2-2",
		{
			U"################################",
			U"################################",
			U"################################",
			U"################################",
			U"################################",
			U"########G######D######D#########",
			U"#####......................#####",
			U"###..........................###",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"###..........................###",
			U"#####......................#####",
			U"###############f################",
			U"################################",
			U"################################",
			U"################################",
			U"################################",
		},
	},
	{
		U"Stage2-3",
		{
			U"################################",
			U"################################",
			U"################################",
			U"################################",
			U"################################",
			U"########D######D######H#########",
			U"#####......................#####",
			U"###..........................###",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"###..........................###",
			U"#####......................#####",
			U"###############g################",
			U"################################",
			U"################################",
			U"################################",
			U"################################",
		},
	},
	{
		U"Stage2-4",
		{
			U"################################",
			U"################################",
			U"################################",
			U"################################",
			U"################################",
			U"#######D###D###I###D###D########",
			U"#####......................#####",
			U"###..........................###",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"###..........................###",
			U"#####......................#####",
			U"###############h################",
			U"################################",
			U"################################",
			U"################################",
			U"################################",
		},
	},
	{
		U"Stage2-5",
		{
			U"################################",
			U"################################",
			U"################################",
			U"################################",
			U"################################",
			U"###############D################",
			U"#####......................#####",
			U"###..........................###",
			U"#..............................#",
			U"#..............◦...............#",
			U"#..............................#",
			U"###..........................###",
			U"#####......................#####",
			U"###############i################",
			U"################################",
			U"################################",
			U"################################",
			U"################################",
		},
	},
	{
		U"Stage3",
		{
			U"################################",
			U"###..........................###",
			U"##............................##",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............ｳ...............#",
			U"C..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"#..............................#",
			U"##............................##",
			U"###..........................###",
			U"################################",
		},
	},
};

static const HashTable<char32, std::pair<StringView, StringView>> WarpPoints
{
	{ U'A', { U"Prologue", U"Stage1" } },
	{ U'B', { U"Stage1", U"Stage2" } },
	{ U'C', { U"Stage2", U"Stage3" } },
	{ U'D', { U"Stage2", U"Stage2" } },
	{ U'E', { U"Stage2", U"Stage2-1" } },
	{ U'F', { U"Stage2-1", U"Stage2-2" } },
	{ U'G', { U"Stage2-2", U"Stage2-3" } },
	{ U'H', { U"Stage2-3", U"Stage2-4" } },
	{ U'I', { U"Stage2-4", U"Stage2-5" } },
};

static const HashTable<char32, std::pair<int32, Array<Speech>>> Conversations
{
	{
		U'始',
		{
			0,
			{
				{
					{ U"???", U"？？？" },
					{
						U"... Brave man......",
						U"……ゆうしゃよ…………",
					},
				},
				{
					{ U"???", U"？？？" },
					{
						U"... Brave man, do you understand my words?",
						U"……ゆうしゃよ、わたしの　ことばが　わかるか？",
					},
					{
						{ { U"Yes", U"はい" }, 0 },
						{ { U"No", U"わかんない" }, U'帰' },
					},
				},
				{
					{ U"???", U"？？？" },
					{
						U"Good!\nThen do me a favor!",
						U"ほんとうか！　ならば　おねがいだ。\nわたしの　たのみを　きいてくれ！",
					},
				},
				{
					{ U"???", U"？？？" },
					{
						U"This world was once peaceful.\nBut it has changed since the demons came!",
						U"このせかいは　かつて　へいわだった。\nだが、あくまが　きてから　かわってしまった！",
					},
				},
				{
					{ U"???", U"？？？" },
					{
						U"Since the demons came to nest in this world,\neveryone's speech has become strange.",
						U"あくまどもが　このせかいに　すくうように　なってから\nみなの　ことばが　おかしくなって　しまったのだ。",
					},
				},
				{
					{ U"???", U"？？？" },
					{
						U"Some people, like you, can understand what is being said,\nwhile others can't understand anything at all.",
						U"キミのように　ことばのつうじるものも　いれば、\nまったく　なにをいってもつうじないものも　いる。",
					},
				},
				{
					{ U"???", U"？？？" },
					{
						U"Please, brave man, save this strange world!",
						U"ゆうしゃよ、この　おかしくなった　せかいを　すくってくれ！",
					},
				},
				{
					{ U"???", U"？？？" },
					{
						U"Please, I beg you...!",
						U"どうか、おねがいだ……！",
					},
				},
			},
		},
	},
	{
		U'帰',
		{
			0,
			{
				{
					{ U"???", U"？？？" },
					{
						U"I see...",
						U"そうか……。",
					},
				},
			},
		},
	},
	{
		U'ｱ',
		{
			1,
			{
				{
					{ U"Girl", U"しょうじょ" },
					{
						U"Excuse me, do you understand my words?",
						U"おにいさん、わたしのことば、わかる？",
					},
					{
						{ { U"Yes", U"わかるよ" }, 0 },
						{ { U"No", U"わかんない" }, U'あ' },
					},
				},
				{
					{ U"Girl", U"しょうじょ" },
					{
						U"Amazing!\nI haven't met anyone who understands my words in a long time.",
						U"すごーい！\nわたしのことばが　わかるひとなんて　ひさしぶり！",
					},
				},
				{
					{ U"Girl", U"しょうじょ" },
					{
						U"Ah, but you may have just answered in a good-natured way.",
						U"あ、でも、テキトーにこたえてるだけかも　しれないよね。",
					},
				},
				{
					{ U"Girl", U"しょうじょ" },
					{
						U"So what is a giraffe",
						U"じゃあ、キリンさんは　なんですか？",
					},
					{
						{ { U"Food", U"たべもの" }, U'あ' },
						{ { U"Animal", U"どうぶつ" }, 0 },
						{ { U"Instrument", U"がっき" }, U'あ' },
					},
				},
				{
					{ U"Girl", U"しょうじょ" },
					{
						U"Correct!\nSo what color is a clear sky?",
						U"せいかい！\nじゃあ、はれた　おそらは　なにいろ？",
					},
					{
						{ { U"Blue", U"あおいろ" }, 0 },
						{ { U"Yellow", U"きいろ" }, U'あ' },
						{ { U"Pink", U"ピンク" }, U'あ' },
					},
				},
				{
					{ U"Girl", U"しょうじょ" },
					{
						U"Correct!\nSo what is one plus one?",
						U"せいかーい！\nじゃあ、いち　たす　いち　は？",
					},
					{
						{ { U"One", U"ぬ" }, U'あ' },
						{ { U"Svn", U"な" }, U'あ' },
						{ { U"Two", U"に" }, 0 },
					},
				},
				{
					{ U"Girl", U"しょうじょ" },
					{
						U"Amazing!  You really can understand my words!\nThanks for talking with me!",
						U"すごーい！　ほんとうに　わかるんだね！\nおはなし　してくれて　ありがとう！",
					},
				},
			},
		},
	},
	{
		U'あ',
		{
			1,
			{
				{
					{ U"Girl", U"しょうじょ" },
					{
						U"What a disappointment.",
						U"なーんだ、がっかり。",
					},
				},
			},
		},
	},
	{
		U'ｲ',
		{
			2,
			{
				{
					{ U"Uncle", U"おっちゃん" },
					{
						U"I saw something strange down the road.",
						U"このさきで　ふしぎなものを　みたんだ。",
					},
				},
				{
					{ U"Uncle", U"おっちゃん" },
					{
						U"I just took the road up here, to the right,\nthen right again, then left, and finally to the middle.",
						U"このうえのみちを　みぎにいって　もういちどみぎにいって\nつぎはひだりにいって　さいごにまんなかをすすんだ　ところだ。",
					},
				},
			},
		},
	},
	{
		U'◦',
		{
			0,
			{
				{
					{ U"Operation", U"そうさ" },
					{
						U"Do you break the crystal?",
						U"すいしょうを　こわしますか？",
					},
					{
						{ { U"Yes", U"はい" }, U'壊' },
						{ { U"No", U"いいえ" }, 0 },
					},
				},
			},
		},
	},
	{
		U'壊',
		{
			0,
			{
				{
					{ U"Operation", U"そうさ" },
					{
						U"The crystal disappeared with the light.",
						U"すいしょうは　ひかりと　ともに　きえた。",
					},
				},
			},
		},
	},
	{
		U'ｳ',
		{
			0,
			{
				{
					{ U"???", U"？？？" },
					{
						U"The game ahead is still a work in progress.\n(Press X to go back to the title screen.)",
						U"このさきは　まだ　さくせいちゅうです。\nごめんね。\n（Xキーで タイトルに もどれます。）",
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
	, m_field{ U"Prologue" }
	, m_nextField{}
	, m_fieldChangeTimer{ 0.2s }
	, m_blockedWarpPoints{ U'B' }
	, m_inConversation{}
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

	startConversation(U'始');
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

					AudioAsset(U"Select").playOneShot();
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

			AudioAsset(U"Walk").playOneShot();
		}

		if (KeyYes.down())
		{
			const char32 fieldChar = GetFieldChar(field, m_player.viewpoint());

			if (not IsASCII(fieldChar))
			{
				if ((fieldChar != U'◦') || (not m_unlockedShifts.contains(1)))
				{
					startConversation(fieldChar);
				}
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

	const auto& field = GameFields.at(m_field);

	for (const auto [y, x] : step({ SceneHeight, SceneWidth }))
	{
		if ((field[y][x] == U'#') || m_blockedWarpPoints.contains(field[y][x]))
		{
			Rect{ x, y, 1, 1 }.draw(ColorF{ 0.8 });
		}
		else if (field[y][x] == U'◦')
		{
			RectF{ x + 0.1, y + 0.1, 0.8, 0.8 }.draw(ColorF{ 0.5 });

			if (not m_unlockedShifts.contains(1))
			{
				Circle{ x + 0.5, y + 0.5, 0.3 }.draw(Palette::White);
			}
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

void GameScene::nextConversation()
{
	const Font& font = FontAsset(U"Text");

	const auto& [shift, speeches] = Conversations.at(m_conversation);
	++m_convIndex;
	if (speeches.size() <= m_convIndex)
	{
		m_inConversation = false;

		if (m_conversation == U'帰')
		{
			changeScene(AppState::Title, TransitionTimeDefault);
		}
		else if (m_conversation == U'始')
		{
			m_nextField = U"Stage1";
			m_fieldChangeTimer.restart();
		}
		else if (m_conversation == U'ｱ')
		{
			m_blockedWarpPoints.erase(U'B');
		}
		else if (m_conversation == U'壊')
		{
			m_unlockedShifts.insert(1);
		}

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
