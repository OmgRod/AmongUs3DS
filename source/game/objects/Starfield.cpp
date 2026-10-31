#include <game/objects/Starfield.hpp>
#include <engine/graphics.hpp>
#include <engine/math.hpp>
#include <cmath>

namespace game {

void Starfield::init(float width, float height, StarDirection direction, float speed) {
    m_width     = width;
    m_height    = height;
    m_direction = direction;
    m_speed     = speed;

    int starCount = 80;
    m_stars.reserve(starCount);

    for (int i = 0; i < starCount; ++i) {
        Star s;
        s.x = engine::Math::randomFloat(0.0f, m_width);
        s.y = engine::Math::randomFloat(0.0f, m_height);
        s.size = engine::Math::randomFloat(1.0f, 2.5f);
        s.speedMultiplier = engine::Math::randomFloat(0.4f, 1.8f);
        s.brightness = static_cast<u8>(engine::Math::randomInt(100, 255));
        m_stars.push_back(s);
    }
}

std::shared_ptr<Starfield> Starfield::create(float width, float height, StarDirection direction, float speed) {
    auto node = std::make_shared<Starfield>();
    node->init(width, height, direction, speed);
    return node;
}

void Starfield::update(float dt) {
    engine::Node::update(dt);

    if (m_speed == 0.0f || m_stars.empty()) return;

    for (auto& star : m_stars) {
        float moveDelta = m_speed * star.speedMultiplier * dt;

        switch (m_direction) {
            case StarDirection::Right:
                star.x += moveDelta;
                if (star.x > m_width) star.x = 0.0f;
                break;
            case StarDirection::Left:
                star.x -= moveDelta;
                if (star.x < 0.0f) star.x = m_width;
                break;
            case StarDirection::Down:
                star.y += moveDelta;
                if (star.y > m_height) star.y = 0.0f;
                break;
            case StarDirection::Up:
                star.y -= moveDelta;
                if (star.y < 0.0f) star.y = m_height;
                break;
        }
    }
}

void Starfield::draw(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation) {
    engine::Node::draw(parentX, parentY, parentScaleX, parentScaleY, parentRotation);

    float absX = parentX + m_x;
    float absY = parentY + m_y;

    for (const auto& star : m_stars) {
        float sx = absX + star.x * parentScaleX;
        float sy = absY + star.y * parentScaleY;
        float sradius = star.size * parentScaleX;

        u32 color = C2D_Color32(star.brightness, star.brightness, star.brightness, star.brightness);
        C2D_DrawCircleSolid(sx, sy, 0.5f, sradius, color);
    }
}

// ---------------------------------------------------------------------------
// StarSpeedTo action implementation
// ---------------------------------------------------------------------------

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

} // namespace game
