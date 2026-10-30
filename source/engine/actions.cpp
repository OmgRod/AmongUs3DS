#include <engine/actions.hpp>
#include <engine/graphics.hpp>
#include <engine/audio.hpp>

namespace engine {

    MoveTo::MoveTo(float duration, float x, float y, Ease::EaseFunc ease)
        : FiniteTimeAction(duration), m_endX(x), m_endY(y), m_ease(ease) {
        m_elapsed = 0.0f;
        m_done = false;
    }

    std::shared_ptr<MoveTo> MoveTo::create(float duration, float x, float y, Ease::EaseFunc ease) {
        return std::make_shared<MoveTo>(duration, x, y, ease);
    }

    void MoveTo::startWithTarget(Node* target) {
        FiniteTimeAction::startWithTarget(target);
        m_target = target;
        m_elapsed = 0.0f;
        m_done = false;
        if (m_target) {
            m_startX = m_target->getX();
            m_startY = m_target->getY();
        } else {
            m_done = true;
        }
    }

    void MoveTo::step(float dt) {
        if (!m_target || m_done) return;
        m_elapsed += dt;
        float progress = m_duration > 0.0f ? std::min(1.0f, m_elapsed / m_duration) : 1.0f;
        float t = m_ease ? m_ease(progress) : progress;
        
        m_target->setPosition(m_startX + (m_endX - m_startX) * t, m_startY + (m_endY - m_startY) * t);
        if (progress >= 1.0f) m_done = true;
    }

    MoveBy::MoveBy(float duration, float deltaX, float deltaY, Ease::EaseFunc ease)
        : FiniteTimeAction(duration), m_deltaX(deltaX), m_deltaY(deltaY), m_ease(ease) {
        m_elapsed = 0.0f;
        m_done = false;
    }

    std::shared_ptr<MoveBy> MoveBy::create(float duration, float deltaX, float deltaY, Ease::EaseFunc ease) {
        return std::make_shared<MoveBy>(duration, deltaX, deltaY, ease);
    }

    void MoveBy::startWithTarget(Node* target) {
        FiniteTimeAction::startWithTarget(target);
        m_target = target;
        m_elapsed = 0.0f;
        m_done = false;
        if (m_target) {
            m_startX = m_target->getX();
            m_startY = m_target->getY();
        }
    }

    void MoveBy::step(float dt) {
        if (!m_target || m_done) return;
        m_elapsed += dt;
        float progress = m_duration > 0.0f ? std::min(1.0f, m_elapsed / m_duration) : 1.0f;
        float t = m_ease ? m_ease(progress) : progress;
        
        m_target->setPosition(m_startX + m_deltaX * t, m_startY + m_deltaY * t);
        if (progress >= 1.0f) m_done = true;
    }

    ScaleTo::ScaleTo(float duration, float scale, Ease::EaseFunc ease)
        : FiniteTimeAction(duration), m_endX(scale), m_endY(scale), m_ease(ease) {
        m_elapsed = 0.0f;
        m_done = false;
    }

    ScaleTo::ScaleTo(float duration, float scaleX, float scaleY, Ease::EaseFunc ease)
        : FiniteTimeAction(duration), m_endX(scaleX), m_endY(scaleY), m_ease(ease) {
        m_elapsed = 0.0f;
        m_done = false;
    }

    std::shared_ptr<ScaleTo> ScaleTo::create(float duration, float scale, Ease::EaseFunc ease) {
        return std::make_shared<ScaleTo>(duration, scale, ease);
    }

    std::shared_ptr<ScaleTo> ScaleTo::create(float duration, float scaleX, float scaleY, Ease::EaseFunc ease) {
        return std::make_shared<ScaleTo>(duration, scaleX, scaleY, ease);
    }

    void ScaleTo::startWithTarget(Node* target) {
        FiniteTimeAction::startWithTarget(target);
        m_target = target;
        m_elapsed = 0.0f;
        m_done = false;
        if (m_target) {
            m_startX = m_target->getScaleX();
            m_startY = m_target->getScaleY();
        } else {
            m_done = true;
        }
    }

    void ScaleTo::step(float dt) {
        if (!m_target || m_done) return;
        m_elapsed += dt;
        float progress = m_duration > 0.0f ? std::min(1.0f, m_elapsed / m_duration) : 1.0f;
        float t = m_ease ? m_ease(progress) : progress;
        
        m_target->setScale(m_startX + (m_endX - m_startX) * t, m_startY + (m_endY - m_startY) * t);
        if (progress >= 1.0f) m_done = true;
    }

    RotateBy::RotateBy(float duration, float deltaAngle, Ease::EaseFunc ease)
        : FiniteTimeAction(duration), m_deltaAngle(deltaAngle), m_ease(ease) {
        m_elapsed = 0.0f;
        m_done = false;
    }

    std::shared_ptr<RotateBy> RotateBy::create(float duration, float deltaAngle, Ease::EaseFunc ease) {
        return std::make_shared<RotateBy>(duration, deltaAngle, ease);
    }

    void RotateBy::startWithTarget(Node* target) {
        FiniteTimeAction::startWithTarget(target);
        m_target = target;
        m_elapsed = 0.0f;
        m_done = false;
        if (m_target) {
            m_startAngle = m_target->getRotation();
        } else {
            m_done = true;
        }
    }

    void RotateBy::step(float dt) {
        if (!m_target || m_done) return;
        m_elapsed += dt;
        float progress = m_duration > 0.0f ? std::min(1.0f, m_elapsed / m_duration) : 1.0f;
        float t = m_ease ? m_ease(progress) : progress;

        m_target->setRotation(m_startAngle + m_deltaAngle * t);
        if (progress >= 1.0f) m_done = true;
    }

    FadeIn::FadeIn(float duration, Ease::EaseFunc ease)
        : FiniteTimeAction(duration), m_ease(ease) {
        m_elapsed = 0.0f;
        m_done = false;
    }

    std::shared_ptr<FadeIn> FadeIn::create(float duration, Ease::EaseFunc ease) {
        return std::make_shared<FadeIn>(duration, ease);
    }

    void FadeIn::startWithTarget(Node* target) {
        FiniteTimeAction::startWithTarget(target);
        m_target = target;
        m_elapsed = 0.0f;
        m_done = false;
        if (m_target) {
            m_startOpacity = m_target->getOpacity();
        } else {
            m_done = true;
        }
    }

    void FadeIn::step(float dt) {
        if (!m_target || m_done) return;
        m_elapsed += dt;
        float progress = m_duration > 0.0f ? std::min(1.0f, m_elapsed / m_duration) : 1.0f;
        float t = m_ease ? m_ease(progress) : progress;

        m_target->setOpacity(m_startOpacity + (m_endOpacity - m_startOpacity) * t);
        if (progress >= 1.0f) m_done = true;
    }

    FadeOut::FadeOut(float duration, Ease::EaseFunc ease)
        : FiniteTimeAction(duration), m_ease(ease) {
        m_elapsed = 0.0f;
        m_done = false;
    }

    std::shared_ptr<FadeOut> FadeOut::create(float duration, Ease::EaseFunc ease) {
        return std::make_shared<FadeOut>(duration, ease);
    }

    void FadeOut::startWithTarget(Node* target) {
        FiniteTimeAction::startWithTarget(target);
        m_target = target;
        m_elapsed = 0.0f;
        m_done = false;
        if (m_target) {
            m_startOpacity = m_target->getOpacity();
        } else {
            m_done = true;
        }
    }

    void FadeOut::step(float dt) {
        if (!m_target || m_done) return;
        m_elapsed += dt;
        float progress = m_duration > 0.0f ? std::min(1.0f, m_elapsed / m_duration) : 1.0f;
        float t = m_ease ? m_ease(progress) : progress;

        m_target->setOpacity(m_startOpacity + (m_endOpacity - m_startOpacity) * t);
        if (progress >= 1.0f) m_done = true;
    }

    std::shared_ptr<DelayTime> DelayTime::create(float duration) {
        auto action = std::make_shared<DelayTime>(duration);
        action->m_elapsed = 0.0f;
        action->m_done = false;
        return action;
    }

    void DelayTime::step(float dt) {
        if (m_done) return;
        m_elapsed += dt;
        if (m_elapsed >= m_duration) m_done = true;
    }

    std::shared_ptr<CallFunc> CallFunc::create(std::function<void()> func) {
        return std::make_shared<CallFunc>(func);
    }

    void CallFunc::startWithTarget(Node* target) {
        FiniteTimeAction::startWithTarget(target);
        if (!m_target) return;
        
        m_target = target;
        if (m_func) m_func();
        m_done = true;
    }

    void CallFunc::step(float dt) {
        m_done = true;
    }

    PlaySound::PlaySound(std::string soundPath, bool loop, float volume, float pan, float pitch, std::string tag)
        : FiniteTimeAction(0.0f), m_soundPath(soundPath), m_loop(loop),
          m_volume(volume), m_pan(pan), m_pitch(pitch), m_tag(tag) {
        m_elapsed = 0.0f;
        m_done = false;
    }

    std::shared_ptr<PlaySound> PlaySound::create(std::string soundPath, bool loop, float volume, float pan, float pitch, std::string tag) {
        return std::make_shared<PlaySound>(soundPath, loop, volume, pan, pitch, tag);
    }

    void PlaySound::startWithTarget(Node* target) {
        FiniteTimeAction::startWithTarget(target);
        m_target = target;
        m_soundId = AudioEngine::getInstance().playSFX(m_soundPath, m_volume, m_pan, m_pitch, m_loop, m_tag);
        m_done = true;
    }

    void PlaySound::step(float dt) {
        m_done = true;
    }

    StopSound::StopSound(uint32_t soundId)
        : FiniteTimeAction(0.0f), m_soundId(soundId), m_mode(Mode::ById) {
        m_done = false;
    }

    StopSound::StopSound(std::string identifier, Mode mode)
        : FiniteTimeAction(0.0f), m_identifier(std::move(identifier)), m_mode(mode) {
        m_done = false;
    }

    StopSound::StopSound()
        : FiniteTimeAction(0.0f), m_mode(Mode::All) {
        m_done = false;
    }

    std::shared_ptr<StopSound> StopSound::create(uint32_t soundId) {
        return std::make_shared<StopSound>(soundId);
    }

    std::shared_ptr<StopSound> StopSound::createByTag(const std::string& tag) {
        return std::make_shared<StopSound>(tag, Mode::ByTag);
    }

    std::shared_ptr<StopSound> StopSound::createByPath(const std::string& path) {
        return std::make_shared<StopSound>(path, Mode::ByPath);
    }

    std::shared_ptr<StopSound> StopSound::createAll() {
        return std::make_shared<StopSound>();
    }

    void StopSound::startWithTarget(Node* target) {
        FiniteTimeAction::startWithTarget(target);
        m_target = target;
        auto& audio = AudioEngine::getInstance();
        switch (m_mode) {
            case Mode::ById:
                audio.stop(m_soundId);
                break;
            case Mode::ByTag:
                audio.stopByTag(m_identifier);
                break;
            case Mode::ByPath:
                audio.stopByPath(m_identifier);
                break;
            case Mode::All:
                audio.stopAll();
                break;
        }
        m_done = true;
    }

    void StopSound::step(float dt) {
        m_done = true;
    }

    Sequence::Sequence(std::vector<std::shared_ptr<FiniteTimeAction>> actions)
        : FiniteTimeAction(0.0f), m_actions(std::move(actions)) {
        float total = 0.0f;
        for (auto& a : m_actions) if (a) total += a->getDuration();
        m_duration = total;
        m_elapsed = 0.0f;
        m_done = false;
    }

    void Sequence::startWithTarget(Node* target) {
        FiniteTimeAction::startWithTarget(target);
        m_target = target;
        m_index = 0;
        m_elapsed = 0.0f;
        m_done = false;
        if (!m_actions.empty() && m_actions[0]) {
            m_actions[0]->startWithTarget(target);
        } else {
            m_done = true;
        }
    }

    void Sequence::step(float dt) {
        if (!m_target || m_done || m_actions.empty()) return;
        m_elapsed += dt;
        
        while (m_index < m_actions.size()) {
            auto& current = m_actions[m_index];
            if (!current) {
                m_index++;
                continue;
            }

            current->step(dt);
            if (current->isDone()) {
                m_index++;
                if (m_index < m_actions.size() && m_actions[m_index]) {
                    m_actions[m_index]->startWithTarget(m_target);
                }
            } else {
                break;
            }
        }

        if (m_index >= m_actions.size()) m_done = true;
    }

    Spawn::Spawn(std::vector<std::shared_ptr<FiniteTimeAction>> actions)
        : FiniteTimeAction(0.0f), m_actions(std::move(actions)) {
        float maxDur = 0.0f;
        for (auto& a : m_actions) if (a && a->getDuration() > maxDur) maxDur = a->getDuration();
        m_duration = maxDur;
        m_elapsed = 0.0f;
        m_done = false;
    }

    void Spawn::startWithTarget(Node* target) {
        FiniteTimeAction::startWithTarget(target);
        m_target = target;
        m_elapsed = 0.0f;
        m_done = false;
        for (auto& a : m_actions) {
            if (a) a->startWithTarget(target);
        }
    }

    void Spawn::step(float dt) {
        if (!m_target || m_done) return;
        m_elapsed += dt;
        bool allDone = true;
        for (auto& a : m_actions) {
            if (a && !a->isDone()) {
                a->step(dt);
                if (!a->isDone()) allDone = false;
            }
        }
        if (allDone) m_done = true;
    }

    RepeatForever::RepeatForever(std::shared_ptr<FiniteTimeAction> action)
        : m_innerAction(action) {}

    std::shared_ptr<RepeatForever> RepeatForever::create(std::shared_ptr<FiniteTimeAction> action) {
        return std::make_shared<RepeatForever>(action);
    }

    void RepeatForever::startWithTarget(Node* target) {
        Action::startWithTarget(target);
        m_target = target;
        if (m_innerAction) m_innerAction->startWithTarget(target);
    }

    void RepeatForever::step(float dt) {
        if (!m_target || !m_innerAction) return;
        m_innerAction->step(dt);
        if (m_innerAction->isDone()) {
            m_innerAction->startWithTarget(m_target);
        }
    }

}