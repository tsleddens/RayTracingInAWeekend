#include "BvhNode.h"

#include "HitResult.h"
#include "World.h"

using namespace tsleddens;

BvhNode::BvhNode( World& world, size_t start, size_t end )
{
    std::shared_ptr<IRayTraceable>* objects = world.GetObjects();
    m_boundingBox                           = AABB();
    for ( auto i = start; i < end; ++i )
    {
        m_boundingBox = AABB( m_boundingBox, objects[i]->BoundingBox() );
    }

    EAxis axis = m_boundingBox.LongestAxis();

    auto fCompare = ( axis == EAxis::X ) ? BoxXCompare : ( axis == EAxis::Y ) ? BoxYCompare : BoxZCompare;

    auto objectSpan = end - start;

    if ( objectSpan == 1 )
    {
        m_left = m_right = objects[start];
    }
    else if ( objectSpan == 2 )
    {
        m_left  = objects[start];
        m_right = objects[start + 1];
    }
    else
    {
        std::sort( objects + start, objects + end,
                   [fCompare]( const std::shared_ptr<IRayTraceable>& a, const std::shared_ptr<IRayTraceable>& b ) {
                       return fCompare( a.get(), b.get() );
                   } );
        auto mid = start + objectSpan / 2;
        m_left   = std::make_shared<BvhNode>( world, start, mid );
        m_right  = std::make_shared<BvhNode>( world, mid, end );
    }
}

bool BvhNode::Intersect( const Ray& ray, HitResult& hitResult, Range<float> range ) const
{
    if ( m_boundingBox.IsHit( ray, range ) )
    {
        bool hitLeft = m_left->Intersect( ray, hitResult, range );

        Range<float> rightRange( range.GetMin(), hitLeft ? hitResult.GetDistance() : range.GetMax() );
        bool         hitRight = m_right->Intersect( ray, hitResult, rightRange );

        return hitLeft || hitRight;
    }

    return false;
}
