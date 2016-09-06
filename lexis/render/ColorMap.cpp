
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

ControlPoint::ControlPoint( const float x, const float y )
{
    setX( x );
    setY( y );
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
    return getR().empty() &&  getG().empty() &&
           getB().empty() &&  getA().empty();
}

::zerobuf::Vector<detail::ControlPoint>& ColorMap::_getControlPoints(
        const ColorMap::Channel channel )
{
    ::zerobuf::Vector< detail::ControlPoint >* cps = 0;
    switch( channel )
    {
    case Channel::red:
        cps = &getR();
        break;
    case Channel::green:
        cps = &getG();
        break;
    case Channel::blue:
        cps = &getB();
        break;
    case Channel::alpha:
        cps = &getA();
        break;
    case Channel::nChannels:
    default:
        throw( std::runtime_error( "Unsupported channel" ));
    }
    return *cps;
}

void ColorMap::_sortChannels()
{
    if( _isSorted )
        return;

    sortChannel( getR( ));
    sortChannel( getG( ));
    sortChannel( getB( ));
    sortChannel( getA( ));
    _isSorted = true;
}


}
}
