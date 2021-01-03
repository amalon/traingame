#include "TrackMode.h"
#include "Renderer.h"

#include <cmath>

TrackMode::TrackMode(Railway *newRailway)
: railway(newRailway),
  hoverNode(nullptr),
  selectedNode(nullptr),
  dragMode(NONE)
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

    switch (dragMode) {
    case MOVE:
        if (!mouseRay.norm.zero()) {
            Vec3f diff = ray.start - mouseRay.start;
            selectedNode->setPosition(selectedNode->getPosition() + diff);
            renderer->setRedraw();
        }
        break;
    default:
        break;
    }

    mouseRay = ray;
}

void TrackMode::mouseLeave()
{
    mouseRay.norm.set(0);
}

void TrackMode::mouseDown(const LineNormal3f &ray, int button, int clicks)
{
    // Left button
    if (button == 0) {
        if (clicks == 1) {
            // Single left click: Select the node under the mouse
            if (selectedNode != hoverNode)
                renderer->setRedraw();
            selectedNode = hoverNode;
            if (selectedNode)
                dragMode = MOVE;
        } else if (clicks == 2) {
            // Double left click: Start a new track section

            if (!selectedNode) {
                TrackNode *startNode = new TrackNode();
                startNode->setPosition((Vec3f)(Vec2f)ray.start);
                railway->addNode(startNode);

                selectedNode = startNode;
            }

            TrackNode *endNode = new TrackNode();
            endNode->setPosition(ray.start);
            endNode->setDirection(selectedNode->getDirection());
            railway->addNode(endNode);

            TrackSection *section = new TrackSection(selectedNode->forward(),
                                                     endNode->backward());
            railway->addSection(section);

            selectedNode = endNode;
            dragMode = MOVE;

            renderer->setRedraw();
        }
    }
}

void TrackMode::mouseUp(const LineNormal3f &ray, int button, int clicks)
{
    // Left button
    if (button == 0) {
        // Finish dragging
        dragMode = NONE;
    }
}
