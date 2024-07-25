#pragma once

namespace rgr
{
    class Cursor
    {
    public:
        enum class CURSOR_STATE
        {
            NORMAL,
            HIDDEN,
            LOCKED,
            DISABLED
        };

        static void SetCursorState(const CURSOR_STATE state);
        static CURSOR_STATE GetCursorState();
    private:
        static CURSOR_STATE m_CursorState;
    };
}
