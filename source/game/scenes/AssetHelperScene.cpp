#include <game/scenes/AssetHelperScene.hpp>
#include <game/objects/Starfield.hpp>
#include <engine/input.hpp>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>
#include "qrcodegen.hpp"

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

        std::string desc = "Game assets are required. Scan the QR code below\n"
                           "to download the PC Companion Tool.";
        m_descLabel = engine::Label::create("romfs:/fonts/arial.bcfnt", desc, 0.5f, C2D_Color32(220, 220, 240, 255));
        if (m_descLabel) {
            m_descLabel->setPosition(size.width / 2.0f, 44.0f);
            m_descLabel->setAnchorPoint(0.5f, 0.0f);
            m_descLabel->setZOrder(2.0f);
            this->addChild(m_descLabel);
        }

        std::string qrUrl = "https://github.com/AmongUs3DS/AmongUs3DS/wiki/How-to-Extract-and-Install-Among-Us-Assets";
        
        qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(qrUrl.c_str(), qrcodegen::QrCode::Ecc::MEDIUM);
        
        int qrCodeSize = qr.getSize();
        float targetTotalSize = 88.0f;
        float modSize = targetTotalSize / qrCodeSize;
        float qrX = (size.width - targetTotalSize) / 2.0f;
        float qrY = 82.0f;

        auto qrBg = engine::DrawNode::createRect(targetTotalSize + 8.0f, targetTotalSize + 8.0f, C2D_Color32(255, 255, 255, 255));
        qrBg->setPosition(qrX - 4.0f, qrY - 4.0f);
        qrBg->setZOrder(1.0f);
        this->addChild(qrBg);

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

        auto linkLabel = engine::Label::create("romfs:/fonts/arial.bcfnt", qrUrl, 0.38f, C2D_Color32(0, 220, 255, 255));
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
    }

    void AssetHelperSceneBottom::update(float dt) {
        engine::Scene::update(dt);
    }

}