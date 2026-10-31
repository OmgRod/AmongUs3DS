#pragma once

#include <3ds.h>
#include <citro2d.h>

/**
 * @brief Game namespace containing game-specific objects, scenes, and color definitions.
 */
namespace game {

    /**
     * @brief Enumeration of available Among Us player color types.
     */
    enum PlayerColor : int {
        Red = 0,     ///< Red color.
        Blue = 1,    ///< Blue color.
        Green = 2,   ///< Green color.
        Pink = 3,    ///< Pink color.
        Orange = 4,  ///< Orange color.
        Yellow = 5,  ///< Yellow color.
        Black = 6,   ///< Black color.
        White = 7,   ///< White color.
        Purple = 8,  ///< Purple color.
        Brown = 9,   ///< Brown color.
        Cyan = 10,   ///< Cyan color.
        Lime = 11,   ///< Lime color.
        MaxColors = 18 ///< Maximum number of supported color variations.
    };

    /**
     * @brief Structure defining color layers (body, shadow, outline) for character sprites.
     */
    struct CharacterColorLayers {
        u32 bodyMain;   ///< Main body fill color.
        u32 bodyShadow; ///< Shadow shading color.
        u32 outline;    ///< Outer stroke outline color.
    };

    /**
     * @brief Namespace providing color palette lookup tables and character color mappings.
     */
    namespace ColorPalette {
        /**
         * @brief Gets the character color layer configuration for a specific color ID.
         * @param colorId Player color identifier.
         * @return Const reference to CharacterColorLayers struct.
         */
        const CharacterColorLayers& getLayers(int colorId);
    }

}
