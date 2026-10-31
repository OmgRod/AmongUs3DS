#pragma once

#include <includes.hpp>

/**
 * @brief Main engine namespace.
 */
namespace engine {

    class Node;

    /**
     * @brief Collection of easing functions used for animations and interpolation.
     */
    namespace Ease {
        /** @brief Function signature for an easing curve taking progress [0, 1] and returning interpolated progress. */
        using EaseFunc = std::function<float(float)>;

        /** @brief Linear easing (no acceleration/deceleration). @param t Progress value [0, 1]. @return Interpolated value. */
        inline float Linear(float t) { return t; }
        
        /** @brief Quadratic acceleration easing (in). @param t Progress value [0, 1]. @return Interpolated value. */
        inline float QuadIn(float t) { return t * t; }
        
        /** @brief Quadratic deceleration easing (out). @param t Progress value [0, 1]. @return Interpolated value. */
        inline float QuadOut(float t) { return t * (2.0f - t); }
        
        /** @brief Quadratic acceleration and deceleration easing (in-out). @param t Progress value [0, 1]. @return Interpolated value. */
        inline float QuadInOut(float t) { return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t; }
        
        /** @brief Sine acceleration easing (in). @param t Progress value [0, 1]. @return Interpolated value. */
        inline float SineIn(float t) { return 1.0f - std::cos(t * (3.1415926535f / 2.0f)); }
        
        /** @brief Sine deceleration easing (out). @param t Progress value [0, 1]. @return Interpolated value. */
        inline float SineOut(float t) { return std::sin(t * (3.1415926535f / 2.0f)); }
        
        /** @brief Sine acceleration and deceleration easing (in-out). @param t Progress value [0, 1]. @return Interpolated value. */
        inline float SineInOut(float t) { return -0.5f * (std::cos(3.1415926535f * t) - 1.0f); }
        
        /** @brief Elastic spring-like deceleration easing (out). @param t Progress value [0, 1]. @return Interpolated value. */
        inline float ElasticOut(float t) {
            if (t == 0.0f) return 0.0f;
            if (t == 1.0f) return 1.0f;
            float p = 0.3f;
            return std::pow(2.0f, -10.0f * t) * std::sin((t - p / 4.0f) * (2.0f * 3.1415926535f) / p) + 1.0f;
        }

        /** @brief Bounce deceleration easing (out). @param t Progress value [0, 1]. @return Interpolated value. */
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

    /**
     * @brief Base class for all engine actions that can be applied to nodes.
     */
    class Action {
    protected:
        Node* m_target = nullptr; ///< The target node this action is operating on.
        bool m_done = false;      ///< Flag indicating whether the action has completed its execution.

    public:
        /** @brief Virtual destructor. */
        virtual ~Action() = default;

        /** 
         * @brief Initializes the action with a target node.
         * @param target Pointer to the target node.
         */
        virtual void startWithTarget(Node* target) { m_target = target; m_done = false; }

        /** 
         * @brief Updates the action state over time.
         * @param dt Delta time elapsed since the last frame in seconds.
         */
        virtual void step(float dt) = 0;

        /** 
         * @brief Checks if the action has completed.
         * @return True if completed, false otherwise.
         */
        virtual bool isDone() const { return m_done; }

        /** @brief Stops the action and detaches the target reference. */
        virtual void stop() { m_target = nullptr; }

        /** 
         * @brief Gets the current target node of the action.
         * @return Pointer to the target node.
         */
        Node* getTarget() const { return m_target; }
    };

    /**
     * @brief Base class for actions that execute over a specified duration.
     */
    class FiniteTimeAction : public Action {
    protected:
        float m_duration = 0.0f; ///< Total duration of the action in seconds.
        float m_elapsed = 0.0f;  ///< Time elapsed since the action started.

    public:
        /** 
         * @brief Constructs a finite time action with a given duration.
         * @param duration Duration in seconds.
         */
        FiniteTimeAction(float duration) : m_duration(duration) {}

        /** 
         * @brief Gets the duration of the action.
         * @return Duration in seconds.
         */
        float getDuration() const { return m_duration; }

        /** 
         * @brief Sets the duration of the action.
         * @param duration New duration in seconds.
         */
        void setDuration(float duration) { m_duration = duration; }
        
        /** 
         * @brief Initializes the action with a target node and resets elapsed time.
         * @param target Pointer to the target node.
         */
        void startWithTarget(Node* target) override {
            Action::startWithTarget(target);
            m_elapsed = 0.0f;
        }
    };

    /**
     * @brief Action that moves a node to an absolute position over time.
     */
    class MoveTo : public FiniteTimeAction {
    private:
        float m_endX, m_endY;       ///< Target destination coordinates.
        float m_startX = 0.0f, m_startY = 0.0f; ///< Starting coordinates captured upon start.
        Ease::EaseFunc m_ease;      ///< Easing function applied to movement.

    public:
        /**
         * @brief Constructs a MoveTo action.
         * @param duration Action duration in seconds.
         * @param x Target X coordinate.
         * @param y Target Y coordinate.
         * @param ease Optional easing function curve.
         */
        MoveTo(float duration, float x, float y, Ease::EaseFunc ease = Ease::Linear);

        /**
         * @brief Factory method to create a shared pointer instance of MoveTo.
         * @param duration Action duration in seconds.
         * @param x Target X coordinate.
         * @param y Target Y coordinate.
         * @param ease Optional easing function curve.
         * @return Shared pointer to the created MoveTo action.
         */
        static std::shared_ptr<MoveTo> create(float duration, float x, float y, Ease::EaseFunc ease = Ease::Linear);

        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    /**
     * @brief Action that moves a node by a relative delta offset over time.
     */
    class MoveBy : public FiniteTimeAction {
    private:
        float m_deltaX, m_deltaY;   ///< Offset values to translate by.
        float m_startX = 0.0f, m_startY = 0.0f; ///< Starting coordinates captured upon start.
        Ease::EaseFunc m_ease;      ///< Easing function applied to movement.

    public:
        /**
         * @brief Constructs a MoveBy action.
         * @param duration Action duration in seconds.
         * @param deltaX Offset along X axis.
         * @param deltaY Offset along Y axis.
         * @param ease Optional easing function curve.
         */
        MoveBy(float duration, float deltaX, float deltaY, Ease::EaseFunc ease = Ease::Linear);

        /**
         * @brief Factory method to create a shared pointer instance of MoveBy.
         * @param duration Action duration in seconds.
         * @param deltaX Offset along X axis.
         * @param deltaY Offset along Y axis.
         * @param ease Optional easing function curve.
         * @return Shared pointer to the created MoveBy action.
         */
        static std::shared_ptr<MoveBy> create(float duration, float deltaX, float deltaY, Ease::EaseFunc ease = Ease::Linear);

        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    /**
     * @brief Action that scales a node to target scale factors over time.
     */
    class ScaleTo : public FiniteTimeAction {
    private:
        float m_endX, m_endY;       ///< Target scale factors for X and Y axes.
        float m_startX = 1.0f, m_startY = 1.0f; ///< Initial scale factors captured upon start.
        Ease::EaseFunc m_ease;      ///< Easing function applied to scaling.

    public:
        /**
         * @brief Constructs a uniform ScaleTo action.
         * @param duration Action duration in seconds.
         * @param scale Uniform target scale factor.
         * @param ease Optional easing function curve.
         */
        ScaleTo(float duration, float scale, Ease::EaseFunc ease = Ease::Linear);

        /**
         * @brief Constructs a non-uniform ScaleTo action.
         * @param duration Action duration in seconds.
         * @param scaleX Target scale factor along X axis.
         * @param scaleY Target scale factor along Y axis.
         * @param ease Optional easing function curve.
         */
        ScaleTo(float duration, float scaleX, float scaleY, Ease::EaseFunc ease = Ease::Linear);

        /**
         * @brief Factory method for uniform scaling.
         * @param duration Action duration in seconds.
         * @param scale Uniform target scale factor.
         * @param ease Optional easing function curve.
         * @return Shared pointer to the created ScaleTo action.
         */
        static std::shared_ptr<ScaleTo> create(float duration, float scale, Ease::EaseFunc ease = Ease::Linear);

        /**
         * @brief Factory method for non-uniform scaling.
         * @param duration Action duration in seconds.
         * @param scaleX Target scale factor along X axis.
         * @param scaleY Target scale factor along Y axis.
         * @param ease Optional easing function curve.
         * @return Shared pointer to the created ScaleTo action.
         */
        static std::shared_ptr<ScaleTo> create(float duration, float scaleX, float scaleY, Ease::EaseFunc ease = Ease::Linear);

        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    /**
     * @brief Action that rotates a node by a relative angle delta over time.
     */
    class RotateBy : public FiniteTimeAction {
    private:
        float m_deltaAngle;         ///< Rotation delta angle in degrees or radians.
        float m_startAngle = 0.0f;  ///< Initial angle captured upon start.
        Ease::EaseFunc m_ease;      ///< Easing function applied to rotation.

    public:
        /**
         * @brief Constructs a RotateBy action.
         * @param duration Action duration in seconds.
         * @param deltaAngle Angle offset to rotate by.
         * @param ease Optional easing function curve.
         */
        RotateBy(float duration, float deltaAngle, Ease::EaseFunc ease = Ease::Linear);

        /**
         * @brief Factory method to create a shared pointer instance of RotateBy.
         * @param duration Action duration in seconds.
         * @param deltaAngle Angle offset to rotate by.
         * @param ease Optional easing function curve.
         * @return Shared pointer to the created RotateBy action.
         */
        static std::shared_ptr<RotateBy> create(float duration, float deltaAngle, Ease::EaseFunc ease = Ease::Linear);

        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    /**
     * @brief Action that fades a node's opacity from transparent to opaque over time.
     */
    class FadeIn : public FiniteTimeAction {
    private:
        float m_startOpacity = 0.0f; ///< Starting opacity value (fully transparent).
        float m_endOpacity = 1.0f;   ///< Ending opacity value (fully opaque).
        Ease::EaseFunc m_ease;       ///< Easing function applied to fade.

    public:
        /**
         * @brief Constructs a FadeIn action.
         * @param duration Action duration in seconds.
         * @param ease Optional easing function curve.
         */
        FadeIn(float duration, Ease::EaseFunc ease = Ease::Linear);

        /**
         * @brief Factory method to create a shared pointer instance of FadeIn.
         * @param duration Action duration in seconds.
         * @param ease Optional easing function curve.
         * @return Shared pointer to the created FadeIn action.
         */
        static std::shared_ptr<FadeIn> create(float duration, Ease::EaseFunc ease = Ease::Linear);

        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    /**
     * @brief Action that fades a node's opacity from opaque to transparent over time.
     */
    class FadeOut : public FiniteTimeAction {
    private:
        float m_startOpacity = 1.0f; ///< Starting opacity value (fully opaque).
        float m_endOpacity = 0.0f;   ///< Ending opacity value (fully transparent).
        Ease::EaseFunc m_ease;       ///< Easing function applied to fade.

    public:
        /**
         * @brief Constructs a FadeOut action.
         * @param duration Action duration in seconds.
         * @param ease Optional easing function curve.
         */
        FadeOut(float duration, Ease::EaseFunc ease = Ease::Linear);

        /**
         * @brief Factory method to create a shared pointer instance of FadeOut.
         * @param duration Action duration in seconds.
         * @param ease Optional easing function curve.
         * @return Shared pointer to the created FadeOut action.
         */
        static std::shared_ptr<FadeOut> create(float duration, Ease::EaseFunc ease = Ease::Linear);

        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    /**
     * @brief Action that introduces a time delay without performing any operations.
     */
    class DelayTime : public FiniteTimeAction {
    public:
        /**
         * @brief Constructs a DelayTime action.
         * @param duration Delay duration in seconds.
         */
        DelayTime(float duration) : FiniteTimeAction(duration) {}

        /**
         * @brief Factory method to create a shared pointer instance of DelayTime.
         * @param duration Delay duration in seconds.
         * @return Shared pointer to the created DelayTime action.
         */
        static std::shared_ptr<DelayTime> create(float duration);

        void step(float dt) override;
    };

    /**
     * @brief Action that executes an arbitrary callback function when reached.
     */
    class CallFunc : public FiniteTimeAction {
    private:
        std::function<void()> m_func; ///< Callback function to invoke.

    public:
        /**
         * @brief Constructs a CallFunc action.
         * @param func Std function callback to execute.
         */
        CallFunc(std::function<void()> func) : FiniteTimeAction(0.0f), m_func(func) {}

        /**
         * @brief Factory method to create a shared pointer instance of CallFunc.
         * @param func Std function callback to execute.
         * @return Shared pointer to the created CallFunc action.
         */
        static std::shared_ptr<CallFunc> create(std::function<void()> func);

        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    /**
     * @brief Action that triggers and plays a sound effect.
     */
    class PlaySound : public FiniteTimeAction {
    private:
        std::string m_soundPath; ///< File path to the sound asset.
        bool m_loop;             ///< Flag indicating if the sound loops.
        float m_volume;          ///< Playback volume level [0, 1].
        float m_pan;             ///< Stereo audio pan [-1, 1].
        float m_pitch;           ///< Playback pitch multiplier.
        std::string m_tag;       ///< Optional identifier tag for grouping.
        uint32_t m_soundId = 0;  ///< Assigned runtime audio instance ID.

    public:
        /**
         * @brief Constructs a PlaySound action.
         * @param soundPath Path to sound file.
         * @param loop True to loop playback, false otherwise.
         * @param volume Volume level [0, 1].
         * @param pan Stereo panning [-1, 1].
         * @param pitch Pitch modifier.
         * @param tag Optional group tag string.
         */
        PlaySound(std::string soundPath, bool loop = false, float volume = 1.0f, float pan = 0.0f, float pitch = 1.0f, std::string tag = "");

        /**
         * @brief Factory method to create a shared pointer instance of PlaySound.
         * @param soundPath Path to sound file.
         * @param loop True to loop playback, false otherwise.
         * @param volume Volume level [0, 1].
         * @param pan Stereo panning [-1, 1].
         * @param pitch Pitch modifier.
         * @param tag Optional group tag string.
         * @return Shared pointer to the created PlaySound action.
         */
        static std::shared_ptr<PlaySound> create(std::string soundPath, bool loop = false, float volume = 1.0f, float pan = 0.0f, float pitch = 1.0f, std::string tag = "");

        void startWithTarget(Node* target) override;
        void step(float dt) override;

        /**
         * @brief Gets the runtime sound instance ID.
         * @return Unique sound ID.
         */
        uint32_t getSoundId() const { return m_soundId; }
    };

    /**
     * @brief Action that stops active audio playback based on specific criteria.
     */
    class StopSound : public FiniteTimeAction {
    public:
        /** @brief Modes for stopping audio sources. */
        enum class Mode { ById, ByTag, ByPath, All };

    private:
        uint32_t m_soundId = 0;    ///< Specific sound ID to target.
        std::string m_identifier;  ///< Target string identifier (tag or path).
        Mode m_mode = Mode::All;   ///< Filtering mode for stopping sounds.

    public:
        /**
         * @brief Constructs a StopSound action targeting a specific sound ID.
         * @param soundId Target unique sound ID.
         */
        StopSound(uint32_t soundId);

        /**
         * @brief Constructs a StopSound action using an identifier and mode.
         * @param identifier Tag or path string.
         * @param mode Targeting mode (ByTag or ByPath).
         */
        StopSound(std::string identifier, Mode mode);

        /** @brief Constructs a StopSound action stopping all audio tracks. */
        StopSound();

        /**
         * @brief Factory method to stop a sound by its ID.
         * @param soundId Unique sound ID.
         * @return Shared pointer to the StopSound action.
         */
        static std::shared_ptr<StopSound> create(uint32_t soundId);

        /**
         * @brief Factory method to stop sounds matching a tag.
         * @param tag Group tag string.
         * @return Shared pointer to the StopSound action.
         */
        static std::shared_ptr<StopSound> createByTag(const std::string& tag);

        /**
         * @brief Factory method to stop sounds matching a file path.
         * @param path Target sound asset path.
         * @return Shared pointer to the StopSound action.
         */
        static std::shared_ptr<StopSound> createByPath(const std::string& path);

        /**
         * @brief Factory method to stop all currently active sounds.
         * @return Shared pointer to the StopSound action.
         */
        static std::shared_ptr<StopSound> createAll();

        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    /**
     * @brief Action container that executes a list of finite time actions sequentially one after another.
     */
    class Sequence : public FiniteTimeAction {
    private:
        std::vector<std::shared_ptr<FiniteTimeAction>> m_actions; ///< Ordered list of sequenced actions.
        size_t m_index = 0; ///< Current active action index in the sequence.

    public:
        /**
         * @brief Constructs a Sequence action container.
         * @param actions Vector of finite time actions.
         */
        Sequence(std::vector<std::shared_ptr<FiniteTimeAction>> actions);
        
        /**
         * @brief Variadic template factory method to create a sequence from multiple actions.
         * @tparam Args Parameter pack of action shared pointers.
         * @param args Action instances.
         * @return Shared pointer to the created Sequence action.
         */
        template<typename... Args>
        static std::shared_ptr<Sequence> create(Args... args) {
            std::vector<std::shared_ptr<FiniteTimeAction>> vec = { args... };
            return std::make_shared<Sequence>(vec);
        }

        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    /**
     * @brief Action container that executes multiple finite time actions concurrently.
     */
    class Spawn : public FiniteTimeAction {
    private:
        std::vector<std::shared_ptr<FiniteTimeAction>> m_actions; ///< List of concurrent actions.

    public:
        /**
         * @brief Constructs a Spawn action container.
         * @param actions Vector of finite time actions to run in parallel.
         */
        Spawn(std::vector<std::shared_ptr<FiniteTimeAction>> actions);

        /**
         * @brief Variadic template factory method to create a spawn container from multiple actions.
         * @tparam Args Parameter pack of action shared pointers.
         * @param args Action instances.
         * @return Shared pointer to the created Spawn action.
         */
        template<typename... Args>
        static std::shared_ptr<Spawn> create(Args... args) {
            std::vector<std::shared_ptr<FiniteTimeAction>> vec = { args... };
            return std::make_shared<Spawn>(vec);
        }

        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

    /**
     * @brief Action wrapper that repeats an inner finite-time action indefinitely.
     */
    class RepeatForever : public Action {
    private:
        std::shared_ptr<FiniteTimeAction> m_innerAction; ///< The action to loop continuously.

    public:
        /**
         * @brief Constructs a RepeatForever wrapper action.
         * @param action Finite time action to repeat.
         */
        RepeatForever(std::shared_ptr<FiniteTimeAction> action);

        /**
         * @brief Factory method to create a shared pointer instance of RepeatForever.
         * @param action Finite time action to repeat.
         * @return Shared pointer to the created RepeatForever action.
         */
        static std::shared_ptr<RepeatForever> create(std::shared_ptr<FiniteTimeAction> action);

        void startWithTarget(Node* target) override;
        void step(float dt) override;
    };

}