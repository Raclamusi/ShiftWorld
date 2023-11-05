# pragma once

# include "App.hpp"
# include "GamePlayer.hpp"
# include "ButtonList.hpp"

class GameScene : public App::Scene
{
public:

	GameScene(const InitData& init);

	void update() override;

	void draw() const override;

private:

	GamePlayer m_player;

	size_t m_languageIndex;

	StringView m_field;
	StringView m_nextField;
	Timer m_fieldChangeTimer;
	HashSet<char32> m_blockedWarpPoints;

	bool m_inConversation;
	char32 m_conversation;
	size_t m_convIndex;
	int32 m_convShift;
	Stopwatch m_convStopwatch;
	DrawableText m_convSpeaker;
	String m_convText;
	Optional<ButtonList> m_convButtons;
	HashSet<int32> m_unlockedShifts;
	Duration m_convSpeed;

	bool m_inPause;
	DrawableText m_pauseText;
	ButtonList m_pauseButtons;

	void startConversation(char32 conv);
	void nextConversation();
};
