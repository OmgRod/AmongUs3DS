#pragma once

#include <engine/graphics.hpp>
#include <game/objects/colors.hpp>

namespace game {

    class PlayerSprite : public engine::Sprite {
    private:
        int m_colorId = 0;

        void applyPlayerColorMapping(unsigned char* imgData, int width, int height, int colorId);

    public:
        PlayerSprite() = default;
        virtual ~PlayerSprite() = default;

        static std::shared_ptr<PlayerSprite> create(const std::string& path, int colorId);

        bool initWithFileAndColor(const std::string& path, int colorId);

        void setPlayerColor(int colorId);
        int getPlayerColor() const { return m_colorId; }
    };

}