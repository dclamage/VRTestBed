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

	const Matrix &cameraBase(uint32_t dwEye) const { return m_mCameraBase; }
	void setCameraBase(Matrix &cameraBase) { m_mCameraBase = cameraBase; }

	const Matrix &invView(uint32_t dwEye) const { return m_mInvView[dwEye]; }
	const Matrix &view(uint32_t dwEye) const { return m_mView[dwEye]; }
	const Matrix &proj(uint32_t dwEye) const { return m_mProj[dwEye]; }
	const Matrix &viewProj(uint32_t dwEye) const { return m_mViewProj[dwEye]; }

private:
	OpenVRManager();

	vr::IVRSystem *m_pVRSystem;
	vr::IVRCompositor *m_pComp;
	vr::IVRResources *m_pResources;

	uint32_t m_dwEyeWidth;
	uint32_t m_dwEyeHeight;
	int32_t m_dwAdapaterIndex;

	vr::TrackedDevicePose_t m_RenderPose[vr::k_unMaxTrackedDeviceCount];

	Matrix m_mCameraBase;
	Matrix m_mInvView[2];
	Matrix m_mView[2];
	Matrix m_mProj[2];
	Matrix m_mViewProj[2];
};