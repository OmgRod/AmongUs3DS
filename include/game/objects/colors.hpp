#pragma once

#include <3ds.h>
#include <citro2d.h>

namespace game {

    enum PlayerColor : int {
        Red = 0,
        Blue = 1,
        Green = 2,
        Pink = 3,
        Orange = 4,
        Yellow = 5,
        Black = 6,
        White = 7,
        Purple = 8,
        Brown = 9,
        Cyan = 10,
        Lime = 11,
        MaxColors = 18
    };

    struct CharacterColorLayers {
        u32 bodyMain;
        u32 bodyShadow;
        u32 outline;
    };

    namespace ColorPalette {
        const CharacterColorLayers& getLayers(int colorId);
    }

}