#include "TrackMode.h"
#include "Renderer.h"

#include <cmath>

TrackMode::TrackMode(Railway *newRailway)
: railway(newRailway),
  step(0),
  start(0.0f),
  end(0.0f),
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

    switch (step) {
    case 1:
        end = ray.start;
        renderer->setRedraw();
        break;
    case 2:
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
    }

    mouseRay = ray;
}

void TrackMode::mouseLeave()
{
    mouseRay.norm.set(0);

    switch (step) {
    case 1:
        end = start;
        renderer->setRedraw();
        break;
    }
}

void TrackMode::mouseDown(const LineNormal3f &ray, int button, int clicks)
{
    // Left button
    if (button == 0) {
        switch (step) {
        case 0:
            step = 1;
            start = ray.start;
            end = start;
            renderer->setRedraw();
            break;
        case 1: {
            if (start == ray.start)
                break;

            step = 2;
            end = ray.start;

            Vec2f vec(end - start);
            float direction = atan2f(vec[1], vec[0]);

            TrackNode *startNode = new TrackNode();
            startNode->setPosition(start);
            startNode->setDirection(direction);
            railway->addNode(startNode);

            TrackNode *endNode = new TrackNode();
            endNode->setPosition(end);
            endNode->setDirection(direction);
            railway->addNode(endNode);

            TrackSection *section = new TrackSection(startNode->forward(),
                                                     endNode->backward());
            railway->addSection(section);

            renderer->setRedraw();
            break;
        }
        case 2:
            if (selectedNode != hoverNode)
                renderer->setRedraw();
            selectedNode = hoverNode;
            if (selectedNode)
                dragMode = MOVE;
            break;
        }
    }
}

void TrackMode::mouseUp(const LineNormal3f &ray, int button, int clicks)
{
    // Left button
    if (button == 0) {
        switch (step) {
        case 2:
            // Finish dragging
            dragMode = NONE;
            break;
        }
    }
}
