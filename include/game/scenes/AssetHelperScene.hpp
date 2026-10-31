#pragma once

#include <includes.hpp>
#include <game/objects/Starfield.hpp>
#include <ftp_server.hpp>
#include <miniz.h>

namespace game {

    class AssetHelperSceneTop : public engine::Scene {
    private:
        std::shared_ptr<engine::Sprite> m_logo;
        std::shared_ptr<engine::Label> m_titleLabel;
        std::shared_ptr<engine::Label> m_descLabel;
        std::shared_ptr<Starfield> m_starfield;

    public:
        static std::shared_ptr<AssetHelperSceneTop> create();
        void init();
        void update(float dt);
    };

    class AssetHelperSceneBottom : public engine::Scene {
    private:
        std::shared_ptr<engine::Label> m_statusLabel;
        std::shared_ptr<engine::Label> m_progressLabel;
        std::shared_ptr<engine::DrawNode> m_progressBarBg;
        std::shared_ptr<engine::DrawNode> m_progressBar;
        std::shared_ptr<Starfield> m_starfield;
        std::unique_ptr<engine::FtpServer> m_ftpServer;
        bool m_transferComplete = false;
        bool m_extracted = false;

    public:
        static std::shared_ptr<AssetHelperSceneBottom> create();
        void init();
        void update(float dt);
    };

}
