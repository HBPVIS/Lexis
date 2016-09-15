
/* Copyright (c) 2016, Human Brain Project
 *                     Ahmet.Bilgili@epfl.ch
 */

#ifndef LEXIS_RENDER_COLORMAP_H
#define LEXIS_RENDER_COLORMAP_H

#include <lexis/api.h>
#include <lexis/render/detail/colorMap.h> // base class

#include <memory>
#include <limits>

namespace lexis
{
namespace render
{

/** Templated 4-channel color class */
template< typename T >
struct Color
{
    T r,g,b,a;
};

/** Control point helper class */
struct ControlPoint : public detail::ControlPoint
{
    /**
     * Constructor
     * @param x position of the control point
     * @param y position of the control point
     */
    ControlPoint( const float x, const float y )
    {
        setX( x );
        setY( y );
    }

    /**
     * Copy a control point.
     * @param cp source control point
     */
    ControlPoint( const ControlPoint& cp )
        : detail::ControlPoint( cp ) {}

    ControlPoint& operator=( const ControlPoint& cp )
    {
        detail::ControlPoint::operator=( cp );
        return *this;
    }
};

/** Definition of a typed vector with list of colors */
template< typename T >
using Colors = std::vector< Color< T >>;

/** 4 channel, 1 dimensional color map defined with control points.  */
class ColorMap : public detail::ColorMap
{
public:
    enum class Channel
    {
        red       = 0,
        green     = 1,
        blue      = 2,
        alpha     = 3,
    };

    LEXIS_API ColorMap();
    LEXIS_API ~ColorMap();

    LEXIS_API ColorMap( const std::string& filename );

    /**
     * Copy a colormap.
     * @param cm source color map.
     */
    LEXIS_API ColorMap( const ColorMap& cm )
        : detail::ColorMap( cm ), _isSorted( false ) {}

    LEXIS_API ColorMap& operator=( const ColorMap& cm )
    {
        detail::ColorMap::operator=( cm );
        _isSorted = false;
        return *this;
    }

    /**
     * Samples colors linearly in the given region of control points, given the
     * number of sample points
     @param count number of sample points
     * @returns colors that are interpolated between the minimum and maximum
     * of the control points.  If there are no control points for a channel, the returned colors
     * will have 0 for that channel. If there is only one control point, all samples will
     * have the same color for that channel.
     */
    template< typename T >
    LEXIS_API Colors< T > sampleColors( size_t count ) const;

    /**
     * Samples colors linearly in the given region of control points, given the
     * number of sample points
     * @param data is filled with the color values in RGBARGBA.... form. If there are no control
     * points for a channel, the returned colors will have 0 for that channel. If there is only one
     * control point, all samples will have the same color for that channel.
     * @param count number of sample points
     */
    template< typename T >
    LEXIS_API void sampleColors( T* data, size_t count ) const;;

    /**
     * Samples and returns colors linearly in the given region of control points, given the
     * number of sample points
     * @param count number of sample points
     * @param min minimum of the range to sample
     * @param max maximum of the range to sample
     * @param emptyColor the samples which are outside the boundaries (min,max) filled with emptyColor
     * @returns colors that are interpolated for given control points range. If there are no
     * control points for a channel, the returned colors will have 0 values for that channel.
     * If there is only one control point, all samples will have the same color for that channel.
     */
    template< typename T >
    LEXIS_API Colors< T > sampleColors( size_t count, float min, float max, T emptyColor ) const;

    /**
     * Samples colors and fills the given buffer linearly in the given region of control points,
     * given the number of sample points
     * @param data is filled with the color values in RGBARGBA.... form. If there are no
     * control points for a channel, the returned colors will have 0 values for that channel.
     * If there is only one control point, all samples will have the same color for that channel.
     * @param count number of sample points
     * @param min minimum of the range to sample
     * @param max maximum of the range to sample
     * @param emptyColor the samples which are outside the boundaries filled with emptyColor
     * @returns colors that are interpolated for given control points range.
     */
    template< typename T >
    LEXIS_API void sampleColors( T* data, size_t count, float min, float max, T emptyColor ) const;

    /**
     * Adds a control point to a given channel. If there is already a control point for the
     * given cp.x, it is replaced with the given control point.
     * @param cp control points
     */
    LEXIS_API void addControlPoint( const detail::ControlPoint& cp, Channel channel );

    /**
     * Removes a control point for a given channel.
     * @param x position of the control point
     * @param channel is the channel of the control points
     * @returns true if control point with x position is removed
     */
    LEXIS_API bool removeControlPoint( float x, Channel channel );

    /**
     * Gets the control point given the index
     * @param x position of the control point
     * @param channel is the channel of the control points
     * @return the control point. If index is out of bounds, returns an invalid control point
     */
    LEXIS_API detail::ControlPoint& getControlPoint( float x, Channel channel );

    /**
     * Gets the control point given the index
     * @param x position of the control point
     * @param channel is the channel of the control points
     * @return the control point. If index is out of bounds, returns an invalid control point
     */
    LEXIS_API detail::ControlPoint getControlPoint( float x, Channel channel ) const;

    /**
     *  Gets the list of control points
     *  @param channel is the channel of the control points
     *  @return the list of control points in a channel
     */
    const ::zerobuf::Vector< detail::ControlPoint >& getControlPoints( Channel channel ) const;

    /**
     *  Gets the list of control points
     *  @param channel is the channel of the control points
     *  @return the list of control points in a channel
     */
    ::zerobuf::Vector< detail::ControlPoint >& getControlPoints( Channel channel );

    /** @return true if there are no control points in any channel */
    LEXIS_API bool isEmpty() const;

private:

    void _sortChannels() const;
    mutable bool _isSorted;
};

/**
 * Gets the range for the 1d texture sampling.
 * @param count is the number of texels
 * @param begin is the center of the first texel
 * @param end is the center of the last texel
 */
void getTextureSampleRange( const size_t count, float& begin, float& end );

#include "ColorMap.ipp"

}
}
#endif // LEXIS_RENDER_COLORMAP_H
