#pragma once

#include <engine/graphics.hpp>
#include <game/objects/colors.hpp>

/**
 * @brief Game namespace containing game-specific objects, scenes, and color definitions.
 */
namespace game {

    /**
     * @brief Sprite class specialized for rendering and recoloring player characters.
     */
    class PlayerSprite : public engine::Sprite {
    private:
        int m_colorId = 0; ///< Current player color identifier.

        /**
         * @brief Applies color mapping transformations to raw image pixel data.
         * @param imgData Pointer to raw image pixel buffer.
         * @param width Image width in pixels.
         * @param height Image height in pixels.
         * @param colorId Target player color identifier.
         */
        void applyPlayerColorMapping(unsigned char* imgData, int width, int height, int colorId);

    public:
        /** @brief Default constructor. */
        PlayerSprite() = default;
        
        /** @brief Virtual destructor. */
        virtual ~PlayerSprite() = default;

        /**
         * @brief Factory method to create a shared PlayerSprite instance from file path and color ID.
         * @param path Asset file path string.
         * @param colorId Player color identifier.
         * @return Shared pointer to PlayerSprite.
         */
        static std::shared_ptr<PlayerSprite> create(const std::string& path, int colorId);

        /**
         * @brief Initializes sprite texture and applies color mapping.
         * @param path Asset file path string.
         * @param colorId Player color identifier.
         * @return True on success, false otherwise.
         */
        bool initWithFileAndColor(const std::string& path, int colorId);

        /**
         * @brief Sets the player color and updates sprite color mapping.
         * @param colorId Player color identifier.
         */
        void setPlayerColor(int colorId);
        
        /** @brief Gets the current player color identifier. @return Color ID. */
        int getPlayerColor() const { return m_colorId; }
    };

}
