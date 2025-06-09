#pragma once

namespace tsleddens
{
    class Perlin
    {
        static constexpr int s_pointCount = 256;
        float m_randomFloats[s_pointCount];
        int m_permuteX[s_pointCount];
        int m_permuteY[s_pointCount];
        int m_permuteZ[s_pointCount];

    public:
        Perlin()
        {
            for (float& m_randomFloat : m_randomFloats)
            {
                m_randomFloat = RandomFloat();
            }

            GeneratePermute(m_permuteX);
            GeneratePermute(m_permuteY);
            GeneratePermute(m_permuteZ);
        }

        float Noise(const Point3& p) const
        {
            const int i = static_cast<int>(4.f * p.x) & 255;
            const int j = static_cast<int>(4.f * p.y) & 255;
            const int k = static_cast<int>(4.f * p.z) & 255;

            return m_randomFloats[m_permuteX[i] ^ m_permuteX[j] ^ m_permuteX[k]];
        }

    private:
        static void GeneratePermute(int* p)
        {
            for (int i = 0; i < s_pointCount; ++i)
            {
                p[i] = i;
            }

            Permute(p, s_pointCount);
        }

        static void Permute(int* p, int n)
        {
            for (int i = n-1; i > 0; --i)
            {
                const int target = RandomInt(0, i);
                const int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }
    };
};
