#include <TrackedDeviceProvider.hpp>
#include <TrackedController.hpp>

#include <openvr_driver.h>

vr::EVRInitError TrackedDeviceProvider::Init(vr::IVRDriverContext* pDriverContext)
{
    VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);

    vr::VRServerDriverHost()->TrackedDeviceAdded("Joycon_Left", vr::TrackedDeviceClass_Controller, &m_Left);
    vr::VRServerDriverHost()->TrackedDeviceAdded("Joycon_Right", vr::TrackedDeviceClass_Controller, &m_Right);

    m_Input.Init();

    return vr::VRInitError_None;
}

void TrackedDeviceProvider::Cleanup()
{
    VR_CLEANUP_SERVER_DRIVER_CONTEXT();
}

void TrackedDeviceProvider::RunFrame()
{
    InputState stateLeft { };
    InputState stateRight { };

    stateLeft = m_Input.GetState(vr::TrackedControllerRole_LeftHand);
    stateRight = m_Input.GetState(vr::TrackedControllerRole_RightHand);

    m_Left.UpdateInput(stateLeft);
    m_Right.UpdateInput(stateRight);

    m_Left.Update(m_Input.IsConnected(vr::TrackedControllerRole_LeftHand));
    m_Right.Update(m_Input.IsConnected(vr::TrackedControllerRole_RightHand));
}