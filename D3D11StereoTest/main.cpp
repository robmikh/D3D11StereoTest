#include "pch.h"
#include "StereoWindow.h"

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Graphics::Capture;
    using namespace Windows::Graphics::DirectX;
    using namespace Windows::Graphics::DirectX::Direct3D11;
    using namespace Windows::System;
    using namespace Windows::UI;
    using namespace Windows::UI::Composition;
}

namespace util
{
    using namespace robmikh::common::desktop;
    using namespace robmikh::common::uwp;
}

enum class StereoEye
{
    Left = 0,
    Right = 1
};

inline auto CreateRenderTargetView(winrt::com_ptr<ID3D11Device> const& device, winrt::com_ptr<ID3D11Texture2D> const& backBuffer, StereoEye eye)
{
    CD3D11_RENDER_TARGET_VIEW_DESC desc(
        D3D11_RTV_DIMENSION_TEXTURE2DARRAY,
        DXGI_FORMAT_B8G8R8A8_UNORM,
        0,
        static_cast<UINT>(eye),
        1
    );
    winrt::com_ptr<ID3D11RenderTargetView> view;
    winrt::check_hresult(device->CreateRenderTargetView(backBuffer.get(), &desc, view.put()));
    return view;
}

int __stdcall WinMain(HINSTANCE, HINSTANCE, PSTR, int) 
{
    winrt::init_apartment();
    StereoWindow::RegisterWindowClass();

    auto controller = util::CreateDispatcherQueueControllerForCurrentThread();
    auto window = StereoWindow(L"D3D11StereoTest");

    // Setup our visual tree
    auto compositor = winrt::Compositor();
    auto target = window.CreateWindowTarget(compositor);
    auto root = compositor.CreateSpriteVisual();
    root.RelativeSizeAdjustment({ 1, 1 });
    root.Brush(compositor.CreateColorBrush(winrt::Colors::White()));
    target.Root(root);

    auto brush = compositor.CreateSurfaceBrush();
    brush.Stretch(winrt::CompositionStretch::Fill);
    auto content = compositor.CreateSpriteVisual();
    content.RelativeSizeAdjustment({ 1, 1 });
    content.Brush(brush);
    root.Children().InsertAtTop(content);

    // Setup d3d
    auto d3dDevice = util::CreateD3DDevice();
    winrt::com_ptr<ID3D11DeviceContext> d3dContext;
    d3dDevice->GetImmediateContext(d3dContext.put());
    DXGI_SWAP_CHAIN_DESC1 desc = {};
    desc.Width = 100;
    desc.Height = 100;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.Stereo = true;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferCount = 2;
    desc.Scaling = DXGI_SCALING_STRETCH;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    desc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
    auto swapChain = util::CreateDXGISwapChain(d3dDevice, &desc);
    auto surface = util::CreateCompositionSurfaceForSwapChain(compositor, swapChain.get());
    brush.Surface(surface);

    winrt::com_ptr<ID3D11Texture2D> backBuffer;
    winrt::check_hresult(swapChain->GetBuffer(0, winrt::guid_of<ID3D11Texture2D>(), backBuffer.put_void()));

    // Clear the left and right views
    auto leftView = CreateRenderTargetView(d3dDevice, backBuffer, StereoEye::Left);
    auto rightView = CreateRenderTargetView(d3dDevice, backBuffer, StereoEye::Right);

    float leftClear[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    float rightClear[] = { 0.0f, 0.0f, 1.0f, 1.0f };

    d3dContext->ClearRenderTargetView(leftView.get(), leftClear);
    d3dContext->ClearRenderTargetView(rightView.get(), rightClear);

    DXGI_PRESENT_PARAMETERS presentParameters = {};
    swapChain->Present1(1, 0, &presentParameters);

    // Message pump
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return static_cast<int>(msg.wParam);
}
