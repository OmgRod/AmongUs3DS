#pragma once

#include <includes.hpp>
#include <engine/actions.hpp>

namespace game {

enum class StarDirection {
    Up,
    Down,
    Left,
    Right
};

class Starfield : public engine::Node {

    struct Star {
        float x, y;
        float size;
        float opacity;
        float speedFactor;
        u32   baseColor;
    };

    float m_width    = 0.0f;
    float m_height   = 0.0f;
    StarDirection m_direction = StarDirection::Right;
    float m_speed    = 0.0f;
    float m_minSpeed = 10.0f;
    float m_maxSpeed = 60.0f;
    std::vector<Star> m_stars;

    uint32_t m_rng = 12345u;
    float nextRand();
    float nextRandRange(float lo, float hi);

    void  initStar(Star& s, bool randomX);
    void  drawStarAt(float x, float y, float sz, u32 color) const;

    void init(
        float width,
        float height,
        int   amount    = 60,
        StarDirection direction = StarDirection::Right,
        float minSpeed  = 10.f,
        float maxSpeed  = 60.f,
        bool  startStill = false
    );

public:
    Starfield() = default;
    virtual ~Starfield() = default;

    static std::shared_ptr<Starfield> create(
        float width,
        float height,
        int   amount    = 60,
        StarDirection direction = StarDirection::Right,
        float minSpeed  = 10.f,
        float maxSpeed  = 60.f,
        bool  startStill = false
    );

    void setSpeed(float speed) { m_speed = speed; }
    float getSpeed() const     { return m_speed; }
    float getMinSpeed() const  { return m_minSpeed; }
    float getMaxSpeed() const  { return m_maxSpeed; }

    virtual void update(float dt) override;
    virtual void draw(float px, float py, float sx, float sy, float rot) override;
};

class StarSpeedTo : public engine::FiniteTimeAction {
    float m_startSpeed = 0.0f;
    float m_endSpeed   = 0.0f;
    engine::Ease::EaseFunc m_ease;

public:
    StarSpeedTo(float duration, float targetSpeed,
                engine::Ease::EaseFunc ease = engine::Ease::Linear);

    static std::shared_ptr<StarSpeedTo> create(
        float duration,
        float targetSpeed,
        engine::Ease::EaseFunc ease = engine::Ease::Linear
    );

    void startWithTarget(engine::Node* target) override;
    void step(float dt) override;
};

}