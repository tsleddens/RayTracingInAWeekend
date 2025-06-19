#pragma once

namespace tsleddens
{
    class ONB
    {
        Vector3 m_axis[3];
    public:
        ONB(const Vector3& n)
        {
            m_axis[2] = glm::normalize(n);
            const Vector3 a = (std::fabs(m_axis[2].x) > 0.9f) ? Vector3(0.f, 1.f, 0.f) : Vector3(1.f, 0.f, 0.f);
            m_axis[1] = glm::normalize(glm::cross(m_axis[2], a));
            m_axis[0] = glm::cross(m_axis[2], m_axis[1]);
        }

        [[nodiscard]] const Vector3& U() const { return m_axis[0]; }
        [[nodiscard]] const Vector3& V() const { return m_axis[1]; }
        [[nodiscard]] const Vector3& W() const { return m_axis[2]; }

        [[nodiscard]] Vector3 Transform(const Vector3& v) const
        {
            return (v[0] * m_axis[0]) + (v[1] * m_axis[1]) + (v[2] * m_axis[2]);
        }
    };
}
