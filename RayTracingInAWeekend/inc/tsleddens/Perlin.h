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
            const float u = p.x - std::floor(p.x);
            const float v = p.y - std::floor(p.y);
            const float w = p.z - std::floor(p.z);

            const int i = static_cast<int>(std::floor(p.x));
            const int j = static_cast<int>(std::floor(p.y));
            const int k = static_cast<int>(std::floor(p.z));

            float c[2][2][2];

            for (int di = 0; di < 2; ++di)
            {
                for (int dj = 0; dj < 2; ++dj)
                {
                    for (int dk = 0; dk < 2; ++dk)
                    {
                        c[di][dj][dk] = m_randomFloats[
                            m_permuteX[(i + di) & 255] ^
                            m_permuteY[(j + dj) & 255] ^
                            m_permuteZ[(k + dk) & 255]
                        ];
                    }
                }
            }

            return TrilinearInterpolation(c, u, v, w);
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

        static void Permute(int* p, const int n)
        {
            for (int i = n - 1; i > 0; --i)
            {
                const int target = RandomInt(0, i);
                const int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }

        static float TrilinearInterpolation(float c[2][2][2], const float u, const float v, const float w)
        {
            float accumulation = 0.f;
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        const float fI = static_cast<float>(i);
                        const float fJ = static_cast<float>(j);
                        const float fK = static_cast<float>(k);
                        accumulation +=
                              (fI * u + (1.f - fI) * (1.f - u))
                            * (fJ * v + (1.f - fJ) * (1.f - v))
                            * (fK * w + (1.f - fK) * (1.f - w))
                            * c[i][j][k];
                    }
                }
            }
            return accumulation;
        }
    };
};
