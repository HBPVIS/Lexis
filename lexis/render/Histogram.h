/* Copyright (c) 2016-2017, Human Brain Project
 *                          Ahmet.Bilgili@epfl.ch
 */

#pragma once

#include <lexis/api.h>
#include <lexis/render/detail/histogram.h> // base class
#include <vmmlib/types.hpp>

namespace lexis
{
namespace render
{

class Histogram : public detail::Histogram
{
public:
    /** The minimum value is set to -inf and maximum value is set to -inf. */
    LEXIS_API Histogram();

    /**
     * Computes the addition of two histograms and modifies the object.
     *
     * If the histogram is empty this operator behaves the same as the
     * assignment operator.
     * @param histogram is the histogram to add
     * @return the modified histogram
     */
    LEXIS_API Histogram& operator+=( const Histogram& histogram );

    /** @return true if two histograms are identical. */
    LEXIS_API bool operator==( const Histogram& rhs ) const;

    /** @return true if two histograms are not identical. */
    LEXIS_API bool operator!=( const Histogram& rhs ) const;

    /**
     * @return the index with the minimum value (if there are multiple indices
     *         for min value, the smallest index is returned).
     */
    LEXIS_API size_t getMinIndex() const;

    /**
     * @return the index with the maximum value (if there are multiple indices
     *         for max value, the smallest index is returned).
     */
    LEXIS_API size_t getMaxIndex() const;

    /** @return true if histogram has no values. */
    LEXIS_API bool isEmpty() const;

    /** @return the sum of the histogram. */
    LEXIS_API uint64_t getSum() const;

    /** @return the data range of the histogram. */
    LEXIS_API vmml::Vector2f getRange() const;

    /**
     * Computes the ratio of the value at a given index.
     *
     * @param index the index of the histogram value
     * @return the ratio at given index. If histogram is empty or index exceeds
     *         the histogram bin count, returns 0.0.
     */
    LEXIS_API double getRatio( size_t index ) const;

    /** Sets the number of bins to newSize and clears the histogram. */
    LEXIS_API void resize( size_t newSize );
};

}
}
