#include "InputManager.hpp"

#include <JoyShockLibrary.h>

using namespace std::chrono_literals;

void InputManager::Init()
{
	std::function<void(int)> connect = [&](int deviceId) -> void { Connect(deviceId); };
	JslSetConnectCallback(connect);

	std::function<void(int, bool)> disconnect = [&](int deviceId, bool timeout) -> void { Disconnect(deviceId, timeout); };
	JslSetDisconnectCallback(disconnect);

	std::function<void(int, JOY_SHOCK_STATE, JOY_SHOCK_STATE, IMU_STATE, IMU_STATE, float)> state = [&](int deviceId, JOY_SHOCK_STATE state, JOY_SHOCK_STATE oldState, IMU_STATE imu, IMU_STATE oldImu, float deltaTime) -> void { UpdateState(deviceId, state, oldState); };
	JslSetCallback(state);

	m_thread = std::thread(&InputManager::Update, this);
	m_thread.detach();
}

InputState InputManager::GetState(vr::ETrackedControllerRole role)
{
	InputState state { };

	switch (role)
	{
		case vr::TrackedControllerRole_LeftHand:
		{
			JOY_SHOCK_STATE reportedState = m_controllerStates[JS_TYPE_JOYCON_LEFT];

			state.system = reportedState.buttons & JSMASK_CAPTURE;

			break;
		}
		case vr::TrackedControllerRole_RightHand:
		{
			JOY_SHOCK_STATE reportedState = m_controllerStates[JS_TYPE_JOYCON_RIGHT];

			state.system = reportedState.buttons & JSMASK_HOME;

			break;
		}
		default:
			break;
	}

	return state;
}

void InputManager::Connect(int deviceId)
{
	int deviceType = JslGetControllerType(deviceId);
	m_controllerMappings[deviceId] = deviceType;

	switch (deviceType)
	{
		case JS_TYPE_JOYCON_LEFT:
			m_Left = true;
			break;
		case JS_TYPE_JOYCON_RIGHT:
			m_Right = true;
			break;
		default:
			break; // Unsupported
	}

	if (!m_connected && (m_Left && m_Right))
	{
		m_connected.exchange(true);

		if (m_thread.joinable())
			m_thread.join();
	}
}

void InputManager::Disconnect(int deviceId, bool timeout)
{
	if (!timeout)
	{
		int deviceType = JslGetControllerType(deviceId);

		switch (deviceType)
		{
			case JS_TYPE_JOYCON_LEFT:
				m_Left = false;
				break;
			case JS_TYPE_JOYCON_RIGHT:
				m_Right = false;
				break;
			default:
				break; // Unsupported
		}

		if (m_connected && (!m_Left && !m_Right))
		{
			m_connected.exchange(false);

			m_thread = std::thread(&InputManager::Update, this);
			m_thread.detach();
		}
	}
}

void InputManager::UpdateState(int deviceId, JOY_SHOCK_STATE state, JOY_SHOCK_STATE oldState)
{
	if (state.buttons != oldState.buttons)
		m_controllerStates[m_controllerMappings[deviceId]].buttons = state.buttons;
}

void InputManager::Update()
{
	while (!m_connected)
	{
		JslConnectDevices();
		std::this_thread::sleep_for(1ms);
	}
}