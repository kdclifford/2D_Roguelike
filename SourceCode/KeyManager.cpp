#include "KeyManager.h"

KeyManager::KeyManager(int up, int down, int left, int right, int exit)
{
	buttonPressed_[EButton::Up] = 0;
	buttonPressed_[EButton::Down] = 0;
	buttonPressed_[EButton::Left] = 0;
	buttonPressed_[EButton::Right] = 0;
	buttonPressed_[EButton::Exit] = 0;

	setButton(EButton::Up, up);
	setButton(EButton::Down, down);
	setButton(EButton::Left, left);
	setButton(EButton::Right, right);
	setButton(EButton::Exit, exit);
}

void KeyManager::setButton(EButton button, int code)
{
	auto key = buttonPressed_.find(button);

	if (key != buttonPressed_.end())
	{
		for (const auto& [k, v] : buttonMap_)
		{
			if (v == button)
			{
				buttonMap_.erase(k);
				break;
			}
		}
	}
	buttonMap_[code] = button;
}

bool KeyManager::exitGame()
{
	return buttonPressed_[EButton::Exit];
}

void KeyManager::resetButtonState()
{
	for (auto& [k, v] : buttonPressed_)
	{
		v &= KEY_SEEN;
	}
}

unsigned char KeyManager::buttonPress(int button, unsigned char state)
{
	auto key = buttonMap_.find(button);

	if (key != buttonMap_.end())
	{
		buttonPressed_.find(key->second)->second = state;
		return buttonPressed_.find(key->second)->second;
	}

	return 0;
}

unsigned char KeyManager::buttonPress(EButton button)
{
	auto key = buttonPressed_.find(button);

	if (key != buttonPressed_.end())
	{
		return key->second;
	}

	return 0;
}

unsigned char KeyManager::buttonRelease(int button, unsigned char state)
{
	auto key = buttonMap_.find(button);

	if (key != buttonMap_.end())
	{
		buttonPressed_.find(key->second)->second &= state;
		return buttonPressed_.find(key->second)->second;
	}

	return 0;
}