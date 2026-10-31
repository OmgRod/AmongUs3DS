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
private:
    float         m_width     = 0.0f;
    float         m_height    = 0.0f;
    StarDirection m_direction = StarDirection::Down;
    float         m_speed     = 60.0f;

    struct Star {
        float x;
        float y;
        float size;
        float speedMultiplier;
        u8 brightness;
    };

    std::vector<Star> m_stars;

    void init(float width, float height, StarDirection direction, float speed);

public:
    Starfield() = default;
    virtual ~Starfield() = default;

    static std::shared_ptr<Starfield> create(
        float width,
        float height,
        StarDirection direction = StarDirection::Down,
        float speed = 60.f
    );

    void setSpeed(float speed) { m_speed = speed; }
    float getSpeed() const     { return m_speed; }

    virtual void update(float dt) override;
    virtual void draw(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation) override;
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
