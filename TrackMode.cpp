#include "TrackMode.h"
#include "TrackNode.h"
#include "TrackSection.h"
#include "Railway.h"
#include "Renderer.h"

#include <cmath>

TrackMode::TrackMode(Railway *newRailway, const TrackSpec *newMinSpec)
: railway(newRailway),
  minSpec(newMinSpec),
  hoverNode(nullptr),
  selectedNode(nullptr),
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
                        if (-1.0 / 20.0 < curvature && curvature < 1.0 / 20.0) {
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

            if (!selectedNode) {
                TrackNode *startNode = new TrackNode(minSpec);
                startNode->setNumTracks(4);
                startNode->setMidpoint((Vec3f)(Vec2f)ray.start);
                railway->addNode(startNode);

                selectedNode = startNode;
            }

            TrackNode *endNode = new TrackNode(minSpec);
            endNode->setNumTracks(4);
            endNode->setMidpoint((Vec3f)(Vec2f)ray.start);
            endNode->setDirection(selectedNode->getDirection());
            railway->addNode(endNode);

            TrackSection *section = new TrackSection(selectedNode->forward(),
                                                     endNode->backward());
            railway->addSection(section);

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

        handles[0].mode = RECURVE;
        handles[0].position = (clothoid.positionAtLength(size * 6), position[2]);
        handles[0].index = 0;
        handles[1].mode = RECURVE;
        handles[1].position = (clothoid.positionAtLength(size * -6), position[2]);
        handles[1].index = 0;
        handles[2].mode = ROTATE;
        handles[2].position = position1 + 3 * size * directionVector;
        handles[2].index = 1;
        handles[3].mode = ROTATE;
        handles[3].position = position2 - 3 * size * directionVector;
        handles[3].index = -1;

        // If dragging, just show handles in use
        for (Handle &handle: handles)
            handle.enabled = (dragMode == NONE ||
                              (dragMode == handle.mode && dragIndex == handle.index));
    } else {
        for (Handle &handle: handles)
            handle.enabled = false;
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
