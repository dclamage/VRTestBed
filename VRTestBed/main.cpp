#include "stdafx.h"
#include "OpenVRManager.h"
#include "Device.h"

static bool HandleMessages()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	if (!OpenVRManager::get().init())
	{
		return -1;
	}

	if (!Device::get().init())
	{
		return -1;
	}

	while (HandleMessages())
	{
		if (!OpenVRManager::get().isPreviewWindowOpen())
		{
			break;
		}

		OpenVRManager::get().beginFrame();

		// Render to eyes here, via: Device::get().eyeRTV(dwEye)
		FLOAT pfClearColor[4]{ 1.f, 0.5f, 0.f, 1.f };
		for (uint32_t dwEye = 0; dwEye < 2; dwEye++)
		{
			Device::get().context()->ClearRenderTargetView(Device::get().eyeRTV(dwEye).Get(), pfClearColor);
		}

		OpenVRManager::get().endFrame();
	}

	OpenVRManager::get().shutdown();

	return 0;
}