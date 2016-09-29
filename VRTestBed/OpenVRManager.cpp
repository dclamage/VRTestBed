#include "stdafx.h"

#include "Device.h"
#include "OpenVRManager.h"

OpenVRManager::OpenVRManager()
	: m_pVRSystem(nullptr)
{
}

bool OpenVRManager::init()
{
	if (!vr::VR_IsHmdPresent())
	{
		MessageBox(nullptr, L"No VR Headset found! Please ensure it's plugged in and SteamVR is installed!", L"OpenVR Initialization Failed", MB_OK | MB_ICONERROR);
		return false;
	}

	vr::HmdError err;
	m_pVRSystem = vr::VR_Init(&err, vr::VRApplication_Scene);
	if (!m_pVRSystem)
	{
		std::wstringstream ss;
		ss << "Failed to initialize OpenVR." << std::endl << vr::VR_GetVRInitErrorAsEnglishDescription(err);
		MessageBox(nullptr, ss.str().c_str(), L"OpenVR Initialization Failed", MB_OK | MB_ICONERROR);
		return false;
	}

	m_pVRSystem->GetDXGIOutputInfo(&m_dwAdapaterIndex);
	m_pVRSystem->GetRecommendedRenderTargetSize(&m_dwEyeWidth, &m_dwEyeHeight);

	m_pComp = vr::VRCompositor();
	if (!m_pComp)
	{
		MessageBox(nullptr, L"Failed to get VR Compositor!", L"OpenVR Initialization Failed", MB_OK | MB_ICONERROR);
		return false;
	}

	m_pResources = vr::VRResources();
	if (!m_pResources)
	{
		MessageBox(nullptr, L"Failed to get VR Resources!", L"OpenVR Initialization Failed", MB_OK | MB_ICONERROR);
		return false;
	}

	// Show preview window
	m_pComp->ShowMirrorWindow();

	return true;
}

void OpenVRManager::shutdown()
{
	vr::VR_Shutdown();

	m_pVRSystem = nullptr;
	m_pComp = nullptr;
	m_pResources = nullptr;
}

void OpenVRManager::beginFrame()
{
	m_pComp->WaitGetPoses(&m_RenderPose[0], vr::k_unMaxTrackedDeviceCount, nullptr, 0);
	m_mProj[vr::Eye_Left] = m_pVRSystem->GetProjectionMatrix(vr::Eye_Left, 0.3f, 100.f, vr::API_DirectX);
	m_mProj[vr::Eye_Right] = m_pVRSystem->GetProjectionMatrix(vr::Eye_Right, 0.3f, 100.f, vr::API_DirectX);
}

void OpenVRManager::endFrame()
{
	ComPtr<ID3D11DeviceContext> pContext = Device::get().context();

	for (uint32_t dwEye = 0; dwEye < 2; dwEye++)
	{
		vr::EVREye eEye = (vr::EVREye)dwEye;

		vr::Texture_t texture = { Device::get().eyeTexture(dwEye).Get(), vr::API_DirectX, vr::ColorSpace_Auto };

		vr::VRTextureBounds_t bounds;
		bounds.uMin = 0.0f;
		bounds.uMax = 1.0f;
		bounds.vMin = 0.f;
		bounds.vMax = 1.f;

		m_pComp->Submit(eEye, &texture, &bounds);
	}
	pContext->Flush();
}

bool OpenVRManager::isPreviewWindowOpen()
{
	return m_pComp->IsMirrorWindowVisible();
}
