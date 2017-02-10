/* Copyright (c) 2016-2017, Human Brain Project
 *                          Ahmet.Bilgili@epfl.ch
 */

#pragma once

#include <lexis/api.h>
#include <lexis/render/detail/clipPlanes.h>
#include <vmmlib/types.hpp>

namespace lexis
{
namespace render
{

class ClipPlanes : public detail::ClipPlanes
{
public:
    /**
     * Adds 6 othogonal planes in normalized space (+x,-x,+y,-y,+z,-z). Convex
     * region is defined as an AABB (-0.5,-0.5,-0.5) to (0.5, 0.5, 0.5).
     */
    LEXIS_API ClipPlanes();

    /** @return true if there are no clipping planes. */
    LEXIS_API bool isEmpty() const;

    /** Clears all planes, results in isEmpty() == true. */
    LEXIS_API void clear();

    /** Reset to planes setup by constructor. */
    LEXIS_API void reset();

    /** @return true if the box is outside the clip planes, aka shall be clipped.*/
    LEXIS_API bool isOutside( const vmml::AABBf& box ) const;
};

}
}
