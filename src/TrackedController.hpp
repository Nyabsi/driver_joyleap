#pragma once

#include <InputManager.hpp>

#include <openvr_driver.h>

class TrackedController: public vr::ITrackedDeviceServerDriver
{
public:
	TrackedController(vr::ETrackedControllerRole role);
	vr::EVRInitError Activate(uint32_t unObjectId);
	void Deactivate();
	void EnterStandby();
	void* GetComponent(const char* pchComponentNameAndVersion);
	void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize);
	vr::DriverPose_t GetPose();
	void Update(bool connected);
	void UpdateInput(InputState state);
private:
	void UpdatePose();
	vr::ETrackedControllerRole m_role { vr::ETrackedControllerRole::TrackedControllerRole_Invalid };
	uint32_t m_objectId { 999 };
	vr::DriverPose_t m_pose { 0 };
	vr::VRInputComponentHandle_t m_bindings[10] { };
	vr::VRInputComponentHandle_t m_skeletonHandle { };
};