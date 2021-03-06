#ifndef TRAINS_CLOTHOID_CHAIN_H
#define TRAINS_CLOTHOID_CHAIN_H

#include "Clothoid.h"

#include <list>

template <typename L, typename C>
class ClothoidChain
{
public:
    typedef ::Clothoid<L, C> Clothoid;
    typedef typename Clothoid::Length Length;
    typedef typename Clothoid::Angle Angle;
    typedef typename Clothoid::Curvature Curvature;
    typedef typename Clothoid::CurvatureRate CurvatureRate;
    typedef typename Clothoid::Vec2l Vec2l;
    typedef typename Clothoid::Mat22l Mat22l;
    typedef std::list<Clothoid> ClothoidList;

private:
    ClothoidList chain;

public:

    // Constructor
    ClothoidChain()
    {
    }

    // Append a clothoid
    Clothoid *append()
    {
        if (chain.empty()) {
            chain.push_back(Clothoid());
        } else {
            chain.push_back(chain.back().getNextClothoid());
        }
        return &chain.back();
    }

    // Clear the chain
    void clear()
    {
        chain.clear();
    }

    // Clothoid accessors
    const ClothoidList &clothoids() const
    {
        return chain;
    }

    // Calculators

    Length length() const
    {
        Length length = 0;
        for (const Clothoid &clothoid: chain)
            length += clothoid.getLength();
        return length;
    }

    Length parallelLength(Length leftOffset) const
    {
        Length length = 0;
        for (const Clothoid &clothoid: chain)
            length += clothoid.getParallelLength(leftOffset);
        return length;
    }

    Vec2l positionAtLength(Length length) const
    {
        if (length <= 0)
            return chain.front().getStartPosition();
        for (const Clothoid &clothoid: chain) {
            Length clothoidLength = clothoid.getLength();
            if (length < clothoidLength)
                return clothoid.positionAtLength(length);
            length -= clothoidLength;
        }
        return chain.back().getStartPosition();
    }

    Vec2l parallelPositionAtParallelLength(Length leftOffset, Length length,
                                           Mat22l *outRotMatrix = nullptr) const
    {
        if (length <= 0) {
            return chain.front().getStartParallelPosition(leftOffset, outRotMatrix);
        }
        for (const Clothoid &clothoid: chain) {
            Length clothoidLength = clothoid.getParallelLength(leftOffset);
            if (length < clothoidLength)
                return clothoid.parallelPositionAtParallelLength(leftOffset, length, outRotMatrix);
            length -= clothoidLength;
        }
        return chain.back().getStartParallelPosition(leftOffset, outRotMatrix);
    }
};

#endif // TRAINS_CLOTHOID_CHAIN_H
