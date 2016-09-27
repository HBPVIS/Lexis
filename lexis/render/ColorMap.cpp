
/* Copyright (c) 2016, Human Brain Project
 *                     Ahmet.Bilgili@epfl.ch
 */
#include "ColorMap.h"

#include <algorithm>
#include <sstream>
#include <fstream>

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
       cpVec.push_back( ControlPoint( cps[ i ].getX(), cps[ i ].getY( )));

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
{}

ColorMap::~ColorMap()
{}

ColorMap::ColorMap( const std::string& filename )
{
    std::ifstream file( filename.c_str( ));

    if( !file.is_open( ))
        *this = getDefaultColorMap( 0.0f, 256.0f );

    std::string line;
    std::stringstream iss( line );
    while( std::getline( file, line ))
        iss << line;

    fromJSON( iss.str( ));
    _isSorted = false;
}

ColorMap ColorMap::getDefaultColorMap( const float min, const float max )
{
    ColorMap colorMap;

    const float dataRange = max - min;
    const lexis::render::ControlPoint redCps[] = {
                                                   { min + 0.0f * dataRange, 0.0f },
                                                   { min + 0.2f * dataRange, 0.6f },
                                                   { min + 1.0f * dataRange, 1.0f }
                                                 };

    for( const auto& cp: redCps )
        colorMap.addControlPoint( cp, lexis::render::ColorMap::Channel::red );


    const lexis::render::ControlPoint greenCps[] = {
                                                   { min + 0.0f * dataRange, 0.0f },
                                                   { min + 0.4f * dataRange, 0.9f },
                                                   { min + 1.0f * dataRange, 1.0f }
                                                 };

    for( const auto& cp: greenCps )
        colorMap.addControlPoint( cp, lexis::render::ColorMap::Channel::green );


    const lexis::render::ControlPoint blueCps[] = {
                                                    { min + 0.0f * dataRange, 0.0f },
                                                    { min + 0.4f * dataRange, 0.8f },
                                                    { min + 1.0f * dataRange, 1.0f }
                                                  };

    for( const auto& cp: blueCps )
        colorMap.addControlPoint( cp, lexis::render::ColorMap::Channel::blue );


    const lexis::render::ControlPoint alphaCps[] = {
                                                     { min + 0.0f * dataRange, 0.0f },
                                                     { min + 0.2f * dataRange, 0.6f },
                                                     { min + 1.0f * dataRange, 1.0f }
                                                   };


    for( const auto& cp: alphaCps )
        colorMap.addControlPoint( cp, lexis::render::ColorMap::Channel::alpha );

    return colorMap;
}

void ColorMap::addControlPoint( const detail::ControlPoint& cp, const Channel channel )
{
    removeControlPoint( cp.getX(), channel );
    auto& cps = getControlPoints( channel );
    cps.push_back( cp );
    _isSorted = false;
}

bool ColorMap::removeControlPoint( const float x, ColorMap::Channel channel )
{
    auto& cps = getControlPoints( channel );
    size_t index = -1u;
    for( size_t i = 0; i < cps.size(); ++i )
    {
        const auto& c = cps[ i ];
        if( c.getX() == x )
        {
            index = i;
            break;
        }
    }

    std::vector< ControlPoint > copy;
    for( size_t i = 0; i < cps.size(); ++i )
    {
        if( i == index )
            continue;

        const auto& c = cps[ i ];
        copy.emplace_back( c.getX(), c.getY( ));
    }

    cps.clear();
    for( const auto& cp: copy )
        cps.push_back( cp );

    return true;
}

bool ColorMap::isEmpty() const
{
    return getRed().empty() &&  getGreen().empty() &&
           getBlue().empty() &&  getAlpha().empty();
}

const ::zerobuf::Vector<detail::ControlPoint>& ColorMap::getControlPoints(
        const ColorMap::Channel channel ) const
{
    _sortChannels();

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

::zerobuf::Vector<detail::ControlPoint>& ColorMap::getControlPoints(
        const ColorMap::Channel channel )
{
    _sortChannels();

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

void ColorMap::clear()
{
    getRed().clear();
    getGreen().clear();
    getBlue().clear();
    getAlpha().clear();
}

void ColorMap::_sortChannels() const
{
    if( _isSorted )
        return;

    ColorMap* colorMap = const_cast< ColorMap* >( this );
    sortChannel( colorMap->getRed( ));
    sortChannel( colorMap->getGreen( ));
    sortChannel( colorMap->getBlue( ));
    sortChannel( colorMap->getAlpha( ));
    _isSorted = true;
}

void getTextureSampleRange( const size_t count, float& begin, float& end )
{
    const float diff = 1.0f / ( count - 1 );
    begin = diff; end = 1.0f - diff;
}

}
}
