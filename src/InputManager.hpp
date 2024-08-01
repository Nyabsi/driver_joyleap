#pragma once

#include <functional>
#include <thread>
#include <atomic>
#include <map>

#include <openvr_driver.h>
#include <JoyShockLibrary.h>

struct InputState 
{
	bool valid { false };
	bool system { false };
	bool trigger { false };
	bool grip { false };
	bool thumbClick { false };
	float thumbX { 0.0f };
	float thumbY { 0.0f };
	bool aButton { false };
	bool bButton { false };
};

class InputManager {
public:
	void Init();
	void Destroy();
	InputState GetState(vr::ETrackedControllerRole role);
	bool IsConnected(vr::ETrackedControllerRole role) const;
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
	uint64_t m_lastUpdate { };
};