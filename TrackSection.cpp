#include "TrackSection.h"

TrackSection::TrackSection(TrackNode::Reference start,
                           TrackNode::Reference end)
: nodes{start, end}
{
    start.addTrackSection(this);
    end.addTrackSection(this);

    interpolate();
}

/**
 * @brief Calculate more complete clothoid parameters for a double opposite
 *        clothoid.
 * @param directionDelta[in]      Final direction (radians CCW from initial
 *                                direction, in range -M_PI..M_PI).
 * @param curvatureA[in]          Initial curvature (radians/length CCW).
 * @param curvatureB[in]          Final curvature (radians/length CCW).
 * @param curvatureRateAC[in]     Chosen curvature rate to maximum curvature.
 * @param outCurvatureRateCB[out] Curvature rate from maximum curvature to end.
 * @param outLengthAC[out]        Length of curve to maximum curvature.
 * @param outLengthCB[out]        Length of curve from maximum curvature to end.
 * @returns Whether a solution was found.
 */
bool TrackSection::calcImpliedFromCurvatureRateAC(float directionDelta,
                                                  float curvatureA, float curvatureB,
                                                  float curvatureRateAC,
                                                  float *outCurvatureRateCB,
                                                  float *outLengthAC, float *outLengthCB)
{
    // clothoid curve A -> C with curvatureRateAC & length lengthAC
    // clothoid curve C -> B with -curvatureRateAC & length lengthCB

    // Given the curvature rate, calculate lengthAC
    // Solve quadratic equation linking length to curvature rate
    // 0 = (directionA - directionB) + 1/2*curvatureA²/curvatureRateAC
    //   + 2*curvatureA * lengthAC
    //   + curvatureRateAC * lengthAC²
    const float a = curvatureRateAC;
    const float halfB = curvatureA;
    const float c = -directionDelta + (curvatureA*curvatureA - curvatureB*curvatureB)/(curvatureRateAC*2);
    const float quarterB2m4ac = halfB*halfB - a*c;
    if (quarterB2m4ac < 0)
        return false;
    float sqrtQuarterB2m4ac = sqrtf(quarterB2m4ac);
    float lengthAC = (-halfB - sqrtQuarterB2m4ac) / a;
    float lengthCB = lengthAC + (curvatureA - curvatureB) / curvatureRateAC;
    if (lengthAC < 0 || lengthCB < 0) {
        lengthAC = (-halfB + sqrtQuarterB2m4ac) / a;
        lengthCB = lengthAC + (curvatureA - curvatureB) / curvatureRateAC;
    }
    if (lengthAC < 0 || lengthCB < 0)
        return false;
    if (lengthCB < 0)
        return false;

    *outCurvatureRateCB = -curvatureRateAC;
    *outLengthAC = lengthAC;
    *outLengthCB = lengthCB;
    return true;
}

/**
 * @brief Calculate curvature rates to eliminate loops with a fixed curvature
 *        straight/curve after, using with a pair of clothoids.
 * @param directionA            Direction at start (radians CCW from east).
 * @param curvatureA            Curvature at start (radians/length CCW).
 * @param positionB             Position of a known point on straight.
 * @param directionB            Direction of straight away from start (radians
 *                              CCW from east).
 * @param curvatureB            Curvature at end (radians/length CCW).
 * @param outRate1[out]         Curvature rate from start to max curvature
 *                              (radians/length² CCW).
 * @param outLen1[out]          Length of clothoid from start to max curvature.
 * @param outRate2[out]         Curvature rate from max curvature to straight.
 * @param outLen2[out]          Length of clothoid from max curvature to
 *                              straight.
 * @param outPositionB[out]     Position of start of straight.
 * @param outExtraStraight[out] Length of extra straight to reach @b.
 * @returns Whether a solution was found.
 */
bool TrackSection::interpolateClothoidPair(float directionA, float curvatureA,
                                           const Vec2f &positionB, float directionB,
                                           float curvatureB,
                                           float *outRate1, float *outLen1,
                                           float *outRate2, float *outLen2,
                                           Vec2f *outPositionB, float *outExtraStraight)
{
    // TODO handle curvatureB != 0
    constexpr unsigned int maxIterations = 32;
    // We know the start angle and curvature.
    // We know the final straight line we need to line up with.
    // We need to find a solution with 2 clothoids of equal and opposite
    // curvature rate.

    float directionDelta = directionB - directionA;
    while (directionDelta < -M_PI)
        directionDelta += M_PI*2;
    while (directionDelta > M_PI)
        directionDelta -= M_PI*2;

    // FIXME, need a better guess? than using curvatureRate?
    const float curvatureRate = 1.0f / 30.0f / 10.0f;
    float guess = directionDelta / positionB.sqr() * 4;
    if (guess < -curvatureRate)
        guess = -curvatureRate/100;
    else if (guess > curvatureRate)
        guess = curvatureRate/100;

    ClothoidT c1, c2;
    Vec2f tangentVec;
    sincosf(directionB, &tangentVec[1], &tangentVec[0]);
    Vec2f perpendicularVec(-tangentVec[1], tangentVec[0]);

    float curvatureRateAC = guess;
    float curvatureRateCB[2];
    float lengthAC[2], lengthCB[2];
    Vec2f finalPos[2];
    float dist[2];
    const float minDist = 0.00001f;
    for (int it = 0; it < maxIterations; ++it) {
        const float delta = curvatureRateAC/100;
        c1.setStartDirection(directionA);
        c1.setStartCurvature(curvatureA);

        for (int i = 0; i < 2; ++i) {
            if (!calcImpliedFromCurvatureRateAC(directionDelta, curvatureA,
                                                curvatureB, curvatureRateAC + delta*i,
                                                &curvatureRateCB[i],
                                                &lengthAC[i], &lengthCB[i])) {
                return false;
            }

            c1.setCurvatureRate(curvatureRateAC + delta*i);
            c1.setLength(lengthAC[i]);
            c2 = c1.getNextClothoid();
            c2.setCurvatureRate(curvatureRateCB[i]);
            c2.setLength(lengthCB[i]);

            finalPos[i] = c2.getEndPosition();
            dist[i] = (finalPos[i] - positionB) * perpendicularVec;
        }

        // Iterate, using the Neuton-Raphson method
        curvatureRateAC -= delta * dist[0] / (dist[1] - dist[0]);

        if (fabs(dist[0]) < minDist) {
            // Successfully converged
            *outRate1 = curvatureRateAC;
            *outLen1 = lengthAC[0];
            *outRate2 = curvatureRateCB[0];
            *outLen2 = lengthCB[0];
            *outPositionB = finalPos[0];
            *outExtraStraight = (positionB - finalPos[0]) * tangentVec;
            return true;
        }
    }
    // No convergence, fail!
    return false;

}

struct SegmentParams {
    float length;
};
struct TransitionParams : public SegmentParams {
    float rate;
};

struct TrackSectionParams {
    // Transition curves
    TransitionParams t1, t2, t3, t4;
    // Fixed curves
    SegmentParams c1, c2;
    // Straights
    SegmentParams s;
};

void TrackSection::interpolate()
{
    constexpr bool unloop1 = true;
    constexpr bool unloop2 = true;
    constexpr int d1start = 0;
    constexpr int d1end = 2;
    constexpr int d2start = 0;
    constexpr int d2end = 2;

    // TODO pick an appropriate rate
    const float curvatureRate = 1.0f / 30.0f / 10.0f;
    const float minCurvature = 1.0f / 30.0f;
    const float maxCurvature = 1.0f / 15.0f;
    const float startDirection = nodes[0].getDirection();
    const float startCurvature = nodes[0].getCurvature();
    const float endDirectionRev = nodes[1].getDirection();
    const float endCurvatureRev = nodes[1].getCurvature();

    float direction1 = -1.0f;
    float direction2 = 1.0f;

    float minCurvature1 = minCurvature;
    float minCurvature2 = minCurvature;
    TrackSectionParams bestParams;

    // individual clothoids excluding the curves and straights
    ClothoidT uncurvedT1[2]; // from start to first curve
    ClothoidT uncurvedT2[2]; // from first curve to straight
    ClothoidT uncurvedT3[2][2]; // from straight to second curve
    ClothoidT uncurvedT4rev[2]; // from end to second curve
    ClothoidT uncurvedT3rev[2]; // from second curve to straight
    ClothoidT uncurvedT2rev[2][2]; // from straight to first curve

    // Indexed by d1
    TransitionParams simpleT1[2], simpleT2[2];
    // Indexed by d2
    TransitionParams simpleT3[2], simpleT4[2];
    float transition1DirectionChange[2];
    float transition2DirectionChange[2];
    float transition3DirectionChange[2];
    float transition4DirectionChange[2];
    Vec2f offset = (Vec2f)nodes[1].getPosition() - (Vec2f)nodes[0].getPosition();
    for (int d1 = 0; d1 < 2; ++d1) {
        float direction = d1 ? 1.0f : -1.0f;
        simpleT1[d1].rate = direction * curvatureRate;
        simpleT1[d1].length = (direction * minCurvature1 - startCurvature) / simpleT1[d1].rate;
        simpleT2[d1].rate = -simpleT1[d1].rate;
        simpleT2[d1].length = minCurvature1 / curvatureRate;
        simpleT3[d1].rate = direction * curvatureRate;
        simpleT3[d1].length = minCurvature2 / curvatureRate;
        simpleT4[d1].rate = -simpleT3[d1].rate;
        simpleT4[d1].length = (-endCurvatureRev - direction * minCurvature2) / simpleT4[d1].rate;

        uncurvedT1[d1].setStartDirection(nodes[0].getDirection());
        uncurvedT1[d1].setStartCurvature(startCurvature);
        uncurvedT1[d1].setCurvatureRate(simpleT1[d1].rate);
        uncurvedT1[d1].setLength(simpleT1[d1].length);
        transition1DirectionChange[d1] = uncurvedT1[d1].getEndDirectionChange();
        // exclude direction change to straighten out
        float distToStraight = uncurvedT1[d1].lengthAtCurvature(0);
        if (distToStraight > 0 && distToStraight < simpleT1[d1].length)
            transition1DirectionChange[d1] -= uncurvedT1[d1].directionChangeAtLength(distToStraight);
        uncurvedT1[d1] = uncurvedT1[d1].getNextClothoid();

        uncurvedT2[d1].setStartDirection(uncurvedT1[d1].getStartDirection());
        uncurvedT2[d1].setStartCurvature(uncurvedT1[d1].getStartCurvature());
        uncurvedT2[d1].setCurvatureRate(-simpleT1[d1].rate);
        uncurvedT2[d1].setLength(simpleT2[d1].length);
        transition2DirectionChange[d1] = uncurvedT2[d1].getEndDirectionChange();
        uncurvedT2[d1] = uncurvedT2[d1].getNextClothoid();

        uncurvedT4rev[d1].setStartDirection(endDirectionRev);
        uncurvedT4rev[d1].setStartCurvature(endCurvatureRev);
        uncurvedT4rev[d1].setCurvatureRate(simpleT4[d1].rate);
        uncurvedT4rev[d1].setLength(simpleT4[d1].length);
        transition4DirectionChange[d1] = -uncurvedT4rev[d1].getEndDirectionChange();
        // exclude direction change to straighten out
        distToStraight = uncurvedT4rev[d1].lengthAtCurvature(0);
        if (distToStraight > 0 && distToStraight < simpleT4[d1].length)
            transition4DirectionChange[d1] += uncurvedT4rev[d1].directionChangeAtLength(distToStraight);
        uncurvedT4rev[d1] = uncurvedT4rev[d1].getNextClothoid();

        uncurvedT3rev[d1].setStartDirection(uncurvedT4rev[d1].getStartDirection());
        uncurvedT3rev[d1].setStartCurvature(uncurvedT4rev[d1].getStartCurvature());
        uncurvedT3rev[d1].setCurvatureRate(-simpleT4[d1].rate);
        uncurvedT3rev[d1].setLength(simpleT3[d1].length);
        transition3DirectionChange[d1] = -uncurvedT3rev[d1].getEndDirectionChange();
        uncurvedT3rev[d1] = uncurvedT3rev[d1].getNextClothoid();

        for (int d2 = 0; d2 < 2; ++d2) {
            float direction2 = d2 ? 1.0f : -1.0f;
            uncurvedT3[d1][d2].setStartDirection(uncurvedT2[d1].getStartDirection());
            uncurvedT3[d1][d2].setCurvatureRate(direction2 * curvatureRate);
            uncurvedT3[d1][d2].setLength(simpleT3[d1].length);
            uncurvedT3[d1][d2] = uncurvedT3[d1][d2].getNextClothoid();

            uncurvedT2rev[d1][d2].setStartDirection(uncurvedT3rev[d1].getStartDirection());
            uncurvedT2rev[d1][d2].setCurvatureRate(-direction2 * curvatureRate);
            uncurvedT2rev[d1][d2].setLength(simpleT2[d1].length);
            uncurvedT2rev[d1][d2] = uncurvedT2rev[d1][d2].getNextClothoid();
        }
    }

    float bestLength = -1;
    for (int d1 = d1start; d1 < d1end; ++d1) {
        float dir1 = d1 ? 1.0f : -1.0f;
        for (int d2 = d2start; d2 < d2end; ++d2) {
            float dir2 = d2 ? 1.0f : -1.0f;
            TrackSectionParams params;
            params.t1 = simpleT1[d1];
            params.t2 = simpleT2[d1];
            params.t3 = simpleT3[d2];
            params.t4 = simpleT4[d2];
            // Find vector between curve circles
            Vec2f thisOffset = offset - uncurvedT1[d1].circleAtStart()
                                    + uncurvedT4rev[d2].circleAtStart();
            // vector between centers of curve circles when straight = 0
            Vec2f lineStart = (uncurvedT2[d1].getStartPosition() - (uncurvedT1[d1].circleAtStart() - uncurvedT1[d1].getStartPosition())) +
                                uncurvedT3[d1][d2].circleAtStart();
            Vec2f lineNorm2;
            sincosf(uncurvedT2[d1].getStartDirection(), &lineNorm2[1], &lineNorm2[0]);
            Vec2f lineNorm3;
            sincosf(uncurvedT3rev[d2].getStartDirection(), &lineNorm3[1], &lineNorm3[0]);
            // x = lineStart[0] + s * lineNorm[0]
            // y = lineStart[1] + s * lineNorm[1]
            // sqr = x² + y²
            // sqr = (lineStart[0] + s * lineNorm[0])² + (lineStart[1] + s * lineNorm[1])²
            // sqr = lineStart[0]² + 2*lineStart[0]*lineNorm[0]*s + lineNorm[0]²*s²
            //     + lineStart[1]² + 2*lineStart[1]*lineNorm[1]*s + lineNorm[1]²*s²
            // sqr = lineStart² + 2*(lineStart*lineNorm)*s + s²
            // a = 1
            // b = 2*(lineStart*lineNorm)
            // c = lineStart² - sqr
            // s = (-b +- sqrt(b²-4ac)) / 2a
            // s = (-b +- sqrt(b²-4c)) / 2
            // s = (-2*(lineStart*lineNorm) +- sqrt(4*(lineStart*lineNorm)²-4c)) / 2
            // s = -(lineStart*lineNorm) +- sqrt((lineStart*lineNorm)²-c)
            // Curve is angle between straight vector and point of intersection
            float b = lineStart * lineNorm2;
            float c = lineStart.sqr() - thisOffset.sqr();
            float b2m4ac = b*b - c;
            if (b2m4ac >= 0) {
                params.s.length = -b + sqrt(b2m4ac);
                if (params.s.length < 0) {
                    // Can't shorten the transition curves if swapping direction
                    if (d1 != d2)
                        continue;
                    // TODO reduce t2 & t3 to allow a tighter turn
                    continue;
                }
                // Vector from midpoints of circles when curve=0
                Vec2f transition2Vec = uncurvedT1[d1].getStartPosition() + uncurvedT2[d1].getStartPosition() - uncurvedT1[d1].circleAtStart();
                Vec2f transition3Vec = uncurvedT3[d1][d2].circleAtStart();
                Vec2f transition3VecRev = uncurvedT4rev[d2].getStartPosition() + uncurvedT3rev[d2].getStartPosition() - uncurvedT4rev[d2].circleAtStart();
                Vec2f transition2VecRev = uncurvedT2rev[d2][d1].circleAtStart();
                Vec2f midpoint2Vec = transition2Vec + transition3Vec + lineNorm2 * params.s.length;
                Vec2f midpoint3Vec = -(transition3VecRev + transition2VecRev + lineNorm3 * params.s.length);
                float angle1 = atan2f(midpoint2Vec[1], midpoint2Vec[0]);
                float angle2 = atan2f(thisOffset[1], thisOffset[0]);
                float angle3 = atan2f(midpoint3Vec[1], midpoint3Vec[0]);
                float curve1 = dir1 * (angle2 - angle1);
                while (curve1 < 0)
                    curve1 += M_PI*2;
                while (curve1 >= M_PI*2)
                    curve1 -= M_PI*2;
                params.c1.length = curve1 / minCurvature1;
                float curve2 = dir2 * (angle3 - angle2);
                while (curve2 < 0)
                    curve2 += M_PI*2;
                while (curve2 >= M_PI*2)
                    curve2 -= M_PI*2;
                params.c2.length = curve2 / minCurvature2;

                float curve1DirectionChange = fabs(transition1DirectionChange[d1] + dir1*curve1 + transition2DirectionChange[d1]);
                float curve2DirectionChange = fabs(transition3DirectionChange[d2] + dir2*curve2 + transition4DirectionChange[d2]);

                if (unloop1 && curve1DirectionChange > M_PI*2) {
                    ClothoidT curve1 = uncurvedT1[d1];
                    curve1.setLength(params.c1.length);
                    ClothoidT curvedT2 = curve1.getNextClothoid();
                    curvedT2.setLength(params.t2.length);
                    curvedT2.setCurvatureRate(params.t2.rate);
                    ClothoidT curvedS = curvedT2.getNextClothoid();

                    float rate1, len1, rate2, len2, extraStraight;
                    Vec2f positionB;
                    if (TrackSection::interpolateClothoidPair(startDirection, startCurvature,
                                                            curvedS.getStartPosition(),
                                                            curvedS.getStartDirection(), 0,
                                                            &rate1, &len1, &rate2, &len2,
                                                            &positionB, &extraStraight)) {
                        if (extraStraight > -params.s.length && fabs(rate1) < maxCurvature) {
                            params.t1.rate = rate1;
                            params.t1.length = len1;
                            params.c1.length = 0;
                            params.t2.rate = rate2;
                            params.t2.length = len2;
                            params.s.length += extraStraight;
                        }
                    }
                }
                if (unloop2 && curve2DirectionChange > M_PI*2) {
                    ClothoidT curve2 = uncurvedT4rev[d2];
                    curve2.setLength(params.c2.length);
                    ClothoidT curvedT3 = curve2.getNextClothoid();
                    curvedT3.setLength(params.t3.length);
                    curvedT3.setCurvatureRate(params.t3.rate);
                    ClothoidT curvedS = curvedT3.getNextClothoid();

                    float rate1, len1, rate2, len2, extraStraight;
                    Vec2f positionB;
                    if (TrackSection::interpolateClothoidPair(endDirectionRev, endCurvatureRev,
                                                            curvedS.getStartPosition(),
                                                            curvedS.getStartDirection(), 0,
                                                            &rate1, &len1, &rate2, &len2,
                                                            &positionB, &extraStraight)) {
                        if (extraStraight > -params.s.length && fabs(rate1) < maxCurvature) {
                            params.t4.rate = rate1;
                            params.t4.length = len1;
                            params.c2.length = 0;
                            params.t3.rate = rate2;
                            params.t3.length = len2;
                            params.s.length += extraStraight;
                        }
                    }
                }

                float length = params.t1.length
                             + params.c1.length
                             + params.t2.length
                             + params.s.length
                             + params.t3.length
                             + params.c2.length
                             + params.t4.length;
                if (bestLength < 0 || length < bestLength) {
                    bestLength = length;

                    direction1 = dir1;
                    direction2 = dir2;
                    bestParams = params;
                }
            }
        }
    }

    /* Other routes to attempt for comparison:
     * 2 clothoids
     * if d1==d2, negative straight, use double clothoid calculation to replace T2 and T3
     */

    chain.clear();

    ClothoidT *transition1 = chain.append();
    transition1->setStartPosition((ClothoidT::Vec2l)nodes[0].getPosition());
    transition1->setStartDirection(startDirection);
    transition1->setStartCurvature(startCurvature);

    transition1->setCurvatureRate(bestParams.t1.rate);
    transition1->setLength(bestParams.t1.length);

    if (bestParams.c1.length > 0) {
        ClothoidT *curve1 = chain.append();
        curve1->setLength(bestParams.c1.length);
    }

    if (bestParams.t2.length > 0) {
        ClothoidT *transition2 = chain.append();
        transition2->setCurvatureRate(bestParams.t2.rate);
        transition2->setLength(bestParams.t2.length);
    }

    if (bestParams.s.length > 0) {
        ClothoidT *straight = chain.append();
        straight->setLength(bestParams.s.length);
    }

    if (bestParams.t3.length > 0) {
        ClothoidT *transition3 = chain.append();
        transition3->setCurvatureRate(bestParams.t3.rate);
        transition3->setLength(bestParams.t3.length);
    }

    if (bestParams.c2.length > 0) {
        ClothoidT *curve2 = chain.append();
        curve2->setLength(bestParams.c2.length);
    }

    if (bestParams.t4.length > 0) {
        ClothoidT *transition4 = chain.append();
        transition4->setCurvatureRate(bestParams.t4.rate);
        transition4->setLength(bestParams.t4.length);
    }

    ClothoidT *end = chain.append();
}

void TrackSection::notifyNodeChanged(TrackNode *node)
{
    interpolate();
}
