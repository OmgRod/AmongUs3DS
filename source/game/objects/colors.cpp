#include <game/objects/colors.hpp>

namespace game {
namespace ColorPalette {

    static const CharacterColorLayers s_colorLayers[MaxColors] = {
        { C2D_Color32(215, 30, 34, 255), C2D_Color32(122, 8, 56, 255), C2D_Color32(18, 2, 9, 255) }, // 0: Red
        { C2D_Color32(29, 60, 233, 255), C2D_Color32(9, 21, 142, 255), C2D_Color32(2, 4, 21, 255) }, // 1: Blue
        { C2D_Color32(27, 145, 62, 255), C2D_Color32(10, 77, 46, 255), C2D_Color32(2, 12, 7, 255) }, // 2: Green
        { C2D_Color32(255, 99, 212, 255), C2D_Color32(172, 43, 174, 255), C2D_Color32(25, 7, 25, 255) }, // 3: Pink
        { C2D_Color32(255, 141, 28, 255), C2D_Color32(180, 62, 21, 255), C2D_Color32(26, 10, 4, 255) }, // 4: Orange
        { C2D_Color32(255, 255, 103, 255), C2D_Color32(195, 136, 34, 255), C2D_Color32(28, 20, 6, 255) }, // 5: Yellow
        { C2D_Color32(74, 86, 94, 255), C2D_Color32(30, 31, 38, 255), C2D_Color32(5, 5, 6, 255) }, // 6: Black
        { C2D_Color32(233, 247, 255, 255), C2D_Color32(132, 149, 192, 255), C2D_Color32(20, 22, 28, 255) }, // 7: White
        { C2D_Color32(120, 61, 210, 255), C2D_Color32(59, 23, 124, 255), C2D_Color32(9, 4, 19, 255) }, // 8: Purple
        { C2D_Color32(128, 88, 45, 255), C2D_Color32(94, 38, 21, 255), C2D_Color32(14, 6, 4, 255) }, // 9: Brown
        { C2D_Color32(68, 255, 247, 255), C2D_Color32(36, 169, 191, 255), C2D_Color32(36, 169, 191, 255) }, // 10: Cyan
        { C2D_Color32(91, 254, 75, 255), C2D_Color32(21, 167, 66, 255), C2D_Color32(4, 25, 10, 255) }, // 11: Lime
    };

    const CharacterColorLayers& getLayers(int colorId) {
        if (colorId < 0 || colorId >= MaxColors) {
            return s_colorLayers[0];
        }
        return s_colorLayers[colorId];
    }

}
}