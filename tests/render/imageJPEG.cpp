
/* Copyright (c) 2016, Human Brain Project
 *                     Cyrille Favreau <cyrille.favreau@epfl.ch>
 */

#define BOOST_TEST_MODULE imageJPEG

#include <boost/test/unit_test.hpp>

#include <lexis/render/imageJPEG.h>
#include <utility>

const size_t NB_BYTES = 16;
const std::string expectedJSON = "{\n"
                                 "   \"data\" : \"\"\n"
                                 "}\n";
const std::string expectedJSONWithData = "{\n"
                                 "   \"data\" : \"AAECAwQFBgcICQoLDA0ODw==\"\n"
                                 "}\n";

BOOST_AUTO_TEST_CASE(emptyImageJPEG)
{
    const lexis::render::ImageJPEG imageJPEG;
    BOOST_CHECK( imageJPEG.getData().size() == 0 );
}

BOOST_AUTO_TEST_CASE(initializeImageJPEG)
{
    lexis::render::ImageJPEG imageJPEG;
    std::vector< uint8_t > v;
    for( uint8_t i = 0; i < NB_BYTES; ++i )
        v.push_back(i);
    imageJPEG.setData( v );
    const std::string& json = imageJPEG.toJSON();
    BOOST_CHECK_EQUAL( json, expectedJSONWithData );
}

BOOST_AUTO_TEST_CASE(copyConstructImageJPEG)
{
    lexis::render::ImageJPEG temporary;
    std::vector< uint8_t > v;
    for( uint8_t i = 0; i < NB_BYTES; ++i )
        v.push_back(i);
    temporary.setData( v );
    lexis::render::ImageJPEG imageJPEG( temporary );
    BOOST_CHECK( imageJPEG == temporary );
}

BOOST_AUTO_TEST_CASE(moveConstructImageJPEG)
{
    lexis::render::ImageJPEG temporary;
    std::vector< uint8_t > v;
    for( uint8_t i = 0; i < NB_BYTES; ++i )
        v.push_back(i);
    temporary.setData( v );
    lexis::render::ImageJPEG imageJPEG( std::move( temporary ));
    BOOST_CHECK( imageJPEG.getData().size() == NB_BYTES );
    BOOST_CHECK( temporary.getData().size() == 0 );
    BOOST_CHECK_NE( imageJPEG, temporary );
}

BOOST_AUTO_TEST_CASE(moveImageJPEG)
{
    lexis::render::ImageJPEG temporary;
    std::vector< uint8_t > v;
    for( uint8_t i = 0; i < NB_BYTES; ++i )
        v.push_back(i);
    temporary.setData( v );
    lexis::render::ImageJPEG imageJPEG;
    imageJPEG = std::move( temporary );
    BOOST_CHECK( imageJPEG.getData().size() == NB_BYTES );
    BOOST_CHECK( temporary.getData().size() == 0 );
    BOOST_CHECK_NE( imageJPEG, temporary );
}

BOOST_AUTO_TEST_CASE(ImageJPEGJSON)
{
    lexis::render::ImageJPEG imageJPEG;

    const std::string& json = imageJPEG.toJSON();
    BOOST_CHECK_EQUAL( json, expectedJSON );

    lexis::render::ImageJPEG imageJPEG2;
    imageJPEG2.fromJSON( json );
    BOOST_CHECK( imageJPEG == imageJPEG2 );

    BOOST_CHECK( !imageJPEG.fromJSON( "blubb" ));
}
