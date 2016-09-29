#pragma once

class Device
{
public:
	Device(Device &o) = delete;
	static Device &get()
	{
		static Device inst;
		return inst;
	}

	bool init();

	ComPtr<ID3D11Device> device() const { return m_pDevice; }
	ComPtr<ID3D11DeviceContext> context() const { return m_pContext; }

	D3D_FEATURE_LEVEL featureLevel() const { return m_eFeatureLevel; }
	ComPtr<ID3D11Texture2D> eyeTexture(uint32_t dwEye) const { return m_pEyeTexture[dwEye]; }
	ComPtr<ID3D11RenderTargetView> eyeRTV(uint32_t dwEye) const { return m_pEyeRTV[dwEye]; }
	ComPtr<ID3D11ShaderResourceView> eyeSRV(uint32_t dwEye) const { return m_pEyeSRV[dwEye]; }
	const D3D11_VIEWPORT &eyeViewport(uint32_t dwEye) const { return m_EyeViewport[dwEye]; }

private:
	Device();

	ComPtr<IDXGIFactory1> m_pDXGIFactory;
	ComPtr<IDXGIAdapter1> m_pDXGIAdapter;
	ComPtr<IDXGIOutput> m_pDXGIOutput;
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pContext;

	ComPtr<ID3D11Texture2D> m_pEyeTexture[2];
	ComPtr<ID3D11RenderTargetView> m_pEyeRTV[2];
	ComPtr<ID3D11ShaderResourceView> m_pEyeSRV[2];
	D3D11_VIEWPORT m_EyeViewport[2];

	D3D_FEATURE_LEVEL m_eFeatureLevel;
};
