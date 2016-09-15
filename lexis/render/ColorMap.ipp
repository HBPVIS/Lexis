
/* Copyright (c) 2016, Human Brain Project
 *                     Ahmet.Bilgili@epfl.ch
 */

#include "ColorMap.h"

static const size_t nChannels = 4;

template< typename T >
Colors< T > ColorMap::sampleColors( size_t count ) const
{
    return sampleColors< T >( count,
                              -std::numeric_limits< float >::max(),
                              std::numeric_limits< float >::max(),
                              static_cast< T >( 0 ));
}

template< typename T >
void ColorMap::sampleColors( T* data, size_t count ) const
{
    return sampleColors< T >( data,
                              count,
                              -std::numeric_limits< float >::max(),
                              std::numeric_limits< float >::max(),
                              static_cast< T >( 0 ));
}

template< typename T >
Colors< T > ColorMap::sampleColors( size_t count, float min, float max, T emptyColor ) const
{
    Colors< T > colors( count, { emptyColor, emptyColor, emptyColor, emptyColor });
    sampleColors( reinterpret_cast< T* >( colors.data( )), count, min, max, emptyColor );
    return colors;
}

template< typename T >
void ColorMap::sampleColors( T* data, size_t count, float min, float max, T emptyColor ) const
{
    if( isEmpty( ))
        return;

    _sortChannels();

    const Channel channels[] = { Channel::red, Channel::green, Channel::blue, Channel::alpha };

    float minX = min; float maxX = max;
    if( min == -std::numeric_limits< float >::max( ) && max == std::numeric_limits< float >::max( ))
    {
        minX = std::numeric_limits< float >::max();
        maxX = -std::numeric_limits< float >::max();
        for( const auto& channel: channels )
        {
            const ::zerobuf::Vector< detail::ControlPoint >& cps = getControlPoints( channel );

            if( cps.empty())
                continue;

            const detail::ControlPoint& begin = cps[ 0 ];
            const detail::ControlPoint& end = cps[ cps.size() - 1 ];
            if( minX > begin.getX( ))
                minX = begin.getX();

            if( maxX < end.getX( ))
                maxX = end.getX();
        }
    }

    for( const auto& channel: channels )
    {
        const ::zerobuf::Vector< detail::ControlPoint >& cps = getControlPoints( channel );

        const size_t cpsSize = cps.size();

        if( cpsSize == 0 )
        {
            for( size_t sample = 0; sample < count; ++sample )
                data[ sample * nChannels + ( size_t )channel ] = emptyColor;
            continue;
        }

        const detail::ControlPoint& begin = cps[ 0 ];
        const detail::ControlPoint& end = cps[ cpsSize - 1 ];

        const float cpMinX = begin.getX();
        const float cpMaxX = end.getX();

        // Range is out of bounds
        if( maxX < cpMinX || minX > cpMaxX )
        {
            for( size_t sample = 0; sample < count; ++sample )
                data[ sample * nChannels + (size_t)channel ] = emptyColor;
            continue;
        }

        if( cpsSize == 1 ) // 1 element, fill everything with same control point color
        {
            const T val = std::is_integral< T >::value ?
                            cps[ 0 ].getY() * (float)std::numeric_limits< T >::max() :
                            cps[ 0 ].getY();

            for( size_t sample = 0; sample < count; ++sample )
                data[ sample * nChannels + ( size_t )channel ] = val;
            continue;
        }

        // Find the first control point larger than minimum range, above condition
        // already satisfies the search can go until "cpsSize - 1"
        size_t i = 0;
        while( i < ( cpsSize - 1 ) && cps[ i ].getX() <= minX )
            ++i;

        const float xDiff = ( maxX - minX ) / ( count - 1 );
        for( size_t sample = 0; sample < count; ++sample )
        {
            const float x = minX + xDiff * (float)sample;
            if( x < cpMinX || x > cpMaxX )
            {
                data[ sample * nChannels + (size_t)channel ] = emptyColor;
                continue;
            }

            if( x > cps[ i ].getX( ))
                ++i;

            // If there are consecutive control points with same x value
            // forward to the last one
            while( cps[ i - 1 ].getX() == cps[ i ].getX( ))
                ++i;

            const detail::ControlPoint& current = cps[ i - 1 ];
            const detail::ControlPoint& next = cps[ i ];

            // Linear interpolation between control points
            const float weight = ( x - current.getX( )) / ( next.getX() - current.getX( ));
            const float currentColor = current.getY();
            const float nextColor = next.getY();
            const float val = currentColor * ( 1.0f - weight ) + nextColor * weight;

            data[ sample * nChannels + (size_t)channel ] =
                std::is_integral< T >::value ? val * (float)std::numeric_limits< T >::max() : val;
        }
    }
}

#include <ostream>

namespace
{
template< class T >
std::ostream& operator<<( std::ostream& os, const ::lexis::render::Color< T >& color )
{
    os << "r: " << color.r << " "
       << "g: " << color.g << " "
       << "b: " << color.b << " "
       << "a:"  << color.a;
    return os;
}
}
