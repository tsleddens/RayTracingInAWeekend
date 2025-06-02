#pragma once

namespace tsleddens
{
    class DeFocusDisk
    {
        float m_angle;
        float m_distance;

        Point3 m_u;
        Point3 m_v;

    public:
        DeFocusDisk(const float angle, const float distance);

        void GenerateDisk(const Vector3& u, const Vector3& v);

        float GetDistance() const { return m_distance; }
        Point3 GetPosition(const Point3& position) const;

    private:
        static Vector3 RandomVector3InUnitDisk();
    };
}