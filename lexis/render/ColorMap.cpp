
/* Copyright (c) 2016, Human Brain Project
 *                     Ahmet.Bilgili@epfl.ch
 */
#include "ColorMap.h"

#include <algorithm>

namespace lexis
{
namespace render
{
namespace
{
void sortChannel( ::zerobuf::Vector< detail::ControlPoint >& cps )
{
    std::vector< detail::ControlPoint > cpVec;
    cpVec.reserve( cps.size( ));
    for( size_t i = 0; i < cps.size(); ++i )
       cpVec.push_back( cps[ i ] );

    std::sort( cpVec.begin(), cpVec.end(),
        []( const detail::ControlPoint& cp1, const detail::ControlPoint& cp2){
            return cp1.getX() < cp2.getX();
    } );

    cps.clear();
    for( const auto& cp: cpVec )
       cps.push_back( cp );
}
}

ColorMap::ColorMap()
{
    registerDeserializedCallback([&]{ _isSorted = false; });
}

ColorMap::~ColorMap()
{}

void ColorMap::addControlPoint( const detail::ControlPoint& cp, const Channel channel )
{
    ::zerobuf::Vector< detail::ControlPoint >& cps = _getControlPoints( channel );
    for( size_t i = 0; i < cps.size(); ++i )
    {
        detail::ControlPoint& c = cps[ i ];
        if( c.getX() == cp.getX( ))
        {
            c.setY( cp.getY( ));
            return;
        }
    }

    cps.push_back( cp );
    _isSorted = false;
}

bool ColorMap::isEmpty() const
{
    return getRed().empty() &&  getGreen().empty() &&
           getBlue().empty() &&  getAlpha().empty();
}

::zerobuf::Vector<detail::ControlPoint>& ColorMap::_getControlPoints(
        const ColorMap::Channel channel )
{
    switch( channel )
    {
    case Channel::red:
        return getRed();
    case Channel::green:
        return getGreen();
    case Channel::blue:
        return getBlue();
    case Channel::alpha:
        return getAlpha();
    default:
        throw( std::runtime_error( "Unsupported channel" ));
    }
}

void ColorMap::_sortChannels()
{
    if( _isSorted )
        return;

    sortChannel( getRed( ));
    sortChannel( getGreen( ));
    sortChannel( getBlue( ));
    sortChannel( getAlpha( ));
    _isSorted = true;
}


}
}
