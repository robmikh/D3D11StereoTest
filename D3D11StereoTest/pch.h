#pragma once

#include <Unknwn.h>
#include <inspectable.h>

#include <wil/cppwinrt.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <winrt/Windows.Graphics.DirectX.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Composition.Desktop.h>

#include <vector>
#include <memory>
#include <string>

#include <wil/resource.h>

#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d2d1_3.h>
#include <wincodec.h>

#include <robmikh.common/composition.desktop.interop.h>
#include <robmikh.common/composition.interop.h>
#include <robmikh.common/d3dHelpers.h>
#include <robmikh.common/d3dHelpers.desktop.h>
#include <robmikh.common/direct3d11.interop.h>
#include <robmikh.common/stream.interop.h>
#include <robmikh.common/hwnd.interop.h>
#include <robmikh.common/dispatcherqueue.desktop.interop.h>
#include <robmikh.common/capture.desktop.interop.h>