#pragma once

class OpenVRManager
{
public:
	OpenVRManager(OpenVRManager &o) = delete;
	static OpenVRManager &get()
	{
		static OpenVRManager inst;
		return inst;
	}

	bool init();
	void shutdown();

	void beginFrame();
	void endFrame();

	bool isPreviewWindowOpen();

	uint32_t eyeWidth() const { return m_dwEyeWidth; }
	uint32_t eyeHeight() const { return m_dwEyeHeight; }
	int32_t adpaterIndex() const { return m_dwAdapaterIndex; }

private:
	OpenVRManager();

	vr::IVRSystem *m_pVRSystem;
	vr::IVRCompositor *m_pComp;
	vr::IVRResources *m_pResources;

	uint32_t m_dwEyeWidth;
	uint32_t m_dwEyeHeight;
	int32_t m_dwAdapaterIndex;

	vr::TrackedDevicePose_t m_RenderPose[vr::k_unMaxTrackedDeviceCount];
	vr::HmdMatrix44_t m_mProj[2];
};