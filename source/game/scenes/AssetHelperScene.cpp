#include <game/scenes/AssetHelperScene.hpp>
#include <game/objects/Starfield.hpp>
#include <engine/input.hpp>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>
#include "qrcodegen.hpp" // Include the real qrcodegen library

namespace game {

    std::shared_ptr<AssetHelperSceneTop> AssetHelperSceneTop::create() {
        auto scene = std::make_shared<AssetHelperSceneTop>();
        scene->init();
        return scene;
    }

    void AssetHelperSceneTop::init() {
        auto size = engine::Director::getInstance().getTopScreenSize();

        m_starfield = Starfield::create(size.width, size.height, StarDirection::Down, 50.0f);
        m_starfield->setZOrder(0.0f);
        this->addChild(m_starfield);

        m_logo = engine::Sprite::create("romfs:/sprites/bannerLogo_AmongUs.png");
        if (m_logo) {
            m_logo->setPosition(size.width / 2.0f, 6.0f);
            m_logo->setAnchorPoint(0.5f, 0.0f);
            m_logo->setScale(0.26f);
            m_logo->setZOrder(1.0f);
            this->addChild(m_logo);
        }

        // Compact & readable legal notice
        std::string desc = "Game assets are required. Scan the QR code below\n"
                           "to download the PC Companion Tool.";
        m_descLabel = engine::Label::create("romfs:/fonts/arial.bcfnt", desc, 0.5f, C2D_Color32(220, 220, 240, 255));
        if (m_descLabel) {
            m_descLabel->setPosition(size.width / 2.0f, 44.0f);
            m_descLabel->setAnchorPoint(0.5f, 0.0f);
            m_descLabel->setZOrder(2.0f);
            this->addChild(m_descLabel);
        }

        // --- REAL QR CODE GENERATION USING qrcodegen ---
        std::string qrUrl = "https://github.com/AmongUs3DS/assets-helper";
        
        // Generate the QR Code object using Medium Error Correction
        qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(qrUrl.c_str(), qrcodegen::QrCode::Ecc::MEDIUM);
        
        int qrCodeSize = qr.getSize();
        float targetTotalSize = 88.0f; // Total pixel width/height of the drawn QR code on screen
        float modSize = targetTotalSize / qrCodeSize;
        float qrX = (size.width - targetTotalSize) / 2.0f;
        float qrY = 82.0f;

        // Draw white background container for the QR code modules
        auto qrBg = engine::DrawNode::createRect(targetTotalSize + 8.0f, targetTotalSize + 8.0f, C2D_Color32(255, 255, 255, 255));
        qrBg->setPosition(qrX - 4.0f, qrY - 4.0f);
        qrBg->setZOrder(1.0f);
        this->addChild(qrBg);

        // Render each true module block dynamically from the generated QrCode matrix
        for (int y = 0; y < qrCodeSize; y++) {
            for (int x = 0; x < qrCodeSize; x++) {
                if (qr.getModule(x, y)) {
                    auto mod = engine::DrawNode::createRect(modSize + 0.3f, modSize + 0.3f, C2D_Color32(0, 0, 0, 255));
                    mod->setPosition(qrX + (x * modSize), qrY + (y * modSize));
                    mod->setZOrder(2.0f);
                    this->addChild(mod);
                }
            }
        }

        // Clean, readable URL text below the QR code
        auto linkLabel = engine::Label::create("romfs:/fonts/arial.bcfnt", qrUrl, 0.5f, C2D_Color32(0, 220, 255, 255));
        if (linkLabel) {
            linkLabel->setPosition(size.width / 2.0f, qrY + targetTotalSize + 8.0f);
            linkLabel->setAnchorPoint(0.5f, 0.0f);
            linkLabel->setZOrder(2.0f);
            this->addChild(linkLabel);
        }
    }

    void AssetHelperSceneTop::update(float dt) {
        engine::Scene::update(dt);
    }

    std::shared_ptr<AssetHelperSceneBottom> AssetHelperSceneBottom::create() {
        auto scene = std::make_shared<AssetHelperSceneBottom>();
        scene->init();
        return scene;
    }

    void AssetHelperSceneBottom::init() {
        auto size = engine::Director::getInstance().getBottomScreenSize();

        m_starfield = Starfield::create(size.width, size.height, StarDirection::Down, 50.0f);
        m_starfield->setZOrder(0.0f);
        this->addChild(m_starfield);

        u32 ip = gethostid();
        struct in_addr ip_addr;
        ip_addr.s_addr = ip;
        std::string ipStr = inet_ntoa(ip_addr);

        std::string statusText = "3DS IP Address: " + ipStr + "\n"
                                 "FTP Server Port: 5000\n\n"
                                 "Status: Waiting for PC connection...";
        m_statusLabel = engine::Label::create("romfs:/fonts/arial.bcfnt", statusText, 0.42f, C2D_Color32(255, 255, 255, 255));
        if (m_statusLabel) {
            m_statusLabel->setPosition(20.0f, 20.0f);
            m_statusLabel->setAnchorPoint(0.0f, 0.0f);
            m_statusLabel->setZOrder(1.0f);
            this->addChild(m_statusLabel);
        }

        // Progress bar background
        float barWidth = size.width - 40.0f;
        float barHeight = 20.0f;
        m_progressBarBg = engine::DrawNode::createRect(barWidth, barHeight, C2D_Color32(50, 50, 70, 255));
        if (m_progressBarBg) {
            m_progressBarBg->setPosition(20.0f, 150.0f);
            m_progressBarBg->setAnchorPoint(0.0f, 0.0f);
            m_progressBarBg->setZOrder(1.0f);
            this->addChild(m_progressBarBg);
        }

        // Progress bar fill
        m_progressBar = engine::DrawNode::createRect(2.0f, barHeight - 4.0f, C2D_Color32(0, 220, 100, 255));
        if (m_progressBar) {
            m_progressBar->setPosition(22.0f, 152.0f);
            m_progressBar->setAnchorPoint(0.0f, 0.0f);
            m_progressBar->setZOrder(2.0f);
            this->addChild(m_progressBar);
        }

        m_progressLabel = engine::Label::create("romfs:/fonts/arial.bcfnt", "Waiting for connection...", 0.45f, C2D_Color32(200, 200, 220, 255));
        if (m_progressLabel) {
            m_progressLabel->setPosition(size.width / 2.0f, 180.0f);
            m_progressLabel->setAnchorPoint(0.5f, 0.0f);
            m_progressLabel->setZOrder(2.0f);
            this->addChild(m_progressLabel);
        }

        mkdir("sdmc:/3ds", 0777);
        mkdir("sdmc:/3ds/AmongUs3DS", 0777);

        m_ftpServer = std::make_unique<engine::FtpServer>(5000, "sdmc:/3ds/AmongUs3DS/assets.zip");
        m_ftpServer->start();
    }

    void AssetHelperSceneBottom::update(float dt) {
        engine::Scene::update(dt);

        if (engine::Input::getInstance().isKeyDown(KEY_START)) {
            if (m_ftpServer) m_ftpServer->stop();
            exit(0);
        }

        if (m_ftpServer && !m_transferComplete) {
            float progress = m_ftpServer->getProgress(); 
            size_t received = m_ftpServer->getBytesReceived();

            if (received > 0) {
                float maxBarWidth = engine::Director::getInstance().getBottomScreenSize().width - 44.0f;
                if (m_progressBar) {
                    m_progressBar->setContentSize(std::max(2.0f, maxBarWidth * progress), 16.0f);
                }

                char buf[128];
                snprintf(buf, sizeof(buf), "Received: %zu KB (%.1f%%)", received / 1024, progress * 100.0f);
                if (m_progressLabel) m_progressLabel->setString(buf);
            }

            if (m_ftpServer->isTransferComplete()) {
                m_transferComplete = true;
                m_ftpServer->stop();

                if (m_progressLabel) m_progressLabel->setString("Extracting assets archive...");

                if (extract_zip("sdmc:/3ds/AmongUs3DS/assets.zip", "sdmc:/3ds/AmongUs3DS/assets")) {
                    unlink("sdmc:/3ds/AmongUs3DS/assets.zip");
                    if (m_statusLabel) m_statusLabel->setString("Status: Assets successfully installed!");
                    if (m_progressLabel) m_progressLabel->setString("Success! Press START to launch game.");
                    m_extracted = true;
                } else {
                    if (m_statusLabel) m_statusLabel->setString("Status: Extraction failed!");
                    if (m_progressLabel) m_progressLabel->setString("Extraction failed! Press START to exit.");
                }
            }
        }
    }

}