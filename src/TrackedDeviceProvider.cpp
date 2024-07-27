#include <TrackedDeviceProvider.hpp>
#include <TrackedController.hpp>

#include <openvr_driver.h>

vr::EVRInitError TrackedDeviceProvider::Init(vr::IVRDriverContext* pDriverContext)
{
    VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);

    vr::VRServerDriverHost()->TrackedDeviceAdded("Leap_Left_Hand", vr::TrackedDeviceClass_Controller, &m_Left);
    vr::VRServerDriverHost()->TrackedDeviceAdded("Leap_Right_Hand", vr::TrackedDeviceClass_Controller, &m_Right);

    return vr::VRInitError_None;
}

void TrackedDeviceProvider::Cleanup()
{
    VR_CLEANUP_SERVER_DRIVER_CONTEXT();
}

void TrackedDeviceProvider::RunFrame()
{
    m_Left.Update();
    m_Right.Update();
}