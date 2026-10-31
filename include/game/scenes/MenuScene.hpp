#pragma once

#include <includes.hpp>
#include <game/objects/Starfield.hpp>

/**
 * @brief Game namespace containing game-specific objects, scenes, and color definitions.
 */
namespace game {

    /**
     * @brief Structure representing a floating crewmate in the menu background.
     */
    struct FloatingCrew {
        std::shared_ptr<engine::Sprite> sprite; ///< Crewmate sprite reference.
        float x, y;                             ///< Current 2D coordinates.
        float vx, vy;                           ///< Velocity vector components.
        float rotation;                         ///< Current rotation angle in degrees.
        float vRot;                             ///< Rotational velocity.
    };

    /**
     * @brief Scene class rendering the main menu top screen.
     */
    class MenuSceneTop : public engine::Scene {
    private:
        std::shared_ptr<Starfield> m_starfield;    ///< Background starfield effect.
        std::vector<FloatingCrew> m_floatingCrew;  ///< List of floating crewmates.

        /**
         * @brief Spawns a floating crewmate member.
         * @param outside True if spawning outside space.
         */
        void spawnCrewMember(bool outside);

    public:
        /**
         * @brief Factory method to create a shared MenuSceneTop instance.
         * @return Shared pointer to MenuSceneTop.
         */
        static std::shared_ptr<MenuSceneTop> create();
        
        /** @brief Initializes scene nodes and UI. */
        void init();
        
        /** @brief Updates scene state per frame. @param dt Delta time in seconds. */
        void update(float dt) override;
    };

    /**
     * @brief Scene class rendering the main menu bottom touchscreen.
     */
    class MenuSceneBottom : public engine::Scene {
    private:
        std::shared_ptr<Starfield> m_starfield; ///< Background starfield effect.

    public:
        /**
         * @brief Factory method to create a shared MenuSceneBottom instance.
         * @return Shared pointer to MenuSceneBottom.
         */
        static std::shared_ptr<MenuSceneBottom> create();
        
        /** @brief Initializes scene nodes and UI. */
        void init();
        
        /** @brief Updates scene state per frame. @param dt Delta time in seconds. */
        void update(float dt) override;
    };

}
