#include <TrackedController.hpp>
#include <cstring>

#include <Windows.h>

TrackedController::TrackedController(vr::ETrackedControllerRole role)
{
    m_role = role;
    m_pose = { 0 };
}

vr::EVRInitError TrackedController::Activate(uint32_t unObjectId)
{
	vr::EVRInitError result = vr::VRInitError_Driver_Failed;

	if (m_objectId == 999)
	{
		m_objectId = unObjectId;

        auto props = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_objectId);

        vr::VRProperties()->SetStringProperty(props, vr::Prop_ManufacturerName_String, "Nintendo");
        vr::VRProperties()->SetStringProperty(props, vr::Prop_TrackingSystemName_String, "Leapify_Extension");
        vr::VRProperties()->SetStringProperty(props, vr::Prop_SerialNumber_String, m_role == vr::TrackedControllerRole_LeftHand ? "Joycon_Left" : "Joycon_Right");

        vr::VRProperties()->SetInt32Property(props, vr::Prop_DeviceClass_Int32, vr::TrackedDeviceClass_Controller);
        vr::VRProperties()->SetInt32Property(props, vr::Prop_ControllerHandSelectionPriority_Int32, 999);
        vr::VRProperties()->SetInt32Property(props, vr::Prop_ControllerRoleHint_Int32, m_role);

        vr::VRProperties()->SetBoolProperty(props, vr::Prop_WillDriftInYaw_Bool, false);
        vr::VRProperties()->SetBoolProperty(props, vr::Prop_DeviceIsWireless_Bool, true);
        vr::VRProperties()->SetBoolProperty(props, vr::Prop_DeviceCanPowerOff_Bool, true);
        vr::VRProperties()->SetBoolProperty(props, vr::Prop_Identifiable_Bool, false);

        vr::VRProperties()->SetBoolProperty(props, vr::Prop_DeviceIsCharging_Bool, false);
        vr::VRProperties()->SetBoolProperty(props, vr::Prop_DeviceProvidesBatteryStatus_Bool, false);

        vr::VRProperties()->SetInt32Property(props, vr::Prop_Axis0Type_Int32, vr::k_eControllerAxis_TrackPad);
        vr::VRProperties()->SetInt32Property(props, vr::Prop_Axis1Type_Int32, vr::k_eControllerAxis_Trigger);
        vr::VRProperties()->SetInt32Property(props, vr::Prop_Axis2Type_Int32, vr::k_eControllerAxis_Trigger);

        vr::VRProperties()->SetStringProperty(props, vr::Prop_ModelNumber_String, "Joycon_Controller");

        vr::VRProperties()->SetStringProperty(props, vr::Prop_ResourceRoot_String, "joyleap");
        vr::VRProperties()->SetStringProperty(props, vr::Prop_InputProfilePath_String, "{joyleap}/input/index_controller_profile.json");
        vr::VRProperties()->SetStringProperty(props, vr::Prop_RenderModelName_String, m_role == vr::TrackedControllerRole_LeftHand ? "{joyleap}valve_controller_knu_1_0_left" : "{leapify}valve_controller_knu_1_0_right");
        vr::VRProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceOff_String, m_role == vr::TrackedControllerRole_LeftHand ? "{joyleap}/icons/left_controller_status_off.png" : "{leapify}/icons/right_controller_status_off.png");
        vr::VRProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceSearching_String, m_role == vr::TrackedControllerRole_LeftHand ? "{joyleap}/icons/left_controller_status_searching.gif" : "{leapify}/icons/right_controller_status_searching.gif");
        vr::VRProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceSearchingAlert_String, m_role == vr::TrackedControllerRole_LeftHand ? "{joyleap}/icons/left_controller_status_searching_alert.gif" : "{leapify}/icons//right_controller_status_searching_alert.gif");
        vr::VRProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceReady_String, m_role == vr::TrackedControllerRole_LeftHand ? "{joyleap}/icons/left_controller_status_ready.png" : "{leapify}/icons//right_controller_status_ready.png");
        vr::VRProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceReadyAlert_String, m_role == vr::TrackedControllerRole_LeftHand ? "{joyleap}/icons/left_controller_status_ready_alert.png" : "{leapify}/icons//right_controller_status_ready_alert.png");
        vr::VRProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceNotReady_String, m_role == vr::TrackedControllerRole_LeftHand ? "{joyleap}/icons/left_controller_status_error.png" : "{leapify}/icons//right_controller_status_error.png");
        vr::VRProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceStandby_String, m_role == vr::TrackedControllerRole_LeftHand ? "{joyleap}/icons/left_controller_status_off.png" : "{leapify}/icons//right_controller_status_off.png");
        vr::VRProperties()->SetStringProperty(props, vr::Prop_NamedIconPathDeviceAlertLow_String, m_role == vr::TrackedControllerRole_LeftHand ? "{joyleap}/icons/left_controller_status_ready_low.png" : "{leapify}/icons//right_controller_status_ready_low.png");

        vr::VRDriverInput()->CreateBooleanComponent(props, "/input/system/click", &m_bindings[0]);
        vr::VRDriverInput()->CreateBooleanComponent(props, "/input/trigger/click", &m_bindings[1]);
        vr::VRDriverInput()->CreateScalarComponent(props, "/input/trigger/value", &m_bindings[2], vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedOneSided);
        vr::VRDriverInput()->CreateScalarComponent(props, "/input/grip/force", &m_bindings[3], vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedOneSided);
        vr::VRDriverInput()->CreateScalarComponent(props, "/input/grip/value", &m_bindings[4], vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedOneSided);
        vr::VRDriverInput()->CreateBooleanComponent(props, "/input/thumbstick/click", &m_bindings[5]);
        vr::VRDriverInput()->CreateScalarComponent(props, "/input/thumbstick/x", &m_bindings[6], vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedTwoSided);
        vr::VRDriverInput()->CreateScalarComponent(props, "/input/thumbstick/y", &m_bindings[7], vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedTwoSided);
        vr::VRDriverInput()->CreateBooleanComponent(props, "/input/a/click", &m_bindings[8]);
        vr::VRDriverInput()->CreateBooleanComponent(props, "/input/b/click", &m_bindings[9]);

        if (m_role == vr::TrackedControllerRole_LeftHand)
            vr::VRDriverInput()->CreateSkeletonComponent(props, "/input/skeleton/left", "/skeleton/hand/left", "/pose/raw", vr::VRSkeletalTracking_Full, nullptr, 0, &m_skeletonHandle);
        else
            vr::VRDriverInput()->CreateSkeletonComponent(props, "/input/skeleton/right", "/skeleton/hand/right", "/pose/raw", vr::VRSkeletalTracking_Full, nullptr, 0, &m_skeletonHandle);

        result = vr::VRInitError_None;
    }

	return result;
}

void TrackedController::Deactivate()
{
}

void TrackedController::EnterStandby()
{
}

void* TrackedController::GetComponent(const char* pchComponentNameAndVersion)
{
    return nullptr;
}

void TrackedController::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize)
{
}

vr::DriverPose_t TrackedController::GetPose()
{
    return m_pose;
}

void TrackedController::Update(bool connected)
{
    m_pose.deviceIsConnected = connected;
    m_pose.poseIsValid = true;

    UpdatePose();
}

void TrackedController::UpdateInput(InputState state)
{
    if (state.valid)
    {
        vr::VRDriverInput()->UpdateBooleanComponent(m_bindings[0], state.system, 0.0f);
        vr::VRDriverInput()->UpdateBooleanComponent(m_bindings[1], state.trigger, 0.0f);
        vr::VRDriverInput()->UpdateScalarComponent(m_bindings[2], state.trigger ? 1.0f : 0.0f, 0.0f);
        vr::VRDriverInput()->UpdateScalarComponent(m_bindings[3], state.grip ? 1.0f : 0.0f, 0.0f);
        vr::VRDriverInput()->UpdateScalarComponent(m_bindings[4], state.grip ? 1.0f : 0.0f, 0.0f);
        vr::VRDriverInput()->UpdateBooleanComponent(m_bindings[5], state.thumbClick, 0.0f);
        vr::VRDriverInput()->UpdateScalarComponent(m_bindings[6], state.thumbX, 0.0f);
        vr::VRDriverInput()->UpdateScalarComponent(m_bindings[7], state.thumbY, 0.0f);
        vr::VRDriverInput()->UpdateBooleanComponent(m_bindings[8], state.aButton, 0.0f);
        vr::VRDriverInput()->UpdateBooleanComponent(m_bindings[9], state.bButton, 0.0f);
    }
}

void TrackedController::UpdatePose()
{
    vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_objectId, GetPose(), sizeof(vr::DriverPose_t));
    vr::VRDriverInput()->UpdateSkeletonComponent(m_skeletonHandle, vr::VRSkeletalMotionRange_WithoutController, nullptr, 0);
}