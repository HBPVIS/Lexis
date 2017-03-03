/* Copyright (c) 2017, Human Brain Project
 *                     Daniel.Nachbaur@epfl.ch
 */

#define BOOST_TEST_MODULE Histogram

#include <lexis/render/Histogram.h>
#include <boost/test/unit_test.hpp>
#include <vmmlib/vector.hpp>

BOOST_AUTO_TEST_CASE( equality )
{
    lexis::render::Histogram histogram;
    lexis::render::Histogram other;

    BOOST_CHECK_EQUAL( histogram, other );
    BOOST_CHECK_EQUAL( histogram, histogram );

    other.setMax( 10 );
    BOOST_CHECK_NE( histogram, other );

    histogram.setMax( 10 );
    BOOST_CHECK_EQUAL( histogram, other );

    histogram.setBins( { 1,2,3 } );
    BOOST_CHECK_NE( histogram, other );

    other.setBins( { 1,2,3 } );
    BOOST_CHECK_EQUAL( histogram, other );

    other.setBins( { 1,2 } );
    BOOST_CHECK_NE( histogram, other );
}

BOOST_AUTO_TEST_CASE( sampleCurve )
{
    lexis::render::Histogram histogram;
    const auto& emptySample = histogram.sampleCurve( false, { 0, 1 } );
    BOOST_CHECK( emptySample.empty( ));

    histogram.setBins( { 10, 5, 0, 5 } );
    histogram.setMax( 10 );

    const auto& sample = histogram.sampleCurve( false, { 0, 1 } );
    BOOST_REQUIRE_EQUAL( sample.size(), 4 );
    const std::vector< vmml::Vector2f > expected = { {0, 0}, {0.25, 0.5}, {0.5, 1}, {0.75, 0.5} };
    BOOST_CHECK_EQUAL_COLLECTIONS( sample.begin(), sample.end(),
                                   expected.begin(), expected.end( ));

    const auto& logSample = histogram.sampleCurve( true, { 0, 1 } );
    BOOST_REQUIRE_EQUAL( logSample.size(), 4 );
    const std::vector< vmml::Vector2f > logExpected =
        { {0, 0}, {0.25, 0.30102998}, {0.5, 1.f}, {0.75, 0.30102998} };
    for( size_t i = 0; i < 4; ++i )
    {
        BOOST_CHECK_CLOSE( logSample[i].x(), logExpected[i].x(),
                           std::numeric_limits<float>::epsilon( ));
        BOOST_CHECK_CLOSE( logSample[i].y(), logExpected[i].y(),
                           std::numeric_limits<float>::epsilon( ));
    }

    const auto& sampleRange = histogram.sampleCurve( false, { 0.3, 0.7 } );
    BOOST_REQUIRE_EQUAL( sampleRange.size(), 2 );
    const std::vector< vmml::Vector2f > expectedRange = { {0, 0.5}, {0.5, 1} };
    BOOST_CHECK_EQUAL_COLLECTIONS( sampleRange.begin(), sampleRange.end(),
                                   expectedRange.begin(), expectedRange.end( ));
}
