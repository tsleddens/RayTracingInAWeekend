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
    m_sampleCount = 0;
    m_currentSampleStartTime = std::chrono::high_resolution_clock::now();
}

void AppBase::UpdateFps()
{
    static auto lastUpdate = std::chrono::high_resolution_clock::now();
    ++m_sampleCount;
    const auto now = std::chrono::high_resolution_clock::now();
    const auto elapsedSeconds = std::chrono::duration<float>(now - lastUpdate).count();
    const auto elapsedCurrentSampleSeconds = std::chrono::duration<float>(now - m_currentSampleStartTime).count();
    if (elapsedSeconds >= 1.0f) // only update every second
    {
        static wchar_t buffer[256] = {};
        (void)swprintf_s(buffer, L"FPS: %.2f, Samples: %llu, %.2f seconds", static_cast<float>(m_sampleCount) / elapsedCurrentSampleSeconds, m_sampleCount, elapsedCurrentSampleSeconds);
        SetWindowTitle(buffer);
        lastUpdate = now;
    }
}
