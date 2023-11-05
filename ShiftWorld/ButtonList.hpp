# pragma once

# include <Siv3D.hpp>
# include "Button.hpp"

class ButtonList
{
public:

	[[nodiscard]]
	ButtonList(size_t n);

	[[nodiscard]]
	const Button& operator [](size_t index) const;

	[[nodiscard]]
	Button& operator [](size_t index);

	[[nodiscard]]
	size_t size() const noexcept;

	[[nodiscard]]
	const Array<Button>& buttons() const noexcept;

	[[nodiscard]]
	size_t focusIndex() const noexcept;

	[[nodiscard]]
	const Button& focusedButton() const;

	[[nodiscard]]
	Button& focusedButton();

	[[nodiscard]]
	bool isClicked() const;

	void focus(size_t index);

	bool update();

	void draw() const;

	using iterator = Array<Button>::iterator;
	using const_iterator = Array<Button>::const_iterator;
	using reverse_iterator = Array<Button>::reverse_iterator;
	using const_reverse_iterator = Array<Button>::const_reverse_iterator;
	[[nodiscard]] iterator begin() noexcept { return m_buttons.begin(); }
	[[nodiscard]] iterator end() noexcept { return m_buttons.end(); }
	[[nodiscard]] const_iterator begin() const noexcept { return m_buttons.begin(); }
	[[nodiscard]] const_iterator end() const noexcept { return m_buttons.end(); }
	[[nodiscard]] const_iterator cbegin() const noexcept { return m_buttons.cbegin(); }
	[[nodiscard]] const_iterator cend() const noexcept { return m_buttons.cend(); }
	[[nodiscard]] reverse_iterator rbegin() noexcept { return m_buttons.rbegin(); }
	[[nodiscard]] reverse_iterator rend() noexcept { return m_buttons.rend(); }
	[[nodiscard]] const_reverse_iterator rbegin() const noexcept { return m_buttons.rbegin(); }
	[[nodiscard]] const_reverse_iterator rend() const noexcept { return m_buttons.rend(); }
	[[nodiscard]] const_reverse_iterator crbegin() const noexcept { return m_buttons.crbegin(); }
	[[nodiscard]] const_reverse_iterator crend() const noexcept { return m_buttons.crend(); }

private:

	Array<Button> m_buttons;

	size_t m_focusIndex;
};

inline ButtonList::ButtonList(size_t n)
	: m_buttons(n)
	, m_focusIndex{ 0 }
{
	m_buttons[0].focus();
}

inline const Button& ButtonList::operator [](size_t index) const
{
	return m_buttons[index];
}

inline Button& ButtonList::operator [](size_t index)
{
	return m_buttons[index];
}

inline size_t ButtonList::size() const noexcept
{
	return m_buttons.size();
}

inline const Array<Button>& ButtonList::buttons() const noexcept
{
	return m_buttons;
}

inline size_t ButtonList::focusIndex() const noexcept
{
	return m_focusIndex;
}

inline const Button& ButtonList::focusedButton() const
{
	return m_buttons[m_focusIndex];
}

inline Button& ButtonList::focusedButton()
{
	return m_buttons[m_focusIndex];
}

inline bool ButtonList::isClicked() const
{
	return (KeyYes.down() || focusedButton().region.leftClicked());
}

inline void ButtonList::focus(size_t index)
{
	index %= size();

	if (index != m_focusIndex)
	{
		focusedButton().blur();
		m_focusIndex = index;
		focusedButton().focus();

		AudioAsset(U"Select").playOneShot();
	}
}

inline bool ButtonList::update()
{
	size_t newIndex = m_focusIndex;

	if (not Cursor::DeltaF().isZero())
	{
		const auto it = std::ranges::find_if(m_buttons, [](auto&& button) { return button.region.mouseOver(); });

		if (it != m_buttons.end())
		{
			newIndex = (it - m_buttons.begin());
		}
	}

	if (KeyDown.down())
	{
		newIndex = ((m_focusIndex + 1) % m_buttons.size());
	}
	else if (KeyUp.down())
	{
		newIndex = ((m_focusIndex + m_buttons.size() - 1) % m_buttons.size());
	}

	focus(newIndex);

	const bool clicked = isClicked();

	if (clicked)
	{
		AudioAsset(U"OK").playOneShot();
	}

	return clicked;
}

inline void ButtonList::draw() const
{
	for (auto&& button : m_buttons)
	{
		button.draw();
	}
}
