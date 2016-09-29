#include "stdafx.h"

#include "OpenVRManager.h"
#include "Device.h"

Device::Device()
	: m_eFeatureLevel(D3D_FEATURE_LEVEL_11_0)
{
}

bool Device::init()
{
	HRESULT hr = S_OK;

	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **)&m_pDXGIFactory);
	if (FAILED(hr))
	{
		std::wstringstream ss;
		ss << "Failed to create DXGIFactory1. hr=" << std::hex << hr;
		MessageBox(nullptr, ss.str().c_str(), L"DirectX 11 Initialization Failed", MB_OK | MB_ICONERROR);
		return false;
	}

	hr = m_pDXGIFactory->EnumAdapters1(OpenVRManager::get().adpaterIndex(), &m_pDXGIAdapter);
	if (FAILED(hr))
	{
		std::wstringstream ss;
		ss << "Failed to get adapater " << OpenVRManager::get().adpaterIndex() << ". hr=" << std::hex << hr;
		MessageBox(nullptr, ss.str().c_str(), L"DirectX 11 Initialization Failed", MB_OK | MB_ICONERROR);
		return false;
	}

	hr = m_pDXGIAdapter->EnumOutputs(0, &m_pDXGIOutput);
	if (FAILED(hr))
	{
		std::wstringstream ss;
		ss << "Failed to get output. hr=" << std::hex << hr;
		MessageBox(nullptr, ss.str().c_str(), L"DirectX 11 Initialization Failed", MB_OK | MB_ICONERROR);
		return false;
	}

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL *pCurFeatureLevels = featureLevels;
	UINT dwCurNumFeatureLevels = numFeatureLevels;
	while (*pCurFeatureLevels >= D3D_FEATURE_LEVEL_11_0)
	{
		hr = D3D11CreateDevice(m_pDXGIAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, 0, pCurFeatureLevels, dwCurNumFeatureLevels, D3D11_SDK_VERSION, &m_pDevice, &m_eFeatureLevel, &m_pContext);
		if (hr != E_INVALIDARG)
		{
			break;
		}

		pCurFeatureLevels++;
		dwCurNumFeatureLevels--;
	}
	if (FAILED(hr))
	{
		std::wstringstream ss;
		ss << "Failed to create D3D11 device. hr=" << std::hex << hr;
		MessageBox(nullptr, ss.str().c_str(), L"DirectX 11 Initialization Failed", MB_OK | MB_ICONERROR);
		return false;
	}

	for (uint32_t i = 0; i < 2; i++)
	{
		CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, OpenVRManager::get().eyeWidth(), OpenVRManager::get().eyeHeight(), 1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
		hr = m_pDevice->CreateTexture2D(&textureDesc, nullptr, &m_pEyeTexture[i]);
		if (FAILED(hr))
		{
			std::wstringstream ss;
			ss << "Failed to create eye texture of size " << OpenVRManager::get().eyeWidth() << "x" << OpenVRManager::get().eyeHeight() << ". hr=" << std::hex << hr;
			MessageBox(nullptr, ss.str().c_str(), L"DirectX 11 Initialization Failed", MB_OK | MB_ICONERROR);
			return false;
		}

		CD3D11_RENDER_TARGET_VIEW_DESC rtvDesc(m_pEyeTexture[i].Get(), D3D11_RTV_DIMENSION_TEXTURE2D, textureDesc.Format);
		hr = m_pDevice->CreateRenderTargetView(m_pEyeTexture[i].Get(), &rtvDesc, &m_pEyeRTV[i]);
		if (FAILED(hr))
		{
			std::wstringstream ss;
			ss << "Failed to create eye texture RTV of size " << OpenVRManager::get().eyeWidth() << "x" << OpenVRManager::get().eyeHeight() << ". hr=" << std::hex << hr;
			MessageBox(nullptr, ss.str().c_str(), L"DirectX 11 Initialization Failed", MB_OK | MB_ICONERROR);
			return false;
		}

		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(m_pEyeTexture[i].Get(), D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
		hr = m_pDevice->CreateShaderResourceView(m_pEyeTexture[i].Get(), &srvDesc, &m_pEyeSRV[i]);
		if (FAILED(hr))
		{
			std::wstringstream ss;
			ss << "Failed to create eye texture SRV of size " << OpenVRManager::get().eyeWidth() << "x" << OpenVRManager::get().eyeHeight() << ". hr=" << std::hex << hr;
			MessageBox(nullptr, ss.str().c_str(), L"DirectX 11 Initialization Failed", MB_OK | MB_ICONERROR);
			return false;
		}

		m_EyeViewport[i].TopLeftX = 0;
		m_EyeViewport[i].TopLeftY = 0;
		m_EyeViewport[i].Width = (FLOAT)OpenVRManager::get().eyeWidth();
		m_EyeViewport[i].Height = (FLOAT)OpenVRManager::get().eyeHeight();
		m_EyeViewport[i].MinDepth = 0;
		m_EyeViewport[i].MaxDepth = 1;
	}

	return true;
}
