#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

#include <iostream>
#include <filesystem>

namespace tsleddens
{
    class RtwImage
    {
        const int s_bytesPerPixel = 3;

        float* m_fData = nullptr;

        unsigned char* m_bData = nullptr;

        int m_width = 0;
        int m_height = 0;
        int m_bytesPerScanline = 0;

    public:
        RtwImage() = default;
        ~RtwImage()
        {
            delete[] m_bData;
            stbi_image_free(m_fData);
        }

        RtwImage(const char* pFilename)
        {
            auto filename = GetAssetPath(std::string(pFilename));
            if (!Load(filename))
            {
                std::cerr << "ERROR: Could not load image file '" << pFilename << "'.\n";
            }
        }

        bool Load(const std::filesystem::path& filename)
        {
            auto n = s_bytesPerPixel;
            auto utf8 = filename.u8string();
            auto string = std::string(utf8.begin(), utf8.end());
            m_fData = stbi_loadf(string.c_str(), &m_width, &m_height, &n, s_bytesPerPixel);
            bool result = m_fData != nullptr;
            if (result)
            {
                m_bytesPerScanline = m_width * s_bytesPerPixel;
                ConvertToBytes();
            }
            return result;
        }

        int Width() const { return m_fData == nullptr ? 0 : m_width; }
        int Height() const { return m_fData == nullptr ? 0 : m_height; }

        const unsigned char* PixelData(int x, int y) const
        {
            static unsigned char magenta[] = { 255, 0,255 };
            if (m_bData == nullptr) return magenta;

            x = glm::clamp(x, 0, m_width);
            y = glm::clamp(y, 0, m_height);

            return m_bData + (y * m_bytesPerScanline) + (x * s_bytesPerPixel);
        }

    private:
        static unsigned char floatToByte(float value)
        {
            if (value <= 0.f)
                return 0;
            if (1.f <= value)
                return 255;
            return static_cast<unsigned char>(255.999f * value);
        }

        static std::filesystem::path GetExecutableDir() {
            char buffer[MAX_PATH];
            GetModuleFileNameA(NULL, buffer, MAX_PATH);
            return std::filesystem::path(buffer).parent_path();
        }

        static std::filesystem::path GetAssetPath(const std::string& relativePath) {
            return GetExecutableDir() / relativePath;
        }

        void ConvertToBytes()
        {
            int totalBytes = m_width * m_height * s_bytesPerPixel;
            m_bData = new unsigned char[totalBytes];

            auto* pB = m_bData;
            auto* pF = m_fData;
            for (auto i = 0; i < totalBytes; ++i, ++pB, ++pF)
            {
                *pB = floatToByte(*pF);
            }
        }
    };
};
