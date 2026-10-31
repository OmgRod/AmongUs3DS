#pragma once

#include <includes.hpp>
#include <game/objects/Starfield.hpp>

/**
 * @brief Game namespace containing game-specific objects, scenes, and color definitions.
 */
namespace game {

    /**
     * @brief Scene class rendering the top screen for asset loading / help.
     */
    class AssetHelperSceneTop : public engine::Scene {
    private:
        std::shared_ptr<engine::Sprite> m_logo;       ///< Logo image sprite.
        std::shared_ptr<engine::Label> m_titleLabel;   ///< Title text label.
        std::shared_ptr<engine::Label> m_descLabel;    ///< Description text label.
        std::shared_ptr<Starfield> m_starfield;        ///< Background starfield effect.

    public:
        /**
         * @brief Factory method to create a shared AssetHelperSceneTop instance.
         * @return Shared pointer to AssetHelperSceneTop.
         */
        static std::shared_ptr<AssetHelperSceneTop> create();
        
        /** @brief Initializes scene nodes and UI elements. */
        void init();
        
        /** @brief Updates scene state per frame. @param dt Delta time in seconds. */
        void update(float dt);
    };

    /**
     * @brief Scene class rendering the bottom touchscreen for asset loading / help.
     */
    class AssetHelperSceneBottom : public engine::Scene {
    private:
        std::shared_ptr<Starfield> m_starfield; ///< Background starfield effect.

    public:
        /**
         * @brief Factory method to create a shared AssetHelperSceneBottom instance.
         * @return Shared pointer to AssetHelperSceneBottom.
         */
        static std::shared_ptr<AssetHelperSceneBottom> create();
        
        /** @brief Initializes scene nodes and UI elements. */
        void init();
        
        /** @brief Updates scene state per frame. @param dt Delta time in seconds. */
        void update(float dt);
    };

}
