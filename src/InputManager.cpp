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

			state.valid = true;
			state.trigger = reportedState.lTrigger;
			state.system = reportedState.buttons & JSMASK_CAPTURE;
			state.grip = reportedState.buttons & JSMASK_SL;
			state.thumbClick = reportedState.buttons & JSMASK_LCLICK;
			state.thumbX = reportedState.stickLX;
			state.thumbY = reportedState.stickLY;
			state.bButton = reportedState.buttons & JSMASK_UP;
			state.aButton = reportedState.buttons & JSMASK_DOWN;

			break;
		}
		case vr::TrackedControllerRole_RightHand:
		{
			JOY_SHOCK_STATE reportedState = m_controllerStates[JS_TYPE_JOYCON_RIGHT];

			state.valid = true;
			state.trigger = reportedState.rTrigger;
			state.system = reportedState.buttons & JSMASK_CAPTURE;
			state.grip = reportedState.buttons & JSMASK_SR;
			state.thumbClick = reportedState.buttons & JSMASK_RCLICK;
			state.thumbX = reportedState.stickRX;
			state.thumbY = reportedState.stickRY;
			state.bButton = reportedState.buttons & JSMASK_N;
			state.aButton = reportedState.buttons & JSMASK_S;

			break;
		}
		default:
			break;
	}

	return state;
}

bool InputManager::IsConnected(vr::ETrackedControllerRole role) const
{
	return role == vr::TrackedControllerRole_LeftHand ? m_Left : m_Right;
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

	if (!m_connected.load() && (m_Left && m_Right))
	{
		m_connected.exchange(true);

		if (m_thread.joinable())
			m_thread.join();
	}
}

void InputManager::Disconnect(int deviceId, bool timeout)
{
	switch (m_controllerMappings[deviceId])
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

	if (m_connected.load() && (!m_Left || !m_Right))
	{
		m_connected.exchange(false);

		m_thread = std::thread(&InputManager::Update, this);
		m_thread.detach();
	}
}

void InputManager::UpdateState(int deviceId, JOY_SHOCK_STATE state, JOY_SHOCK_STATE oldState)
{
	m_controllerStates[m_controllerMappings[deviceId]] = state;
}

void InputManager::Update()
{
	while (!m_connected)
	{
		JslConnectDevices();
		std::this_thread::sleep_for(1ms);
	}
}