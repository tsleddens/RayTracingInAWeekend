#include "AppBase.h"

using namespace tsleddens;

void AppBase::OnInit()
{
    InitWorld(m_world);
    m_bvh = BvhNode(m_world, 0, m_world.GetObjectCount());
}

void AppBase::OnResize(UINT newWidth, UINT newHeight)
{
    m_camera.Resize(newWidth, newHeight);
    m_frameCount = 0;
}

void AppBase::UpdateFps()
{
    ++m_frameCount;
    static ULONGLONG framesPassed = m_frameCount;
    static ULONGLONG prevCount = GetTickCount();
    if (GetTickCount64() - prevCount > 1000) // only update every second
    {
        static wchar_t buffer[256] = {};
        (void)swprintf_s(buffer, L"FPS: %d, Samples: %llu", static_cast<int>(m_frameCount - framesPassed), m_frameCount);
        SetWindowTitle(buffer);
        framesPassed = m_frameCount;
        prevCount = GetTickCount64();
    }
}
