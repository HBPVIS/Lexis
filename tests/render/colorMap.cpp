
/* Copyright (c) 2016, Blue Brain Project
 *                     Ahmet Bilgili <ahmet.bilgili@epfl.ch>
 */

#define BOOST_TEST_MODULE colorMap

#include <boost/test/unit_test.hpp>

#include <lexis/render/ColorMap.h>

const size_t sampleCount = 256;
const uint8_t testEmptyColor = 0;

lexis::render::ColorMap createColorMap()
{
    lexis::render::ControlPoint ctrlPnts[] =
        {
            { 0.7f, 0.1f },
            { 5.0f, 0.9f },
            { 12.0f, 0.1f },
            { 3.2f, 0.4f },
            { 16.3f, 0.7f },
            { 2.0f, 0.1f },
            { 2.3f, 0.4f },
            { 14.2f, 0.3f }
        };

    lexis::render::ColorMap colorMap;
    for( size_t i = 0; i < 8; ++i )
    {
        colorMap.addControlPoint( ctrlPnts[ i ], lexis::render::ColorMap::Channel::red );
        colorMap.addControlPoint( ctrlPnts[ i ], lexis::render::ColorMap::Channel::green );
        colorMap.addControlPoint( ctrlPnts[ i ], lexis::render::ColorMap::Channel::blue );
        colorMap.addControlPoint( ctrlPnts[ i ], lexis::render::ColorMap::Channel::alpha );
    }
    return colorMap;
}

template< class T >
void checkEqual( const lexis::render::Colors< T >& cm, const T* buffer )
{
    size_t pos = 0;
    for( size_t i = 0; i < sampleCount; ++i )
    {
        BOOST_CHECK_EQUAL( cm[ i ].r, buffer[ pos++ ] );
        BOOST_CHECK_EQUAL( cm[ i ].g, buffer[ pos++ ] );
        BOOST_CHECK_EQUAL( cm[ i ].b, buffer[ pos++ ] );
        BOOST_CHECK_EQUAL( cm[ i ].a, buffer[ pos++ ] );
    }
}

BOOST_AUTO_TEST_CASE(controlPointsRange_uint8_t)
{
    auto colorMap = createColorMap();
    const auto& colors = colorMap.sampleColors< uint8_t >( sampleCount );
    uint8_t buffer[ 4 * sampleCount ] = { 0 };
    colorMap.sampleColors( &buffer[ 0 ], sampleCount );
    checkEqual( colors, buffer );
    BOOST_CHECK_EQUAL( colors[ 0 ].r,
            uint8_t( 0.1f * std::numeric_limits< uint8_t >::max( )));
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].r,
            uint8_t( 0.7f * std::numeric_limits< uint8_t >::max( )));
}

BOOST_AUTO_TEST_CASE(controlPointsRange_float)
{
    auto colorMap = createColorMap();
    const auto& colors = colorMap.sampleColors< float >( sampleCount );
    float buffer[ 4 * sampleCount ] = { 0 };
    colorMap.sampleColors( &buffer[ 0 ], sampleCount );
    checkEqual( colors, buffer );
    BOOST_CHECK_EQUAL( colors[ 0 ].r, 0.1f );
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].r, 0.7f );
}

BOOST_AUTO_TEST_CASE(inRange)
{
    const float begin = 1.1f;
    const float end = 13.1f;
    auto colorMap = createColorMap();
    uint8_t buffer[ 4 * sampleCount ] = { 0 };
    const auto& colors = colorMap.sampleColors< uint8_t >( sampleCount, begin, end, testEmptyColor );
    colorMap.sampleColors( &buffer[ 0 ], sampleCount, begin, end, testEmptyColor );
    checkEqual( colors, buffer );
}

BOOST_AUTO_TEST_CASE(exactRange)
{
    const float begin = 0.7f;
    const float end = 16.3f;
    auto colorMap = createColorMap();
    uint8_t buffer[ 4 * sampleCount ] = { 0 };
    const auto& colors = colorMap.sampleColors< uint8_t >( sampleCount, begin, end, testEmptyColor );
    colorMap.sampleColors( &buffer[ 0 ], sampleCount, begin, end, testEmptyColor );
    checkEqual( colors, buffer );
}

BOOST_AUTO_TEST_CASE(outsideRange)
{
    const float begin = 0.02f;
    const float end = 20.1f;
    auto colorMap = createColorMap();
    uint8_t buffer[ 4 * sampleCount ] = { 0 };
    const auto& colors = colorMap.sampleColors< uint8_t >( sampleCount, begin, end, testEmptyColor );
    colorMap.sampleColors( &buffer[ 0 ], sampleCount, begin, end, testEmptyColor );
    checkEqual( colors, buffer );
}

BOOST_AUTO_TEST_CASE(leftOutsideRange)
{
    const float begin = 0.02f;
    const float end = 10.2f;
    auto colorMap = createColorMap();
    uint8_t buffer[ 4 * sampleCount ] = { 0 };
    const auto& colors = colorMap.sampleColors< uint8_t >( sampleCount, begin, end, testEmptyColor );
    colorMap.sampleColors( &buffer[ 0 ], sampleCount, begin, end, testEmptyColor );
    checkEqual( colors, buffer );
}

BOOST_AUTO_TEST_CASE(rightOutsideRange)
{
    const float begin = 5.02f;
    const float end = 22.1f;
    auto colorMap = createColorMap();
    uint8_t buffer[ 4 * sampleCount ] = { 0 };
    const auto& colors = colorMap.sampleColors< uint8_t >( sampleCount, begin, end, testEmptyColor );
    colorMap.sampleColors( &buffer[ 0 ], sampleCount, begin, end, testEmptyColor );
    checkEqual( colors, buffer );
}


