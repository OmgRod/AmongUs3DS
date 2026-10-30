#include <game/objects/Starfield.hpp>
#include <cstring>
#include <cmath>

namespace game {

float Starfield::nextRand() {
    m_rng = m_rng * 1664525u + 1013904223u;
    return static_cast<float>(m_rng & 0x7FFFFFFFu) / static_cast<float>(0x7FFFFFFFu);
}

float Starfield::nextRandRange(float lo, float hi) {
    return lo + nextRand() * (hi - lo);
}

void Starfield::initStar(Star& s, bool randomX) {
    if (randomX) {
        s.x = nextRandRange(0.0f, m_width);
    } else {
        switch (m_direction) {
            case StarDirection::Right: s.x = -2.0f;          s.y = nextRandRange(0.0f, m_height); break;
            case StarDirection::Left:  s.x = m_width  + 2.0f; s.y = nextRandRange(0.0f, m_height); break;
            case StarDirection::Down:  s.y = -2.0f;          s.x = nextRandRange(0.0f, m_width);  break;
            case StarDirection::Up:    s.y = m_height + 2.0f; s.x = nextRandRange(0.0f, m_width);  break;
        }
    }
    if (randomX) {
        s.y = nextRandRange(0.0f, m_height);
    }

    s.speedFactor = nextRandRange(0.15f, 1.0f);

    float depthBias = s.speedFactor * s.speedFactor;
    s.size = 1.0f + depthBias * 2.0f;

    s.opacity = 0.25f + depthBias * 0.75f;

    u8 bright = static_cast<u8>(s.opacity * 255.0f);
    s.baseColor = C2D_Color32(bright, bright, bright, 255);
}

void Starfield::drawStarAt(float x, float y, float sz, u32 color) const {
    float half = sz * 0.5f;
    C2D_DrawRectSolid(x - half, y - half, 0.5f, sz, sz, color);
}

void Starfield::init(float width, float height, int amount,
                     StarDirection direction,
                     float minSpeed, float maxSpeed,
                     bool startStill) {
    m_width     = width;
    m_height    = height;
    m_direction = direction;
    m_minSpeed  = minSpeed;
    m_maxSpeed  = maxSpeed;
    m_speed     = startStill ? 0.0f : maxSpeed;

    m_stars.resize(static_cast<size_t>(amount));
    for (auto& s : m_stars) {
        initStar(s, /*randomX=*/true);
    }
}

std::shared_ptr<Starfield> Starfield::create(float width, float height, int amount,
                                              StarDirection direction,
                                              float minSpeed, float maxSpeed,
                                              bool startStill) {
    auto node = std::make_shared<Starfield>();
    node->init(width, height, amount, direction, minSpeed, maxSpeed, startStill);
    return node;
}

void Starfield::update(float dt) {
    engine::Node::update(dt);

    for (auto& s : m_stars) {
        float spd = m_speed * s.speedFactor;

        switch (m_direction) {
            case StarDirection::Right:
                s.x += spd * dt;
                if (s.x > m_width  + s.size) initStar(s, /*randomX=*/false);
                break;
            case StarDirection::Left:
                s.x -= spd * dt;
                if (s.x < -s.size)            initStar(s, /*randomX=*/false);
                break;
            case StarDirection::Down:
                s.y += spd * dt;
                if (s.y > m_height + s.size)  initStar(s, /*randomX=*/false);
                break;
            case StarDirection::Up:
                s.y -= spd * dt;
                if (s.y < -s.size)            initStar(s, /*randomX=*/false);
                break;
        }
    }
}

void Starfield::draw(float parentX, float parentY, float, float, float) {
    for (const auto& s : m_stars) {
        drawStarAt(parentX + s.x, parentY + s.y, s.size, s.baseColor);
    }
}

StarSpeedTo::StarSpeedTo(float duration, float targetSpeed, engine::Ease::EaseFunc ease)
    : FiniteTimeAction(duration), m_endSpeed(targetSpeed), m_ease(ease) {
    m_elapsed = 0.0f;
    m_done    = false;
}

std::shared_ptr<StarSpeedTo> StarSpeedTo::create(float duration, float targetSpeed, engine::Ease::EaseFunc ease) {
    return std::make_shared<StarSpeedTo>(duration, targetSpeed, ease);
}

void StarSpeedTo::startWithTarget(engine::Node* target) {
    FiniteTimeAction::startWithTarget(target);
    m_elapsed = 0.0f;
    m_done    = false;

    auto* sf = static_cast<Starfield*>(target);
    m_startSpeed = sf ? sf->getSpeed() : 0.0f;
}

void StarSpeedTo::step(float dt) {
    if (!m_target || m_done) return;

    m_elapsed += dt;
    float progress = m_duration > 0.0f
        ? std::min(1.0f, m_elapsed / m_duration)
        : 1.0f;
    float t = m_ease ? m_ease(progress) : progress;

    auto* sf = static_cast<Starfield*>(m_target);
    if (sf) {
        sf->setSpeed(m_startSpeed + (m_endSpeed - m_startSpeed) * t);
    }

    if (progress >= 1.0f) m_done = true;
}

}