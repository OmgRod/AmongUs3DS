#pragma once

#include <includes.hpp>

namespace engine {

    class Node;

    namespace Ease {
        using EaseFunc = std::function<float(float)>;

        inline float Linear(float t) { return t; }
        inline float QuadIn(float t) { return t * t; }
        inline float QuadOut(float t) { return t * (2.0f - t); }
        inline float QuadInOut(float t) { return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t; }
        inline float SineIn(float t) { return 1.0f - std::cos(t * (3.1415926535f / 2.0f)); }
        inline float SineOut(float t) { return std::sin(t * (3.1415926535f / 2.0f)); }
        inline float SineInOut(float t) { return -0.5f * (std::cos(3.1415926535f * t) - 1.0f); }
        
        inline float ElasticOut(float t) {
            if (t == 0.0f) return 0.0f;
            if (t == 1.0f) return 1.0f;
            float p = 0.3f;
            return std::pow(2.0f, -10.0f * t) * std::sin((t - p / 4.0f) * (2.0f * 3.1415926535f) / p) + 1.0f;
        }

        inline float BounceOut(float t) {
            if (t < (1.0f / 2.75f)) {
                return 7.5625f * t * t;
            } else if (t < (2.0f / 2.75f)) {
                t -= (1.5f / 2.75f);
                return 7.5625f * t * t + 0.75f;
            } else if (t < (2.5f / 2.75f)) {
                t -= (2.25f / 2.75f);
                return 7.5625f * t * t + 0.9375f;
            } else {
                t -= (2.625f / 2.75f);
                return 7.5625f * t * t + 0.984375f;
            }
        }
    }

    class Action {
    protected:
        Node* m_target = nullptr;
        bool m_done = false;

    public:
        virtual ~Action() = default;
        virtual void startWithTarget(Node* target) { m_target = target; m_done = false; }
        virtual void step(float dt) = 0;
        virtual bool isDone() const { return m_done; }
        virtual void stop() { m_target = nullptr; }
        Node* getTarget() const { return m_target; }
    };

    class FiniteTimeAction : public Action {
    protected:
        float m_duration = 0.0f;
        float m_elapsed = 0.0f;

    public:
        FiniteTimeAction(float duration) : m_duration(duration) {}
        float getDuration() const { return m_duration; }
        void setDuration(float duration) { m_duration = duration; }
        
        void startWithTarget(Node* target) override {
            Action::startWithTarget(target);
            m_elapsed = 0.0f;
        }
    };

    class MoveTo : public FiniteTimeAction {
    private:
        float m_endX, m_endY;
        float m_startX = 0.0f, m_startY = 0.0f;
        Ease::EaseFunc m_ease;

    public:
        MoveTo(float duration, float x, float y, Ease::EaseFunc ease = Ease::Linear);
        static std::shared_ptr<MoveTo> create(float duration, float x, float y, Ease::EaseFunc ease = Ease::Linear);
        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    class MoveBy : public FiniteTimeAction {
    private:
        float m_deltaX, m_deltaY;
        float m_startX = 0.0f, m_startY = 0.0f;
        Ease::EaseFunc m_ease;

    public:
        MoveBy(float duration, float deltaX, float deltaY, Ease::EaseFunc ease = Ease::Linear);
        static std::shared_ptr<MoveBy> create(float duration, float deltaX, float deltaY, Ease::EaseFunc ease = Ease::Linear);
        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    class ScaleTo : public FiniteTimeAction {
    private:
        float m_endX, m_endY;
        float m_startX = 1.0f, m_startY = 1.0f;
        Ease::EaseFunc m_ease;

    public:
        ScaleTo(float duration, float scale, Ease::EaseFunc ease = Ease::Linear);
        ScaleTo(float duration, float scaleX, float scaleY, Ease::EaseFunc ease = Ease::Linear);
        static std::shared_ptr<ScaleTo> create(float duration, float scale, Ease::EaseFunc ease = Ease::Linear);
        static std::shared_ptr<ScaleTo> create(float duration, float scaleX, float scaleY, Ease::EaseFunc ease = Ease::Linear);
        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    class RotateBy : public FiniteTimeAction {
    private:
        float m_deltaAngle;
        float m_startAngle = 0.0f;
        Ease::EaseFunc m_ease;

    public:
        RotateBy(float duration, float deltaAngle, Ease::EaseFunc ease = Ease::Linear);
        static std::shared_ptr<RotateBy> create(float duration, float deltaAngle, Ease::EaseFunc ease = Ease::Linear);
        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    class FadeIn : public FiniteTimeAction {
    private:
        float m_startOpacity = 0.0f;
        float m_endOpacity = 1.0f;
        Ease::EaseFunc m_ease;

    public:
        FadeIn(float duration, Ease::EaseFunc ease = Ease::Linear);
        static std::shared_ptr<FadeIn> create(float duration, Ease::EaseFunc ease = Ease::Linear);
        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    class FadeOut : public FiniteTimeAction {
    private:
        float m_startOpacity = 1.0f;
        float m_endOpacity = 0.0f;
        Ease::EaseFunc m_ease;

    public:
        FadeOut(float duration, Ease::EaseFunc ease = Ease::Linear);
        static std::shared_ptr<FadeOut> create(float duration, Ease::EaseFunc ease = Ease::Linear);
        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    class DelayTime : public FiniteTimeAction {
    public:
        DelayTime(float duration) : FiniteTimeAction(duration) {}
        static std::shared_ptr<DelayTime> create(float duration);
        void step(float dt) override;
    };

    class CallFunc : public FiniteTimeAction {
    private:
        std::function<void()> m_func;

    public:
        CallFunc(std::function<void()> func) : FiniteTimeAction(0.0f), m_func(func) {}
        static std::shared_ptr<CallFunc> create(std::function<void()> func);
        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    class PlaySound : public FiniteTimeAction {
    private:
        std::string m_soundPath;
        bool m_loop;

    public:
        PlaySound(std::string soundPath, bool loop = false);
        static std::shared_ptr<PlaySound> create(std::string soundPath, bool loop = false);
        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    class Sequence : public FiniteTimeAction {
    private:
        std::vector<std::shared_ptr<FiniteTimeAction>> m_actions;
        size_t m_index = 0;

    public:
        Sequence(std::vector<std::shared_ptr<FiniteTimeAction>> actions);
        
        template<typename... Args>
        static std::shared_ptr<Sequence> create(Args... args) {
            std::vector<std::shared_ptr<FiniteTimeAction>> vec = { args... };
            return std::make_shared<Sequence>(vec);
        }

        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    class Spawn : public FiniteTimeAction {
    private:
        std::vector<std::shared_ptr<FiniteTimeAction>> m_actions;

    public:
        Spawn(std::vector<std::shared_ptr<FiniteTimeAction>> actions);

        template<typename... Args>
        static std::shared_ptr<Spawn> create(Args... args) {
            std::vector<std::shared_ptr<FiniteTimeAction>> vec = { args... };
            return std::make_shared<Spawn>(vec);
        }

        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    class RepeatForever : public Action {
    private:
        std::shared_ptr<FiniteTimeAction> m_innerAction;

    public:
        RepeatForever(std::shared_ptr<FiniteTimeAction> action);
        static std::shared_ptr<RepeatForever> create(std::shared_ptr<FiniteTimeAction> action);
        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

}