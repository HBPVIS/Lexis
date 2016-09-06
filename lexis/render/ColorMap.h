
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

// Templated 4-channel color class
template< typename T >
struct Color
{
    T r,g,b,a;
};

// Control point helper class
struct ControlPoint : public detail::ControlPoint
{
    ControlPoint( const float x, const float y )
    {
        setX( x );
        setY( y );
    }

    ControlPoint( const ControlPoint& cp )
        : detail::ControlPoint( cp ) {}

    ControlPoint& operator=( const ControlPoint& cp )
    {
        detail::ControlPoint::operator=( cp );
        return *this;
    }
};

// Vector with list of Colors can directly be uploaded to GPU
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
        nChannels = 4
    };

    LEXIS_API ColorMap();
    LEXIS_API ~ColorMap();

    /**
     * Copy a colormap.
     * @param cm The colormap to be copied.
     */
    ColorMap( const ColorMap& cm )
        : detail::ColorMap( cm ) {}

    ColorMap& operator=( const ColorMap& cm )
    {
        detail::ColorMap::operator=( cm );
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
    LEXIS_API Colors< T > sampleColors( size_t count );

    /**
     * Samples colors linearly in the given region of control points, given the
     * number of sample points
     * @param data is filled with the color values in RGBARGBA.... form. If there are no control
     * points for a channel, the returned colors will have 0 for that channel. If there is only one
     * control point, all samples will have the same color for that channel.
     * @param count number of sample points
     */
    template< typename T >
    LEXIS_API void sampleColors( T* data, size_t count );

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
    LEXIS_API Colors< T > sampleColors( size_t count, float min, float max, T emptyColor );

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
    LEXIS_API void sampleColors( T* data, size_t count, float min, float max, T emptyColor );

    /**
     * Adds a control point to a given channel. If there is already a control point for the
     * given cp.x, it is replaced with the given control point.
     * @param cp control points
     */
    LEXIS_API void addControlPoint( const detail::ControlPoint& cp, Channel channel );

    /** returns true if there are no control points in any channel */
    LEXIS_API bool isEmpty() const;

private:

    ::zerobuf::Vector< detail::ControlPoint >& _getControlPoints( Channel channel );
    void _sortChannels();
    bool _isSorted;
};

template< typename T >
Colors< T > ColorMap::sampleColors( size_t count )
{
    return sampleColors< T >( count,
                              -std::numeric_limits< float >::infinity(),
                              std::numeric_limits< float >::infinity(),
                              static_cast< T >( 0 ));
}

template< typename T >
void ColorMap::sampleColors( T* data, size_t count )
{
    return sampleColors< T >( data,
                              count,
                              -std::numeric_limits< float >::infinity(),
                              std::numeric_limits< float >::infinity(),
                              static_cast< T >( 0 ));
}

template< typename T >
Colors< T > ColorMap::sampleColors( size_t count, float min, float max, T emptyColor )
{
    Colors< T > colors( count, { emptyColor, emptyColor, emptyColor, emptyColor });
    sampleColors( reinterpret_cast< T* >( colors.data( )), count, min, max, emptyColor );
    return colors;
}

template< typename T >
void ColorMap::sampleColors( T* data, size_t count, float min, float max, T emptyColor )
{
    _sortChannels();
    const Channel channels[] = { Channel::red, Channel::green, Channel::blue, Channel::alpha };
    for( const auto& channel: channels )
    {
        ::zerobuf::Vector< detail::ControlPoint >& cps = _getControlPoints( channel );
        const size_t cpsSize = cps.size();
        if( cps.empty( )) // No element, do nothing
            continue;
        else if( cpsSize == 1 ) // 1 element
        {
            for( size_t sample = 0; sample < count; ++sample )
                data[ sample * ( size_t )Channel::nChannels + ( size_t )channel ] = cps[ 0 ].getY();
            continue;
        }

        const detail::ControlPoint& begin = cps[ 0 ];
        const detail::ControlPoint& end = cps[ cpsSize - 1 ];

        const float cpMinX = begin.getX();
        const float cpMaxX = end.getX();
        const float minX = min == -std::numeric_limits< float >::infinity() ? cpMinX : min;
        const float maxX = max == std::numeric_limits< float >::infinity() ? cpMaxX : max;

        // Range is out of bounds
        if( maxX < cpMinX || minX > cpMaxX )
        {
            for( size_t sample = 0; sample < count; ++sample )
                data[ sample * (size_t)Channel::nChannels + (size_t)channel ] = emptyColor;
            continue;
        }

        // Find the first control point larger than minimum range
        size_t i = 0;
        while( i < cpsSize - 1 )
        {
            detail::ControlPoint& c = cps[ i ];
            if( c.getX() > minX )
                break;
            ++i;
        }

        const float xDiff = ( maxX - minX ) / ( count - 1 );
        for( size_t sample = 0; sample < count; ++sample )
        {
            const float x = minX + xDiff * (float)sample;
            if( x < cpMinX || x > cpMaxX )
                data[ sample * (size_t)Channel::nChannels + (size_t)channel ] = emptyColor;
            else
            {
                if( x > cps[ i ].getX( ))
                    ++i;

                const detail::ControlPoint& current = cps[ i - 1 ];
                const detail::ControlPoint& next = cps[ i ];

                // Linear interpolation between control points
                const float weight = ( x - current.getX( )) / ( next.getX() - current.getX( ));
                const float currentColor = current.getY();
                const float nextColor = next.getY();
                const float val = currentColor * ( 1.0f - weight ) + nextColor * weight;

                data[ sample * (size_t)Channel::nChannels + (size_t)channel ] =
                    std::is_integral< T >::value ? val * (float)std::numeric_limits< T >::max() : val;
            }

        }
    }
}
}
}

#endif // LEXIS_RENDER_COLORMAP_H
