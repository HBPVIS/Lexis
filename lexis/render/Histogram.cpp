/* Copyright (c) 2016-2017, Human Brain Project
 *                          Ahmet.Bilgili@epfl.ch
 */

#include "Histogram.h"

#include <vmmlib/vector.hpp>

namespace lexis
{
namespace render
{

Histogram::Histogram()
{
    setMin( std::numeric_limits<float>::infinity( ));
    setMax( -std::numeric_limits<float>::infinity( ));
}

Histogram& Histogram::operator+=( const Histogram& histogram )
{
    if( histogram.getBins().empty( ))
        return *this;

    if( getBins().empty( ))
    {
        *this = histogram;
        return *this;
    }

    if( histogram.getBins().size() != getBins().size() ||
        histogram.getMin() != getMin() ||
        histogram.getMax() != getMax( ))
    {
        throw std::runtime_error( "Addition of incompatible histograms" );
    }

    const uint64_t* srcBins = histogram.getBins().data();

    uint64_t* bins = getBins().data();
    for( size_t i = 0; i < getBins().size(); ++i )
        bins[ i ] += srcBins[ i ];

    return *this;
}

bool Histogram::operator==( const Histogram& rhs ) const
{
    if( this == &rhs )
        return true;

    return getMin() == rhs.getMin() &&
           getMax() == rhs.getMax() &&
           getBins().size() == rhs.getBins().size() &&
           memcmp( getBins().data(), rhs.getBins().data(),
                   getBins().size() * sizeof( uint64_t ) ) == 0;
}

bool Histogram::operator!=( const Histogram& rhs ) const
{
    return !(*this == rhs);
}

size_t Histogram::getMinIndex() const
{
    const uint64_t* bins = getBins().data();
    return std::distance( bins, std::min_element( bins, bins + getBins().size( )));
}

size_t Histogram::getMaxIndex() const
{
    const uint64_t* bins = getBins().data();
    return std::distance( bins, std::max_element( bins, bins + getBins().size( )));
}

bool Histogram::isEmpty() const
{
    const uint64_t* bins = getBins().data();
    for( size_t i = 0; i < getBins().size(); ++i )
    {
        if( bins[ i ] > 0 )
            return false;
    }
    return true;
}

uint64_t Histogram::getSum() const
{
    const uint64_t* bins = getBins().data();
    uint64_t sum = 0;
    for( size_t i = 0; i < getBins().size(); ++i )
        sum +=  bins[ i ];

    return sum;
}

vmml::Vector2f Histogram::getRange() const
{
    return { getMin(), getMax() };
}

double Histogram::getRatio( const size_t index ) const
{
    if( index >= getBins().size( ))
        return 0.0f;

    const uint64_t sum = getSum();
    if( sum == 0 )
        return 0.0f;

    const uint64_t* bins = getBins().data();
    return (double)bins[ index ] / (double)sum;
}

void Histogram::resize( size_t newSize )
{
    getBins().resize( newSize );
}

}
}
