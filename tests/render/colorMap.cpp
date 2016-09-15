
/* Copyright (c) 2016, Blue Brain Project
 *                     Ahmet Bilgili <ahmet.bilgili@epfl.ch>
 */

#define BOOST_TEST_MODULE colorMap

#include <boost/test/unit_test.hpp>

#include <lexis/render/ColorMap.h>

const size_t sampleCount = 256;
const uint8_t testEmptyColor = 124;
const uint8_t defaultEmptyColor = 0;

lexis::render::ColorMap createColorMap()
{
    lexis::render::ColorMap colorMap;
    lexis::render::ControlPoint rCtrlPnts[] =
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

    for( const lexis::render::ControlPoint& cp: rCtrlPnts )
        colorMap.addControlPoint( cp, lexis::render::ColorMap::Channel::red );

    lexis::render::ControlPoint gCtrlPnts[] =
        {
            { 0.1f, 0.1f },
            { 5.0f, 0.9f },
            { 16.3f, 0.7f },
            { 2.2f, 0.1f }, // Equal x
            { 2.2f, 0.4f }, // 0.4 overwrites
            { 16.9f, 0.3f }
        };

    for( const lexis::render::ControlPoint& cp: gCtrlPnts )
        colorMap.addControlPoint( cp, lexis::render::ColorMap::Channel::green );

    lexis::render::ControlPoint bCtrlPnts[] =
        {
            { 1.2f, 0.1f }, // All samples are filled with same color
        };

    for( const lexis::render::ControlPoint& cp: bCtrlPnts )
        colorMap.addControlPoint( cp, lexis::render::ColorMap::Channel::blue );

    // Empty alpha, so that alpha channel will be filled with empty color
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

uint8_t convertColor( const float color )
{
    return color * (float)std::numeric_limits< uint8_t >::max( );
}

BOOST_AUTO_TEST_CASE(controlPointsRange_uint8)
{
    auto colorMap = createColorMap();
    const auto& colors = colorMap.sampleColors< uint8_t >( sampleCount );
    uint8_t buffer[ 4 * sampleCount ] = { 0 };
    colorMap.sampleColors( &buffer[ 0 ], sampleCount );
    checkEqual( colors, buffer );
    BOOST_CHECK_EQUAL( colors[ 0 ].g, convertColor( 0.1f ));
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].g, convertColor( 0.3f ));
    BOOST_CHECK_EQUAL( colors[ sampleCount / 2 ].b, convertColor( 0.1f ));
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].b, convertColor( 0.1f ));
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].a, defaultEmptyColor );
}

BOOST_AUTO_TEST_CASE(controlPointsRange_float)
{
    auto colorMap = createColorMap();
    const auto& colors = colorMap.sampleColors< float >( sampleCount );
    float buffer[ 4 * sampleCount ] = { 0 };
    colorMap.sampleColors( &buffer[ 0 ], sampleCount );
    checkEqual( colors, buffer );
    BOOST_CHECK_EQUAL( colors[ 0 ].g, 0.1f );
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].g, 0.3f );
    BOOST_CHECK_EQUAL( colors[ sampleCount / 2 ].b, 0.1f );
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].b, 0.1f );
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].a, float( defaultEmptyColor ));
}

BOOST_AUTO_TEST_CASE(inRange_uint8)
{
    const float begin = 1.1f;
    const float end = 13.1f;
    auto colorMap = createColorMap();
    uint8_t buffer[ 4 * sampleCount ] = { 1 };
    const auto& colors =
            colorMap.sampleColors< uint8_t >( sampleCount, begin, end, testEmptyColor );
    colorMap.sampleColors( &buffer[ 0 ], sampleCount, begin, end, testEmptyColor );
    checkEqual( colors, buffer );
    BOOST_CHECK_EQUAL( colors[ 0 ].r, convertColor( 0.1f ));
    BOOST_CHECK_EQUAL( colors[ 0 ].g, uint8_t( 61 )); // x = 1.1 y = 0.242857128 color = 255 * 0.24
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].g, uint8_t( 192 ));
    BOOST_CHECK_EQUAL( colors[ sampleCount / 2 ].b, convertColor( 0.1f ));
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].b, convertColor( 0.1f ));
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].a, uint8_t( testEmptyColor ));
}

BOOST_AUTO_TEST_CASE(inRange_float)
{
    const float begin = 1.1f;
    const float end = 13.1f;
    auto colorMap = createColorMap();
    float buffer[ 4 * sampleCount ] = { 1.0f };
    const auto& colors = colorMap.sampleColors< float >(
                sampleCount, begin, end, float( testEmptyColor ));
    colorMap.sampleColors( &buffer[ 0 ], sampleCount, begin, end, float( testEmptyColor ));
    checkEqual( colors, buffer );

    BOOST_CHECK_EQUAL( colors[ 0 ].r, 0.1f );
    BOOST_CHECK_EQUAL( colors[ 0 ].g, 0.242857128f ); // x = 1.1 y = 0.242857128 color = 255 * y
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].g, 0.756637156f );
    BOOST_CHECK_EQUAL( colors[ sampleCount / 2 ].b, 0.1f );
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].b, 0.1f );
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].a, float( testEmptyColor ));
}

BOOST_AUTO_TEST_CASE(exactRange)
{
    const float begin = 0.1f; // minimum x
    const float end = 16.9f; // maximum x
    auto colorMap = createColorMap();
    uint8_t buffer[ 4 * sampleCount ] = { 0 };
    const auto& colors =
            colorMap.sampleColors< uint8_t >( sampleCount, begin, end, testEmptyColor );
    colorMap.sampleColors( &buffer[ 0 ], sampleCount, begin, end, testEmptyColor );
    checkEqual( colors, buffer );

    BOOST_CHECK_EQUAL( colors[ 0 ].g, convertColor( 0.1f ));
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].g, convertColor( 0.3f ));
    BOOST_CHECK_EQUAL( colors[ sampleCount / 2 ].b, convertColor( 0.1f ));
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].b, convertColor( 0.1f ));
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].a, uint8_t( testEmptyColor ));
}

BOOST_AUTO_TEST_CASE(outOfBounds)
{
    const float begin = 120.0f;
    const float end = 400.2f;
    auto colorMap = createColorMap();
    uint8_t buffer[ 4 * sampleCount ] = { 0 };
    const auto& colors =
            colorMap.sampleColors< uint8_t >( sampleCount, begin, end, testEmptyColor );
    colorMap.sampleColors( &buffer[ 0 ], sampleCount, begin, end, testEmptyColor );
    checkEqual( colors, buffer );
    for( size_t i = 0; i < sampleCount; ++i )
    {
        BOOST_CHECK_EQUAL( colors[ i ].r, testEmptyColor );
        BOOST_CHECK_EQUAL( colors[ i ].g, testEmptyColor );
        BOOST_CHECK_EQUAL( colors[ i ].b, testEmptyColor );
        BOOST_CHECK_EQUAL( colors[ i ].a, testEmptyColor );
    }
}

BOOST_AUTO_TEST_CASE(leftEndOutsideRange)
{
    const float begin = 0.02f;
    const float end = 10.2f;
    auto colorMap = createColorMap();
    uint8_t buffer[ 4 * sampleCount ] = { 0 };
    const auto& colors =
            colorMap.sampleColors< uint8_t >( sampleCount, begin, end, testEmptyColor );
    colorMap.sampleColors( &buffer[ 0 ], sampleCount, begin, end, testEmptyColor );
    checkEqual( colors, buffer );

    BOOST_CHECK_EQUAL( colors[ 0 ].r, testEmptyColor );
    BOOST_CHECK_EQUAL( colors[ 0 ].g, testEmptyColor );
    BOOST_CHECK_EQUAL( colors[ 0 ].b, convertColor( 0.1f ));
    BOOST_CHECK_EQUAL( colors[ 0 ].a, testEmptyColor );
}

BOOST_AUTO_TEST_CASE(rightEndOutsideRange)
{
    const float begin = 1.02f;
    const float end = 22.1f;
    auto colorMap = createColorMap();
    uint8_t buffer[ 4 * sampleCount ] = { 0 };
    const auto& colors = colorMap.sampleColors< uint8_t >( sampleCount, begin, end, testEmptyColor );
    colorMap.sampleColors( &buffer[ 0 ], sampleCount, begin, end, testEmptyColor );
    checkEqual( colors, buffer );

    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].r, testEmptyColor );
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].g, testEmptyColor );
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].b, convertColor( 0.1f ));
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].a, testEmptyColor );
}

BOOST_AUTO_TEST_CASE(bothEndsOutsideRange)
{
    const float begin = 0.01f;
    const float end = 49.3f;
    auto colorMap = createColorMap();
    uint8_t buffer[ 4 * sampleCount ] = { 0 };
    const auto& colors =
            colorMap.sampleColors< uint8_t >( sampleCount, begin, end, testEmptyColor );
    colorMap.sampleColors( &buffer[ 0 ], sampleCount, begin, end, testEmptyColor );
    checkEqual( colors, buffer );

    BOOST_CHECK_EQUAL( colors[ 0 ].r, testEmptyColor );
    BOOST_CHECK_EQUAL( colors[ 0 ].g, testEmptyColor );
    BOOST_CHECK_EQUAL( colors[ 0 ].b, convertColor( 0.1f )); // control point is in range
    BOOST_CHECK_EQUAL( colors[ 0 ].a, testEmptyColor );

    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].r, testEmptyColor );
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].g, testEmptyColor );
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].b, convertColor( 0.1f ));
    BOOST_CHECK_EQUAL( colors[ sampleCount - 1 ].a, testEmptyColor );
}

BOOST_AUTO_TEST_CASE(duplicationOfControlPointXValue)
{
    const float begin = 2.2f;
    const float end = 49.3f;
    auto colorMap = createColorMap();
    float buffer[ 4 * sampleCount ] = { 0 };
    const auto& colors =
            colorMap.sampleColors< float >( sampleCount, begin, end, float( testEmptyColor ));
    colorMap.sampleColors( &buffer[ 0 ], sampleCount, begin, end, float( testEmptyColor ));
    checkEqual( colors, buffer );
    BOOST_CHECK_EQUAL( colors[ 0 ].g, 0.4f ); // At position x = 2.2f, value 0.1f is overwritten
}

BOOST_AUTO_TEST_CASE(textureStartEnd)
{
    float begin, end = 0.0f;
    ::lexis::render::getTextureSampleRange( sampleCount, begin, end );

    const float diff = 1.0f / ( sampleCount - 1 );
    BOOST_CHECK_EQUAL( begin, diff );
    BOOST_CHECK_EQUAL( end, 1.0f - diff );
}
