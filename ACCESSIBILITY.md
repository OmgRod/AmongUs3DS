# Accessibility
<!-- Include a brief statement describing why accessibility matters to your project, who should be able to use and contribute to it, and what this document covers, such as your accessibility commitments, contributor expectations, and issue-reporting process. Write for people who may read this after encountering a barrier. Use plain language and describe user experiences rather than relying on legal language, technical jargon, or standards codes. -->

Accessibility is a core priority for **Among Us: 3DS Edition**. We believe that gaming should be for everyone, and we are committed to making this Homebrew port as playable and navigable as possible for players with visual, motor, or cognitive disabilities.

This document explains our accessibility goals for the 3DS hardware, how contributors can help us uphold them, and how players can report accessibility barriers they encounter while playing.

## Priorities
<!-- Describe the outcomes the project prioritizes and how it works toward them. If you name a WCAG level as an accessibility target, make clear that it is an aspirational goal rather than a verified conformance claim. Only claim conformance after an evaluation, and identify the evaluated scope, date, method, and evaluator. Also describe the areas you prioritize, such as keyboard, screen reader, content, or language support. -->

Due to the fixed hardware constraints of the Nintendo 3DS family (lack of native screen readers, low resolution), our accessibility targets focus heavily on visual clarity and flexible input methods.

- **Colourblind Support** - Since colour is a primary identifier in *Among Us*, we strive to maintain the original game's colourblind text labels for tasks (like the Wires task) and player cosmetics. 
- **Flexible Controls** - The game must be fully playable using physical buttons (D-Pad/Circle Pad, A/B/X/Y, shoulder buttons) *or* the touchscreen. We do not force players to use the touchscreen if physical buttons can accomplish the same action, accommodating different motor capabilities.
- **Readable UI** - Text must scale cleanly without becoming pixelated or illegible on the 240p screens. We use high-contrast text and distinct visual outlines for UI elements.
- **Audio Cues** - Critical gameplay events (reactor meltdowns, meeting calls, task completions) must have distinct audio cues paired with visual screen flashing or clear icons, ensuring players who are hard of hearing or have low vision receive the information.

## Contributor Expectations
<!-- Describe the accessibility guardrails that apply to contributions, including relevant testing, documentation, and continuous integration checks. Name specific tools only when the project uses them, and explain what evidence contributors should include with user-facing changes. -->

If you are contributing code, UI, or assets, please follow these guardrails so we don't regress accessibility:

- **Testing**
  - For UI or menu changes, test navigation using *both* physical buttons and the touchscreen on real hardware or an emulator (Citra/Azahar). Ensure no UI element traps the user.
  - Verify that interactive elements have a clear, visible selected state (e.g., a highlighted border or colour change) when navigated via the D-Pad.
- **Visual Design**
  - Do not use colour as the *only* way to convey meaning. Ensure tasks or status indicators have accompanying shapes, symbols, or text labels.
  - Maintain high contrast for all in-game text against its background. Do not shrink fonts below the established baseline readability size.
- **Input Mapping**
  - If you add a new action or mini-game task, ensure it maps logically to a physical button press. Avoid requiring rapid, complex touchscreen gestures if a simple button press can serve as an alternative.

## Reporting Accessibility Issues
<!-- Provide a direct, accessible way to report a barrier or request help. Invite useful context such as the affected task, URL, observed behavior, browser, operating system, and assistive technology. Make screenshots or recordings optional and do not require disability disclosure. -->

If you run into an accessibility barrier that prevents you from playing or enjoying the game, please let us know - we treat accessibility reports as expertise, not complaints.

1. **Open an issue** on our GitHub repository and use the `accessibility` label.
2. Include, when possible:
   - What you were trying to do (e.g., "Trying to complete the Simon Says task").
   - What went wrong (e.g., "The colours are indistinguishable for deuteranomaly, and there are no shape indicators").
   - The hardware you are playing on (Original 3DS, 2DS, New 3DS XL, or emulator).
   - A screen recording or screenshot, if you're comfortable sharing one.
   - Severity, using the defined taxonomy below.

### Severity
<!-- If your project uses severity levels, define them and give practical examples based on how strongly a barrier affects a user's ability to complete a task. Keep labels consistent with your issue template and resolution targets if those processes exist. Maintainers can assign or confirm severity during triage; reporters do not need to. -->

- **Critical:** Prevents you from completing a core gameplay action (for example, a mandatory task requires a touchscreen gesture you cannot perform, and there is no button alternative).
- **Serious:** Significant difficulty, but a workaround exists (e.g., UI text is very hard to read, but technically decipherable).
- **Moderate:** Annoyance or inconsistent experience (e.g., a menu can only be navigated by the touchscreen, but gameplay works fine with buttons).
- **Minor:** Minor visual or audio issue with minimal impact on playability.

### How We Respond
<!-- Explain what reporters can expect after submitting an accessibility issue, including acknowledgement, status updates, workarounds, expected timelines, and opportunities to verify a fix. Use commitments your project can consistently uphold. -->

- We will acknowledge your report promptly and respectfully.
- We will not require you to disclose a diagnosis or other personal information.
- We will explain the next steps and any hardware limitations that might affect the fix (e.g., 3DS RAM limits).
- We will provide updates when the status changes.
- We may ask you to test a preview build (`.3dsx` or `.cia`) to confirm the fix works for your needs before closing the issue.

## Supported Environments
<!-- List the platforms, devices, browsers, input methods, and assistive technologies the project supports or has tested. Include versions where useful, note partial support, and avoid implying support for combinations that have not been evaluated. -->

This project is built specifically for the Nintendo 3DS ecosystem and is tested across:
- **Original Nintendo 3DS / 3DS XL**
- **Nintendo 2DS**
- **New Nintendo 3DS / New 3DS XL / New 2DS XL**
- **PC Emulators:** Citra, Azahar

**Note on Assistive Tech:** Because the Nintendo 3DS operating system lacks native screen reader support or system-level magnifier tools, this game does not support external assistive software. Our accessibility efforts are entirely localised within the game's own visual, audio, and input design.

## Known Limitations
<!-- Describe known barriers in terms of the affected user experience, such as "videos do not have captions," rather than standards codes. Include available workarounds or equivalent access and link to tracked issues. If no limitations are currently documented, describe what has been tested instead of claiming that no barriers exist. -->

- **Text Chat:** The in-game text chat relies on the native 3DS software keyboard, which requires the touchscreen and stylus to type efficiently. There is currently no voice-to-text alternative.
- **Screen Size:** On original standard-sized 3DS and 2DS models, some UI elements may appear cramped due to the physical 3.0" and 3.53" screen sizes. We recommend playing on XL models for the best visual clarity.

## Feedback
<!-- Explain how users and contributors can suggest improvements to this statement or the project's accessibility practices. Direct active accessibility barriers to the reporting process above. -->

Accessibility is an ongoing practice. If you have suggestions for improving this statement, or ideas for new accessibility features (like custom colourblind filters or remappable controls), please open an issue or a pull request. Thank you for helping make *Among Us: 3DS Edition* playable by everyone.
