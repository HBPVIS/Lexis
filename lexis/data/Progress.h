
/* Copyright (c) 2016, Human Brain Project
 *                     Stefan.Eilemann@epfl.ch
 */

#ifndef LEXIS_DATA_PROGRESS_H
#define LEXIS_DATA_PROGRESS_H

#include <lexis/api.h>
#include <lexis/data/detail/progress.h> // base class

namespace lexis
{
namespace data
{
/** Drop-in progress meter for boost::progress_display. */
class Progress : public detail::Progress
{
public:
    LEXIS_API explicit Progress( unsigned long expected );
    LEXIS_API Progress( const std::string& operation,
                              unsigned long expected );

    LEXIS_API void restart( unsigned long expected );
    LEXIS_API unsigned long operator+=( unsigned long inc );
    unsigned long operator++() { return operator += ( 1 ); }
    unsigned long count() const { return _count; }

private:
    unsigned long _update();

    unsigned long _expected;
    unsigned long _count;
};
}
}

#endif
