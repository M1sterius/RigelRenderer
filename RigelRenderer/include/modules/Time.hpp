#pragma once

#include "Internal.hpp"

namespace rgr
{
    class Time
    {
    public:
        static double GetTime();
        static float GetTimeF();
        static double GetDeltaTime();
        static float GetDeltaTimeF();
    private:
        static double m_Time;
        static double m_OldTime;
        static double m_DeltaTime;
    INTERNAL:
        static void CalcTime(const double timePassed);
    };
}