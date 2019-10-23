/***************************************************************************/
/***               Temper Tech DirectX Framework V 1.0.0                 ***/
/*** Graphics.h                                                          ***/
/*** Copyright for all time (if your law disagrees, it can suck it)      ***/
/***                                                                     ***/
/*** Part of the Temper DirectX Framework                                ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech                      ***/
/***                                                                     ***/
/*** If you need help, go fuck yourself.                                 ***/
/***************************************************************************/

#pragma once

#include <d3d11.h>
#include <wrl.h>
#include "BreezeException.h"
#include "Colors.h"

class Graphics
{
public:
	class Exception : public BreezeException
	{
	public:
		Exception(HRESULT hr, const std::wstring& note, const wchar_t* file, unsigned int line);
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x, y, z;            //position
		float u, v;                //texcoords
	};
public:
	Graphics(class HWNDKey& key);
	Graphics(const Graphics&) = delete;
	Graphics& operator = (const Graphics&) = delete;
	void BeginFrame();
	void EndFrame();
	void PutPixel(int x, int y, int r, int g, int b)
	{
		PutPixel(x, y, { unsigned char(r), unsigned char(g), unsigned char(b) });
	}
	void PutPixel(int x, int y, Color c);

	void SwapIfGrtr(int& a, int& b);
	void DrawRect(int x0, int y0, int x1, int y2, Color c);
	void DrawCRect(int x0, int y0, int w, int h, Color c);
	void DrawRectDim(int x0, int y0, int w, int h, Color c);
	
	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color* pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth = 800;
	static constexpr int ScreenHeight = 600;
};