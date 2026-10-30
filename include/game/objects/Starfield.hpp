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

// ------------------------------------------------------------------
// Starfield
// ------------------------------------------------------------------
// A self-contained scrolling starfield node.
//
// Stars are randomly scattered across [0,width] x [0,height] and
// scroll in the given direction, wrapping around when they leave the
// screen. Each star has its own speed (in the minSpeed..maxSpeed
// range) and size/opacity, giving a parallax-depth feel.
//
// Parameters
//   width, height   : size of the area covered by the starfield
//   amount          : total number of stars
//   direction       : scroll direction (default Right)
//   minSpeed        : slowest star speed in pixels/sec (default 10)
//   maxSpeed        : fastest star speed in pixels/sec (default 60)
//   startStill      : if true, all stars spawn with speed = 0.
//                     Use StarSpeedTo to ease into motion later.
// ------------------------------------------------------------------
class Starfield : public engine::Node {

    struct Star {
        float x, y;            // current position
        float size;            // draw size (1–3 pixels)
        float opacity;         // 0..1 (dim = far, bright = close)
        float speedFactor;     // 0..1, fraction of current m_speed
        u32   baseColor;       // RGBA8 white at varying brightness
    };

    float          m_width    = 0.0f;
    float          m_height   = 0.0f;
    StarDirection  m_direction = StarDirection::Right;
    float          m_speed    = 0.0f;   // current pixels/sec for fastest star
    float          m_minSpeed = 10.0f;
    float          m_maxSpeed = 60.0f;
    std::vector<Star> m_stars;

    // Simple LCG so we don't pull in <random> overhead on 3DS
    uint32_t m_rng = 12345u;
    float nextRand();        // returns 0..1
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

    // Set scroll speed directly (pixels/sec for the fastest star).
    void setSpeed(float speed) { m_speed = speed; }
    float getSpeed() const     { return m_speed; }
    float getMinSpeed() const  { return m_minSpeed; }
    float getMaxSpeed() const  { return m_maxSpeed; }

    virtual void update(float dt) override;
    virtual void draw(float px, float py, float sx, float sy, float rot) override;
};

// ------------------------------------------------------------------
// StarSpeedTo  (FiniteTimeAction)
// ------------------------------------------------------------------
// Smoothly transitions a Starfield's speed from its current value
// to a target value over `duration` seconds, with optional easing.
//
// Usage:
//   starfield->runAction(StarSpeedTo::create(2.0f, 80.f, Ease::SineInOut));
// ------------------------------------------------------------------
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

} // namespace game