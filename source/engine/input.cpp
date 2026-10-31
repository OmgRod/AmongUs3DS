#include <engine/input.hpp>
#include <cstring>

namespace engine {

    std::string Input::getTextInput(const std::string& title, const std::string& initialText, size_t maxLength) {
        SwkbdState swkbd;
        char outBuffer[512] = {0};
        if (initialText.length() < sizeof(outBuffer)) {
            std::strcpy(outBuffer, initialText.c_str());
        }

        swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
        swkbdSetValidation(&swkbd, SWKBD_ANYTHING, 0, 0);
        if (!title.empty()) {
            swkbdSetHintText(&swkbd, title.c_str());
        }
        swkbdSetInitialText(&swkbd, outBuffer);

        SwkbdButton button = swkbdInputText(&swkbd, outBuffer, sizeof(outBuffer));
        if (button != SWKBD_BUTTON_CONFIRM) {
            return "";
        }
        return std::string(outBuffer);
    }

}
