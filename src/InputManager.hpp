#pragma once

#include <functional>
#include <thread>
#include <atomic>
#include <map>

#include <openvr_driver.h>
#include <JoyShockLibrary.h>

struct InputState 
{
	bool system { false };
};

class InputManager {
public:
	void Init();
	InputState GetState(vr::ETrackedControllerRole role);
	bool IsConnected() const { return m_Left && m_Right; }
private:
	void Connect(int deviceId);
	void Disconnect(int deviceId, bool timeout);
	void UpdateState(int deviceId, JOY_SHOCK_STATE state, JOY_SHOCK_STATE oldState);
	void Update();
	std::thread m_thread { };
	std::atomic<bool> m_connected { false };
	bool m_Left { false };
	bool m_Right { false };
	std::map<int, int> m_controllerMappings;
	std::map<int, JOY_SHOCK_STATE> m_controllerStates;
};