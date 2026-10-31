#pragma once

#include <includes.hpp>
#include <engine/actions.hpp>

/**
 * @brief Game namespace containing game-specific objects, scenes, and color definitions.
 */
namespace game {

/**
 * @brief Star movement direction enumeration.
 */
enum class StarDirection {
    Up,    ///< Stars move upwards.
    Down,  ///< Stars move downwards.
    Left,  ///< Stars move leftwards.
    Right  ///< Stars move rightwards.
};

/**
 * @brief Node rendering a scrolling starfield background effect.
 */
class Starfield : public engine::Node {
private:
    float         m_width     = 0.0f;               ///< Starfield viewport width.
    float         m_height    = 0.0f;               ///< Starfield viewport height.
    StarDirection m_direction = StarDirection::Down; ///< Star movement direction.
    float         m_speed     = 60.0f;              ///< Base star movement speed in pixels per second.

    /**
     * @brief Structure representing an individual star particle.
     */
    struct Star {
        float x;                 ///< X coordinate position.
        float y;                 ///< Y coordinate position.
        float size;              ///< Star particle size in pixels.
        float speedMultiplier;   ///< Individual speed scale factor.
        u8 brightness;           ///< Star brightness intensity [0, 255].
    };

    std::vector<Star> m_stars; ///< Collection of active star particles.

    /**
     * @brief Initializes the starfield particles with given dimensions and speed.
     * @param width Viewport width.
     * @param height Viewport height.
     * @param direction Movement direction.
     * @param speed Movement speed.
     */
    void init(float width, float height, StarDirection direction, float speed);

public:
    /** @brief Default constructor. */
    Starfield() = default;
    
    /** @brief Virtual destructor. */
    virtual ~Starfield() = default;

    /**
     * @brief Factory method to create a shared Starfield instance.
     * @param width Viewport width in pixels.
     * @param height Viewport height in pixels.
     * @param direction Star movement direction.
     * @param speed Base speed in pixels per second.
     * @return Shared pointer to Starfield.
     */
    static std::shared_ptr<Starfield> create(
        float width,
        float height,
        StarDirection direction = StarDirection::Down,
        float speed = 60.f
    );

    /** @brief Sets the star movement speed. @param speed Speed in pixels/sec. */
    void setSpeed(float speed) { m_speed = speed; }
    
    /** @brief Gets the star movement speed. @return Speed value. */
    float getSpeed() const     { return m_speed; }

    /** @brief Updates star positions per frame. @param dt Delta time in seconds. */
    virtual void update(float dt) override;
    
    /** @brief Renders the starfield. @param parentX Parent world X. @param parentY Parent world Y. @param parentScaleX Parent X scale. @param parentScaleY Parent Y scale. @param parentRotation Parent rotation. */
    virtual void draw(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation) override;
};

/**
 * @brief Action to smoothly interpolate starfield speed over time.
 */
class StarSpeedTo : public engine::FiniteTimeAction {
    float m_startSpeed = 0.0f;             ///< Starting speed value.
    float m_endSpeed   = 0.0f;             ///< Target ending speed value.
    engine::Ease::EaseFunc m_ease;         ///< Easing curve function.

public:
    /**
     * @brief Constructs a StarSpeedTo action.
     * @param duration Duration in seconds.
     * @param targetSpeed Target speed value.
     * @param ease Easing function curve.
     */
    StarSpeedTo(float duration, float targetSpeed,
                engine::Ease::EaseFunc ease = engine::Ease::Linear);

    /**
     * @brief Factory method to create a shared StarSpeedTo action instance.
     * @param duration Duration in seconds.
     * @param targetSpeed Target speed value.
     * @param ease Easing function curve.
     * @return Shared pointer to StarSpeedTo.
     */
    static std::shared_ptr<StarSpeedTo> create(
        float duration,
        float targetSpeed,
        engine::Ease::EaseFunc ease = engine::Ease::Linear
    );

    /** @brief Initializes action with target node. @param target Target node pointer. */
    void startWithTarget(engine::Node* target) override;
    
    /** @brief Updates action progress over time. @param dt Delta time in seconds. */
    void step(float dt) override;
};

}
