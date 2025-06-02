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
        DeFocusDisk(const float angle = 0.f, const float distance = 1.f);

        void SetAngle(const float angle) { m_angle = angle; }
        void SetDistance(const float distance) { m_distance = distance; }

        void GenerateDisk(const Vector3& u, const Vector3& v);

        float GetDistance() const { return m_distance; }
        float GetAngle() const { return m_angle; }
        Point3 GetPosition(const Point3& position) const;

    private:
        static Vector3 RandomVector3InUnitDisk();
    };
}