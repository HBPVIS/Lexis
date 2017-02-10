/* Copyright (c) 2016-2017, Human Brain Project
 *                          Ahmet.Bilgili@epfl.ch
 */

#define BOOST_TEST_MODULE ClipPlanes

#include <lexis/render/ClipPlanes.h>
#include <vmmlib/aabb.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( testClipping )
{
    // The default clipping planes include space defined by AABB
    // ( -0.5, -0.5, -0.5 ) to ( 0.5, 0.5, 0.5 )

    const vmml::Vector3f cornerOut1( 0.8f, 0.8f, 0.8f );
    const vmml::Vector3f cornerOut2( 0.9f, 0.9f, 0.9f );
    const vmml::Vector3f cornerIn1( -0.3f, -0.3f, -0.3f );
    const vmml::Vector3f cornerIn2(  0.3f, 0.3f, 0.3f );

    const vmml::AABBf boxInside( cornerIn1, cornerIn2 );
    const vmml::AABBf boxOutside( cornerOut1, cornerOut2 );
    const vmml::AABBf boxIntersect( cornerIn1, cornerOut2 );

    lexis::render::ClipPlanes clipPlanes;

    BOOST_CHECK( !clipPlanes.isOutside( boxInside ));
    BOOST_CHECK( clipPlanes.isOutside( boxOutside ));
    BOOST_CHECK( !clipPlanes.isOutside( boxIntersect ));

    BOOST_CHECK( !clipPlanes.isEmpty( ));
    clipPlanes.clear();
    BOOST_CHECK( clipPlanes.isEmpty( ));

    clipPlanes.reset();
    BOOST_CHECK( !clipPlanes.isEmpty( ));

    BOOST_CHECK( !clipPlanes.isOutside( boxInside ));
    BOOST_CHECK( clipPlanes.isOutside( boxOutside ));
    BOOST_CHECK( !clipPlanes.isOutside( boxIntersect ));
}
