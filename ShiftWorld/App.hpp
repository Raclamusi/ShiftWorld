# pragma once

# include "AppState.hpp"
# include "GameData.hpp"

using App = SceneManager<AppState, GameData>;

inline constexpr Size SceneSize{ 32, 18 };
inline constexpr int32 SceneWidth = SceneSize.x;
inline constexpr int32 SceneHeight = SceneSize.y;

inline constexpr Duration TransitionTimeDefault = 300ms;

static const InputGroup KeyYes = (KeyZ | KeyEnter);
static const InputGroup KeyNo = (KeyX | KeyEscape);

[[nodiscard]]
inline Transformer2D GetTransformer()
{
	return Transformer2D{ Mat3x2::Scale(static_cast<double>(Scene::Width()) / SceneWidth), TransformCursor::Yes };
}
