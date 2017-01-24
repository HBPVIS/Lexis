Color Maps {#colorMaps}
============

Being able to specify a mapping between scalar variables and parameters used
for represent visually those variable is a recurring theme in visualization
applications. An exemplary case is the transfer functions that map density to
RGBA and opacity values in volume rendering with an emission-absorption optical
model.

This specification describes the messages and C++ helper classes to deal with
color maps as a particular case of transfer function (TF). The more generic
case will be addressed in a separate document.

## Requirements

- Represent RGBA colormaps with an independent TF for each channel. The basic
  TF definition should be reusable for the more generic case.
- The C++ helper class will encapsulate a colormap and allow the conversion of
  the network representation into a OpenGL ready lookup table for creating a
  texture.
- The network representation shouldn't restrict the options in the
  implementation of a GUI editor.

## Design

The consensus has been opting for using 2D piece-wise linear functions specified
as (unsorted) lists of control points in R^2. An RGBA colormap is an aggregation
of 4 of this control point lists. Generic TFs will reuse the definition of the
control point list.

Control points are always in the absolute scale, that means that the x coordinate
refers to the scale of the source variable and the y coordinate refers to the
scale of the target attribute. The range of RGBA channels will be [0, 1] unless
an application specifies something different (e.g. in physically based TFs)
For example, a simple black to white colormap for neuron membrane voltage
could be described by the control points (-80, 0), (-10, 1)

A color maps cannot be partially updated, the message always describe the entire
color map. In cases where the graphical editor allows remapping a color map to
a different input range the editor is responsible of finding the adjusted x
positions of the control points. For convenience, the editor may store colormaps
using normalized coordinates for the x axis and then recalculate the positions
when applied to an application.

## Specification

* Values outside of the specified control points are mapped to the closest control point.
* An empty transfer function has the value of 1?

## Messages

    namespace lexis.render.detail; // subnamespace needed to avoid name clashes

    table ControlPoint
    {
        x: double;
        y: double;
    }

    table ColorMap
    {
        red: [ControlPoint];
        green: [ControlPoint];
        blue: [ControlPoint];
    }

    table MaterialMap
    {
        diffuse: ColorMap;
        emission: ColorMap;
        specular: ColorMap;
        alpha: [ControlPoint];
        contribution: [ControlPoint] or double;
    }

    table MembraneVoltageMap
    {
        materialMap: MaterialMap;
        range: [double:2];

        // default control points in -80,20
        foo* sampleColors( size_t nElems ); // absolute to range
    }

    table LFPVoltageMap
    {
        MaterialMap materialMap;
        range: [double:2];

        // default control points in -0.8, 0.8?
        foo* sampleColors( size_t nElems ); // absolute to range
    }

    MembraneCurrentMap, SpikeDensityMap, SpikeActivationMap? as above.


### C++ helper classes

    /**
     *  Wraps a lexis.render.detail.ControlMap
     */
    class ColorMap
    {
        enum class Channel
        {
            red       = 0,
            green     = 1,
            blue      = 2,
            alpha     = 3,
        };

        /**
         * Constructs a color map from a file ( *.lba, *.lbb )
         * @param filename name of the color map file
         */
        ColorMap( const std::string& filename );

        /**
         * Constructs a default color map with a given range.
         *
         * The default color map is a list of 50 points from the Viridis color
         * map from Matplotlib (http://bids.github.io/colormap/).
         */
        ColorMap( float min, float max );

       /**
        * Compute a linear sampling of the color map.
        *
        * @param data A length * 4 array of T that will be filled with the color
        *        values in RGBARGBA.... form. If there are no control points
        *        for a channel, the returned colors will have 0 for that
        *        channel. If there is only one control point, all samples will
        *        have the same color for that channel.  The ith sample will
        *        contain the color for min/(max-min) * i/(length-1) where min
        *        and max are the minimum and maximum x values of all control
        *        points.
        *        Valid types for T are all numerical types.
        * @param length number of sample points
        */
        template< typename T >
        void sampleColors( T* data, size_t length ) const;

       /**
        * Compute a linear sampling of the color map within a specified range
        *
        * @param data A length*4 array of T that will be filled with the color
        *        values in RGBARGBA.... form. If there are no control points
        *        for a channel, the returned colors will have 0 for that
        *        channel. If there is only one control point, all samples will
        *        have the same color for that channel. The ith sample will
        *        contain the color for min/(max-min) * i/(length-1)
        * @param length number of sample points
        * @param min
        * @param max

        */
        template< typename T >
        void sampleColors( T* data, size_t length, float min, float max ) const;

        /**
         * @return the control points, sorted by x value.
         */
        zerobuf::Vector< detail::ControlPoint >
        getControlPoints( Channel channel ) const

        /**
         * Set the list of control points of one channel.
         *
         * Note to implementors: the list list needs to be sorted for internal
         * storage as getControlPoints returns the sorted list.
         */
        void setControlPoints( const std::vector< detail::ControlPoint >& points,
                               Channel channel );

        /** Clears the color map by removing all control points */
        void clear();

        /** @return true if there are no control points in any channel */
        bool isEmpty() const;

        /** Return the range [m', M'] to be used in texture lookups.
         *
         *  This range ensures that the mininum x of the control points maps to
         *  the middle of the first texel (1/2t) and maximum x of the control
         *  points maps to (1 - 1/2t), where t in the texture size.
         */
        std::pair< float, float > getTextureMappingRange( size_t textureSize );

        servus::Serializable& getSerializable();

        const servus::Serializable& getSerializable() const;

        /* Other functions that may be useful */

        /**
         * Set the control points from a list of map of x values to RGBA tuples.
         *
         * It could be a contructor as well
         */
        void setControlPoints( const std::map< float, vmml::Vector4f >& points );

        /**
         * Set the control points from a list of RGBA tuples and a range.
         *
         * The number of control points will be points.size() and the x values
         * will be the linear interpolation of [min, max] at 1/points.size()
         * intervals.
         *
         * It could be a contructor as well
         */
        void setControlPoints( const std::vector< vmml::Vector4f >& points,
                               float min, float max );

        /**
         * Readjust the x values of all control points to find the new min, max
         * range.
         *
         * Each channel will be adjusted preserving its relative range to the
         * max and min x-values for all channels.
         */
        void adjustPoints(float min, float max);
    }

## Issues

### 1: Why a color map is not described with a lookup table ?

_Resolved: Yes_

Because a lookup table is a predefined sampling of the transfer function. On
the other hand a piecewise function with control points can be more precise
for transfer functions with high frecuencies and more lightweight for simple
linear functions. Furthermore, depending on the rendering engine a lookup table
may not be needed or may not be sufficient (e.g. pre-integrated volume rendering)
The C++ API still provides a function to easily convert the message into a
lookup table as this is needed by OpenGL.

### 2: Why the colormap uses independent control point lists for each channel ?

_Resolved: Yes_

Colormap editors typically have RGBA control points, however, advanced transfer
function editors have separate curves for each channel. An event with separate
control point lists easily allows the latter, while it's still trivial to use
it for the former.

### 3: Why the ColorMap::sampleColors functions take a plain pointer instead of using unique_ptr or returning std::vector

_Resolved: Yes_

This function is intended to fill texture memory to be used in OpenGL or ISPC so
it should not make any assumption about how the memory is handled or allocate
it itself, leaving the plain pointer as the only alternative.

### 4: ZeroEQ doesn't allow to subscribe multiple instances of a single serializable type neither in the PUB/SUB nor the HTTP interfaces. How does this specification allow the support of multiple colormaps per applications (e.g. as RTNeuron supports in its Python API)

_Resolved: Yes_

Deferred to object instance support (or equivalent) in ZeroEQ or application
specific support.

At the application level, the simplest alternative is to use messages as events
that request updates on objects stored on the receiver side and add a new field
to the messages to identify the target object. Another option is aggregation
on bigger application specific objects.

Solved in http::Server by registering to different endpoints. Pub-Sub can only
have one mapping per type ID.

### 5: Why control points x-values are absolute?

_Resolved: Yes_

Absolute values require additional semantics to describe what is the intent of a
colormap. However, normalized values (in the range [0, 1]) are not sufficient
from the application point of view (overall if synchronized views are
considered), because the absolute range to which the color map maps is still
needed and we want the user to choose this range, not the application to decide
it automatically.

Since absolute values are needed anyway, we opt for the simplest representation:
using absolute values.

For storage on disk, generic color maps can be stored always with normalized
coordinates, and adjusted after loading, but that is up to the front end
interface to decide.

Scrapped ideas to the one above:

    // v2 absolute with semantic events
    table ColorMap
    {
        red: [ControlPoint];
        green: [ControlPoint];
        blue: [ControlPoint];
        alpha: [ControlPoint];
    }

    table MembraneVoltageMap
    {
        ColorMap colorMap;
        foo* sampleColors( size_t nElems ); // absolute to range from points
    }

    table LFPVoltageMap
    {
        ColorMap colorMap;
        foo* sampleColors( size_t nElems ); // absolute to range from points
    }


    // v3 absolute in one event with semantics
    table ColorMap
    {
        ColorMap( enum dunit );
        red: [ControlPoint];
        green: [ControlPoint];
        blue: [ControlPoint];
        alpha: [ControlPoint];
        dunit: enum;
    }



### 6: Should the control point lists be sorted by x-value on the wire-protocol?

_Resolved: No_

Despite the ControlMap C++ ensures that the points will be always sorted, for
robustness it should not be assumed that the received points are in order.
However it is not totally clear to me that it's possible to reorder the
points upon reception without some undesired side-effect.
