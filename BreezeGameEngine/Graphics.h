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
#include "Rect.h"
#include "Surface.h"

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
	Color GetPixel(int x, int y) const;

	void SwapIfGrtr(int& a, int& b);
	Rect<int> ScreenRect();
	void DrawRect(int x0, int y0, int x1, int y2, Color c);
	void DrawRect(Rect<int> Rect, Color c);
	void DrawRect(Rect<float> Rect, Color c);
	void DrawCRect(int x0, int y0, int w, int h, Color c);
	void DrawRectDim(int x0, int y0, int w, int h, Color c);
	void DrawCirc(int x0, int y0, int r, Color C);
	void DrawCirc(Vec<int> pos, int r, Color C);
	void DrawCirc(Vec<float> pos, float r, Color C);
	void DrawULIsoTri(int x, int y, int size, Color C);
	void DrawURIsoTri(int x, int y, int size, Color C);
	void DrawDLIsoTri(int x, int y, int size, Color C);
	void DrawDRIsoTri(int x, int y, int size, Color C);
	void DrawLine( Vec<float> p0, Vec<float> p1, Color c);
	void DrawPolylineC( const std::vector<Vec<float>>& vert, Color c);
	void DrawPolylineC(const std::vector<Vec<float>>& vert, Vec<float> translation, float sx, float sy, float th, Color c);
	template<typename E>
	void DrawSprite(int x, int y, const Surface& S, E effect)
	{
		DrawSprite(x, y, S.Rect(), ScreenRect(), S, effect);
	}

	template<typename E>
	void DrawSprite(int x, int y, const Rect<int>& srcRect, const Surface& S, E effect)
	{
		DrawSprite(x, y, srcRect, ScreenRect(), S, effect);
	}

	template<typename E>
	void DrawSprite(int x, int y, Rect<int> srcRect, const Rect<int>& clip, const Surface& S, E effect)
	{
		if (x < clip.X0)
		{
			srcRect.X0 += clip.X0 - x;
			x = clip.X0;
		}
		else if (x + srcRect.width() >= clip.X1)
		{
			srcRect.X1 -= x + srcRect.width() - clip.X1;
		}

		if (y < clip.Y0)
		{
			srcRect.Y0 += clip.Y0 - y;
			y = clip.Y0;
		}
		else if (y + srcRect.height() >= clip.Y1)
		{
			srcRect.Y1 -= y + srcRect.height() - clip.Y1;
		}

		for (int sy = srcRect.Y0; sy < srcRect.Y1; sy++)
		{
			for (int sx = srcRect.X0; sx < srcRect.X1; sx++)
			{
				effect
				(
					S.GetPixel(sx, sy),
					x + sx - srcRect.X0,
					y + sy - srcRect.Y0,
					*this
				);
			}
		}
	}


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
	Color*												pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth = 800;
	static constexpr int ScreenHeight = 600;
};

#include "SpriteEffect.h"
#ifndef SPRITE_OPTIMIZE
extern template
void Graphics::DrawSprite<SpriteEffect::Copy>(int x, int y, Rect<int> srcRect, const Rect<int>& clip, const Surface& S, SpriteEffect::Copy effect);
extern template
void Graphics::DrawSprite<SpriteEffect::Chroma>(int x, int y, Rect<int> srcRect, const Rect<int>& clip, const Surface& S, SpriteEffect::Chroma effect);
extern template
void Graphics::DrawSprite<SpriteEffect::Substitute>(int x, int y, Rect<int> srcRect, const Rect<int>& clip, const Surface& S, SpriteEffect::Substitute effect);
#endif // !SPRITE_OPTIMIZE
