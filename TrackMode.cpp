#include "TrackMode.h"
#include "TrackNode.h"
#include "TrackSection.h"
#include "TrackPosition.h"
#include "TrackSpec.h"
#include "Railway.h"
#include "Renderer.h"

#include "TrainWheelset.h"
#include "TrainBogie.h"
#include "TrainUnit.h"
#include "Train.h"

#include <cmath>

TrackMode::TrackMode(Railway *newRailway, const TrackSpec *newMinSpec)
: railway(newRailway),
  minSpec(newMinSpec),
  hoverNode(nullptr),
  selectedNode(nullptr),
  testTrain(false),
  dragMode(NONE),
  mouseMoved(true)
{
    mouseRay.start.set(0);
    mouseRay.norm.set(0);
}

void TrackMode::mouseMove(const LineNormal3f &ray)
{
    float range = renderer->getViewpoint().viewportToWorldSize(ray.startPosition(), 0.02);
    TrackNode *closestNode = railway->findClosestNode(ray, range);
    if (hoverNode != closestNode) {
        hoverNode = closestNode;
        renderer->setRedraw();
    }

    if (selectedNode && !mouseRay.norm.zero()) {
        switch (dragMode) {
            case MOVE: {
                Vec3f diff = ray.start - mouseRay.start;
                selectedNode->setMidpoint(selectedNode->getMidpoint() + diff);
                updateHandles();
                break;
            }
            case ROTATE: {
                Vec3f nodeToMouse = ray.start - selectedNode->getPosition();
                if (nodeToMouse) {
                    float direction = atan2f(nodeToMouse[1], nodeToMouse[0]);
                    direction -= M_PI/2 * dragIndex;
                    selectedNode->setDirection(direction);
                    updateHandles();
                }
                break;
            }
            case RECURVE: {
                Vec2f nodeToMouse(ray.start - selectedNode->getPosition());
                if (nodeToMouse) {
                    float direction = selectedNode->getDirection();
                    Vec2f forwardDirection;
                    sincosf(direction, &forwardDirection[1], &forwardDirection[0]);
                    Vec2f leftDirection(-forwardDirection[1], forwardDirection[0]);
                    Vec2f xy(nodeToMouse * forwardDirection, nodeToMouse * leftDirection);
                    float xySqr = xy.sqr();
                    if (xySqr) {
                        float curvature = xy[1] * 2 / xySqr;
                        if (-minSpec->getMaxCurvature() < curvature && curvature < minSpec->getMaxCurvature()) {
                            selectedNode->setCurvature(curvature);
                            updateHandles();
                        }
                    }
                }

                break;
            }
            default:
                break;
        }
    }

    mouseRay = ray;
    mouseMoved = true;
}

void TrackMode::mouseLeave()
{
    mouseRay.norm.set(0);
}

void TrackMode::mouseDown(const LineNormal3f &ray, int button, int clicks)
{
    // Left button
    if (button == 0) {
        if (mouseMoved)
            clicks = 1;
        if (clicks == 1) {
            mouseMoved = false;

            // Single left click
            enum DragMode mode = NONE;
            int index = 0;
            if (selectedNode) {
                // First check if we're clicking on a handle
                float size = renderer->getViewpoint().viewportToWorldSize(selectedNode->getPosition(), 0.01);
                const Handle *handle = handleUnderMouse(ray, size);
                if (handle) {
                    mode = handle->mode;
                    index = handle->index;
                    if (mode == POINTS)
                        selectedNode->switchPoints(index / 2, index & 1);
                }
            }

            bool changed = false;
            if (mode == NONE) {
                // Select the node under the mouse
                changed = (hoverNode != selectedNode);
                selectedNode = hoverNode;
                if (selectedNode)
                    dragMode = MOVE;
            }
            if (mode != NONE) {
                dragMode = mode;
                dragIndex = index;
                changed = true;
            }
            if (changed)
                updateHandles();
        } else if (clicks == 2) {
            // Double left click: Start a new track section
            constexpr unsigned int numTracks = 1;

            if (!selectedNode) {
                TrackNode *startNode = new TrackNode(minSpec);
                startNode->setNumTracks(numTracks);
                startNode->setMidpoint((Vec3f)(Vec2f)ray.start);
                railway->addNode(startNode);

                selectedNode = startNode;
            }

            TrackNode *endNode = new TrackNode(minSpec);
            endNode->setNumTracks(numTracks);
            endNode->setMidpoint((Vec3f)(Vec2f)ray.start);
            endNode->setDirection(selectedNode->getDirection());
            railway->addNode(endNode);

            TrackSection *section = new TrackSection(selectedNode->forward(),
                                                     endNode->backward(),
                                                     minSpec);
            railway->addSection(section);
            if (!testTrain) {
                for (int i = 0; i < numTracks; ++i) {
                    // Create a test train
                    Train *train = new Train();

                    TrainBogie *locoChassis = new TrainBogie();
                    locoChassis->initWheelsets(3, minSpec->getTrackGauge(), 1.0f, 5.0f);

                    TrainBogie *rear = new TrainBogie();
                    rear->initWheelsets(2, minSpec->getTrackGauge(), 0.45f, 1.5f);
                    locoChassis->addBogie(rear, Vec3f(-5.0f, 0, 0));

                    TrainBogie *locoArtic = new TrainBogie();
                    locoArtic->initWheelsets(3, minSpec->getTrackGauge(), 1.0f, 5.0f);

                    TrainBogie *front = new TrainBogie();
                    front->initWheelsets(2, minSpec->getTrackGauge(), 0.45f, 1.5f);
                    locoArtic->addBogie(front, Vec3f(5.0f, 0, 0));

                    locoChassis->addBogie(locoArtic, Vec3f(8.0f, 0, 0));

                    TrainUnit *loco = new TrainUnit();
                    loco->setChassis(locoChassis);

                    train->addUnitBack(loco);

                    TrainBogie *tenderChassis = new TrainBogie();
                    tenderChassis->initWheelsets(5, minSpec->getTrackGauge(), 0.45f, 5.0f);

                    TrainUnit *tender = new TrainUnit();
                    tender->setChassis(tenderChassis);

                    train->addUnitBack(tender);

                    for (int u = 0; u < 10; ++u) {
                        TrainBogie *bogie1 = new TrainBogie();
                        bogie1->initWheelsets(2, minSpec->getTrackGauge(), 0.45f, 2.0f);

                        TrainBogie *bogie2 = new TrainBogie();
                        bogie2->initWheelsets(2, minSpec->getTrackGauge(), 0.45f, 2.0f);

                        TrainBogie *chassis = new TrainBogie();
                        chassis->addBogie(bogie1, Vec3f(7.0f, 0, 0));
                        chassis->addBogie(bogie2, Vec3f(-7.0f, 0, 0));
                        chassis->setExtent(10.0f, 10.0f);

                        TrainUnit *unit = new TrainUnit();
                        unit->setChassis(chassis);

                        train->addUnitBack(unit);
                    }

                    TrackPosition pos;
                    pos.set(section, true, i);
                    train->reposition(pos);

                    railway->addTrain(train);
                }
            }

            selectedNode = endNode;
            dragMode = MOVE;
            updateHandles();
        }
    } else if (button == 1) {
        // Right button
        if (dragMode == RECURVE) {
            // Reset curvature
            selectedNode->setCurvature(0);
            dragMode = NONE;
            updateHandles();
        }
    }
}

void TrackMode::mouseUp(const LineNormal3f &ray, int button, int clicks)
{
    // Left button
    if (button == 0) {
        // Finish dragging
        dragMode = NONE;
        updateHandles();
    }
}

void TrackMode::viewportChanged()
{
    if (selectedNode)
        updateHandles();
}

void TrackMode::updateHandles()
{
    if (selectedNode) {
        Vec3f position = selectedNode->getPosition();
        Vec3f position1 = selectedNode->getPosition(0);
        Vec3f position2 = selectedNode->getPosition(selectedNode->getNumTracks() - 1);
        float direction = selectedNode->getDirection();
        float size = renderer->getViewpoint().viewportToWorldSize(position, 0.02);

        Vec3f directionVector;
        sincosf(direction + M_PI/2, &directionVector[1], &directionVector[0]);
        directionVector[2] = 0;

        Clothoid<float, float> clothoid;
        clothoid.setStartPosition((Vec2f)position);
        clothoid.setStartDirection(direction);
        clothoid.setStartCurvature(selectedNode->getCurvature());

        handles.resize(4 + selectedNode->getNumTracks() * 2);
        handles[0].mode = RECURVE;
        handles[0].position = (clothoid.positionAtLength(size * 6), position[2]);
        handles[0].index = 0;
        handles[0].enabled = true;
        handles[1].mode = RECURVE;
        handles[1].position = (clothoid.positionAtLength(size * -6), position[2]);
        handles[1].index = 0;
        handles[1].enabled = true;
        handles[2].mode = ROTATE;
        handles[2].position = position1 + 3 * size * directionVector;
        handles[2].index = 1;
        handles[2].enabled = true;
        handles[3].mode = ROTATE;
        handles[3].position = position2 - 3 * size * directionVector;
        handles[3].index = -1;
        handles[3].enabled = true;

        // Draw arrow showing point directions
        for (int d = 0; d < 2; ++d) {
            for (int i = 0; i < selectedNode->getNumTracks(); ++i) {
                handles[4 + i*2 + d].mode = POINTS;
                handles[4 + i*2 + d].index = i*2 + d;
                if (selectedNode->hasPoints(i, (bool)d)) {
                    TrackPosition pos;
                    pos.set(selectedNode, true, i);
                    if (pos) {
                        pos += size * 10 * (d*2 - 1);
                        if (pos)
                            handles[4 + i*2 + d].position = pos.getPosition();
                    }
                    handles[4 + i*2 + d].enabled = (bool)pos && (handles[4 + i*2 + d].position - selectedNode->getPosition(i)).sqr() > size*size;
                } else {
                    handles[4 + i*2 + d].enabled = false;
                }
            }
        }

        // If dragging, just show handles in use
        for (Handle &handle: handles)
            handle.enabled = handle.enabled &&
                             (dragMode == NONE ||
                              (dragMode == handle.mode && dragIndex == handle.index));
    } else {
        handles.resize(0);
    }
    renderer->setRedraw();
}

const TrackMode::Handle *TrackMode::handleUnderMouse(const LineNormal3f &ray, float size) const
{
    for (const Handle &handle: handles) {
        if (handle.enabled) {
            Vec3f closest = ray * ray.closestPoint(handle.position);
            float distanceSqr = (closest - handle.position).sqr();
            if (distanceSqr < size*size) {
                return &handle;
            }
        }
    }
    return nullptr;
}
