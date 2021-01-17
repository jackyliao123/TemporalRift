#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

struct AABBCollisionResult {
    bool valid;
    float collisionTime;        // between 0 and 1
    float intersectionDistance; // distance between the 2 intersection surfaces
};

enum CollideFaceType {
    NEGX, NEGY,
    POSX, POSY,
};

struct AABBCollisionFace {
    CollideFaceType faceType;
    float a1, b1, y1;
    float a2, b2, y2;
    GameEngine::CollidableComponent *component;
    sf::Vector2f otherVelocity;

    float getIntersectionDistance() {
        float dist = y2 - y1;
        if(dist < 0) {
            return -dist;
        }
        return dist;
    }
};

struct AABBUtil {
    typedef sf::Rect<float> AABBRect;
    static AABBRect expandByVector(AABBRect rect, sf::Vector2f v) {
        rect.width += v.x < 0 ? -v.x : v.x;
        rect.height += v.y < 0 ? -v.y : v.y;
        if(v.x < 0) {
            rect.left += v.x;
        }
        if(v.y < 0) {
            rect.top += v.y;
        }
        return rect;
    }
    static AABBRect expandByAmount(AABBRect rect, float amount) {
        rect.width += amount * 2;
        rect.height += amount * 2;
        rect.left -= amount;
        rect.top -= amount;
        return rect;
    }
    static AABBCollisionResult ComputeSideSweepResult(const AABBCollisionFace &face, sf::Vector2f vel) {
        AABBCollisionResult result = {};

        if(face.faceType == POSX || face.faceType == NEGX) {
            float temp = vel.x;
            vel.x = vel.y;
            vel.y = temp;
        }

        // Compute time of collision
        float collisionTime = -(face.y1 - face.y2) / vel.y;

        // Check to see if time is valid
        if(!(0 <= collisionTime && collisionTime <= 1)) {
            return result;
        }

        float ax = face.a1 + vel.x * collisionTime;
        float bx = face.b1 + vel.x * collisionTime;

        // Check if collision actually happened
        if(face.a2 <= bx && ax <= face.b2) {
            result.valid = true;
            result.collisionTime = collisionTime;
        }
        return result;
    }
};