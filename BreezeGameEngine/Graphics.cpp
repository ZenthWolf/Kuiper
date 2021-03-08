/***************************************************************************/
/***               Temper Tech DirectX Framework V 1.0.0                 ***/
/*** Graphics.cpp                                                        ***/
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

#include "MainWindow.h"
#include "Graphics.h"
#include "DXErr.h"
#include <assert.h>
#include <string>
#include <array>

// Should be created during the build sequence before preprocessing
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib,"d3d11.lib" )


#define BREEZE_GFX_EXCEPTION( hr,note ) Graphics::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

using Microsoft::WRL::ComPtr;

Graphics::Graphics(HWNDKey& key)
{
	assert(key.hWnd != nullptr);

	//Creat Device and swap chain/get render target view
	/* Swap Chain- holds buffer frames to work on while drawing current frame*/
	/* Prevents screen tear */
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Graphics::ScreenWidth;
	sd.BufferDesc.Height = Graphics::ScreenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 60;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = key.hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	HRESULT				hr;
	UINT				createFlags = 0u;
#ifdef BREEZE_USE_D3D_DEBUG_LAYER
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif


	// create device and front/back buffers
	if (FAILED(hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pImmediateContext)))
	{
		throw BREEZE_GFX_EXCEPTION(hr, L"Creating device and swap chain");
	}


	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if (FAILED(hr = pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer)))
	{
		throw BREEZE_GFX_EXCEPTION(hr, L"Getting back buffer");
	}


	// create a view on backbuffer that we can render to
	if (FAILED(hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView)))
	{
		throw BREEZE_GFX_EXCEPTION(hr, L"Creating render target view on backbuffer");
	}

	// set backbuffer as the render target using created view
	/* Basically, "render the next frame", I think?*/
	pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);

	// set viewport dimensions
	D3D11_VIEWPORT vp;
	vp.Width = float(Graphics::ScreenWidth);
	vp.Height = float(Graphics::ScreenHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports(1, &vp);

	///////////////////////////////////////
	// create texture for cpu render target
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = Graphics::ScreenWidth;
	sysTexDesc.Height = Graphics::ScreenHeight;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	// create the texture
	if (FAILED(hr = pDevice->CreateTexture2D(&sysTexDesc, nullptr, &pSysBufferTexture)))
	{
		throw BREEZE_GFX_EXCEPTION(hr, L"Creating sysbuffer texture");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	if (FAILED(hr = pDevice->CreateShaderResourceView(pSysBufferTexture.Get(),
		&srvDesc, &pSysBufferTextureView)))
	{
		throw BREEZE_GFX_EXCEPTION(hr, L"Creating view on sysBuffer texture");
	}

	////////////////////////////////////////////////
	// create pixel shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if (FAILED(hr = pDevice->CreatePixelShader(
		FramebufferShaders::FramebufferPSBytecode,
		sizeof(FramebufferShaders::FramebufferPSBytecode),
		nullptr,
		&pPixelShader)))
	{
		throw BREEZE_GFX_EXCEPTION(hr, L"Creating pixel shader");
	}

	/////////////////////////////////////////////////
	// create vertex shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if (FAILED(hr = pDevice->CreateVertexShader(
		FramebufferShaders::FramebufferVSBytecode,
		sizeof(FramebufferShaders::FramebufferVSBytecode),
		nullptr,
		&pVertexShader)))
	{
		throw BREEZE_GFX_EXCEPTION(hr, L"Creating vertex shader");
	}

	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	const FSQVertex vertices[] =
	{
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,1.0f,0.5f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(FSQVertex) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if (FAILED(hr = pDevice->CreateBuffer(&bd, &initData, &pVertexBuffer)))
	{
		throw BREEZE_GFX_EXCEPTION(hr, L"Creating vertex buffer");
	}

	//////////////////////////////////////////
	// create input layout for fullscreen quad
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	// Ignore the intellisense error "namespace has no member"
	if (FAILED(hr = pDevice->CreateInputLayout(ied, 2,
		FramebufferShaders::FramebufferVSBytecode,
		sizeof(FramebufferShaders::FramebufferVSBytecode),
		&pInputLayout)))
	{
		throw BREEZE_GFX_EXCEPTION(hr, L"Creating input layout");
	}

	////////////////////////////////////////////////////
	// Create sampler state for fullscreen textured quad
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if (FAILED(hr = pDevice->CreateSamplerState(&sampDesc, &pSamplerState)))
	{
		throw BREEZE_GFX_EXCEPTION(hr, L"Creating sampler state");
	}

	// allocate memory for sysbuffer (16-byte aligned for faster access)
	pSysBuffer = reinterpret_cast<Color*>(
		_aligned_malloc(sizeof(Color)* Graphics::ScreenWidth* Graphics::ScreenHeight, 16u));
}

Graphics::~Graphics()
{
	// free sysbuffer memory (aligned free)
	if (pSysBuffer)
	{
		_aligned_free(pSysBuffer);
		pSysBuffer = nullptr;
	}
	// clear the state of the device context before destruction
	if (pImmediateContext) pImmediateContext->ClearState();
}

void Graphics::EndFrame()
{
	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	if (FAILED(hr = pImmediateContext->Map(pSysBufferTexture.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture)))
	{
		throw BREEZE_GFX_EXCEPTION(hr, L"Mapping sysbuffer");
	}
	// setup parameters for copy operation
	Color* pDst = reinterpret_cast<Color*>(mappedSysBufferTexture.pData);
	const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof(Color);
	const size_t srcPitch = Graphics::ScreenWidth;
	const size_t rowBytes = srcPitch * sizeof(Color);
	// perform the copy line-by-line
	for (size_t y = 0u; y < Graphics::ScreenHeight; y++)
	{
		memcpy(&pDst[y * dstPitch], &pSysBuffer[y * srcPitch], rowBytes);
	}
	// release the adapter memory
	pImmediateContext->Unmap(pSysBufferTexture.Get(), 0u);

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout(pInputLayout.Get());
	pImmediateContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	pImmediateContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	const UINT stride = sizeof(FSQVertex);
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	pImmediateContext->PSSetShaderResources(0u, 1u, pSysBufferTextureView.GetAddressOf());
	pImmediateContext->PSSetSamplers(0u, 1u, pSamplerState.GetAddressOf());
	pImmediateContext->Draw(6u, 0u);

	// flip back/front buffers
	if (FAILED(hr = pSwapChain->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw BREEZE_GFX_EXCEPTION(pDevice->GetDeviceRemovedReason(), L"Presenting back buffer [device removed]");
		}
		else
		{
			throw BREEZE_GFX_EXCEPTION(hr, L"Presenting back buffer");
		}
	}
}

/* Draws rectangle from Point0 to Point 1 */
void Graphics::DrawRect(int x0, int y0, int x1, int y1, Color c)
{
	SwapIfGrtr(x0, x1);
	SwapIfGrtr(y0, y1);

	Rect<int> Screen = ScreenRect();

	if (x0 < Screen.X0)
	{
		x0 = Screen.X0;
	}
	else if (x1 >= Screen.X1)
	{
		x1 = Screen.X1;
	}

	if (y0 < Screen.Y0)
	{
		y0 = Screen.Y0;
	}
	else if (y1 >= Screen.Y1)
	{
		y1 = Screen.Y1;
	}


	for (int x = x0; x <= x1; x++)
	{
		for (int y = y0; y <= y1; y++)
		{
			PutPixel(x, y, c);
		}
	}
}

void Graphics::DrawRect(Rect<float> Rect, Color c)
{
	DrawRect(int(Rect.X0), int(Rect.Y0), int(Rect.X1), int(Rect.Y1), c);
}

void Graphics::DrawRect(Rect<int> Rect, Color c)
{
	DrawRect( int(Rect.X0), int(Rect.Y0), int(Rect.X1), int(Rect.Y1), c );
}

/* Draws Rectangle centered at a point- best used with odd W and H */
void Graphics::DrawCRect(int x0, int y0, int w, int h, Color c)
{
	if (h % 2 == 1)
	{
		h -= 1;
	}
	if (w % 2 == 1)
	{
		w -= 1;
	}

	h = h / 2;
	w = w / 2;

	DrawRect(x0 - w, x0 + w, y0 - h, y0 + h, c);
}

/* Draws Rectangle from P0 (top left) with given width/height */
void Graphics::DrawRectDim(int x0, int y0, int w, int h, Color c)
{
	DrawRect(x0 , y0, x0 + w, y0 + h, c);
}

void Graphics::DrawCirc(int x0, int y0, int r, Color c)
{
	int rsq = r * r;
	for (int y = y0 - r+1; y < y0 + r; y++)
	{
		for (int x = x0 - r+1; x < x0 + r; x++)
		{
			if ((x-x0) * (x-x0) + (y-y0) * (y-y0) <= rsq)
			{
				PutPixel(x, y, c);
			}
		}
	}
}

void Graphics::DrawCirc(Vec<int> pos, int r, Color c)
{
	int rsq = r * r;
	for (int y = pos.Y - r + 1; y < pos.Y + r; y++)
	{
		for (int x = pos.X - r + 1; x < pos.X + r; x++)
		{
			if ((x - pos.X) * (x - pos.X) + (y - pos.Y) * (y - pos.Y) <= rsq)
			{
				PutPixel(x, y, c);
			}
		}
	}
}

void Graphics::DrawCirc(Vec<float> pos, float r, Color c)
{
	float rsq = r * r;
	for (int y = int(pos.Y) - int(r) + 1; y < int(pos.Y) + int(r); y++)
	{
		for (int x = int(pos.X) - int(r) + 1; x < int(pos.X) + int(r); x++)
		{
			if ((float(x) - pos.X) * (float(x) - pos.X) + (float(y) - pos.Y) * (float(y) - pos.Y) <= rsq)
			{
				PutPixel(x, y, c);
			}
		}
	}
}

void Graphics::SwapIfGrtr(int& a, int& b)
{
	//std::swap(a,b)
	if (a > b)
	{
		const int temp = a;
		a = b;
		b = temp;
	}
}

Rect<int> Graphics::ScreenRect()
{
	return { 0, 0, ScreenWidth, ScreenHeight };
}

void Graphics::DrawULIsoTri(int x, int y, int size, Color C)
{
	for (int j = 0; j < size; j++)
	{
		for (int i = 0; i < size - j; i++)
		{
			PutPixel( x + i, y + j, C );
		}
	}
}

void Graphics::DrawURIsoTri(int x, int y, int size, Color C)
{
	for (int j = 0; j < size; j++)
	{
		for (int i = j; i < size; i++)
		{
			PutPixel(x + i, y + j, C);
		}
	}
}

void Graphics::DrawDLIsoTri(int x, int y, int size, Color C)
{
	for (int j = 0; j < size; j++)
	{
		for (int i = 0; i <= j; i++)
		{
			PutPixel(x + i, y + j, C);
		}
	}
}

void Graphics::DrawDRIsoTri(int x, int y, int size, Color C)
{
	for (int j = 0; j < size; j++)
	{
		for (int i = size-j; i <= size; i++)
		{
			PutPixel(x + i, y + j, C);
		}
	}
}


//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception(HRESULT hr, const std::wstring& note, const wchar_t* file, unsigned int line)
	:
	BreezeException(file, line, note),
	hr(hr)
{}

std::wstring Graphics::Exception::GetFullMessage() const
{
	const std::wstring empty = L"";
	const std::wstring errorName = GetErrorName();
	const std::wstring errorDesc = GetErrorDescription();
	const std::wstring& note = GetNote();
	const std::wstring location = GetLocation();
	return    (!errorName.empty() ? std::wstring(L"Error: ") + errorName + L"\n"
		: empty)
		+ (!errorDesc.empty() ? std::wstring(L"Description: ") + errorDesc + L"\n"
			: empty)
		+ (!note.empty() ? std::wstring(L"Note: ") + note + L"\n"
			: empty)
		+ (!location.empty() ? std::wstring(L"Location: ") + location
			: empty);
}

std::wstring Graphics::Exception::GetErrorName() const
{
	return DXGetErrorString(hr);
}

std::wstring Graphics::Exception::GetErrorDescription() const
{
	std::array<wchar_t, 512> wideDescription;
	DXGetErrorDescription(hr, wideDescription.data(), wideDescription.size());
	return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const
{
	return L"Breeze Graphics Exception";
}
