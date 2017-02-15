/* Copyright (c) 2016-2017, Human Brain Project
 *                          Ahmet.Bilgili@epfl.ch
 */

#include "ClipPlanes.h"

#include <vmmlib/aabb.hpp>

namespace lexis
{
namespace render
{

ClipPlanes::ClipPlanes()
{
    reset();
}

bool ClipPlanes::isEmpty() const
{
    return getPlanes().empty();
}

void ClipPlanes::clear()
{
    getPlanes().clear();
}

void ClipPlanes::reset()
{
    setPlanes( {
                   { { -1.0f,  0.0f,  0.0f }, 0.5f }, // +X plane
                   { {  1.0f,  0.0f,  0.0f }, 0.5f }, // -X plane
                   { {  0.0f, -1.0f,  0.0f }, 0.5f }, // +Y plane
                   { {  0.0f,  1.0f,  0.0f }, 0.5f }, // -Y plane
                   { {  0.0f,  0.0f, -1.0f }, 0.5f }, // +Z plane
                   { {  0.0f,  0.0f,  1.0f }, 0.5f }  // -Z Plane
                } );
}

bool ClipPlanes::isOutside( const vmml::AABBf& worldBox ) const
{
    for( const auto& plane : getPlanes() )
    {
        const float* normal = plane.getNormal();
        if( worldBox.isInFront( { normal[ 0 ], normal[ 1 ], normal[ 2 ], plane.getD() } ))
            return true;
    }

    return false;
}

}
}
