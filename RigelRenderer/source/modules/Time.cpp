#include "Time.hpp"

#include <chrono>

namespace rgr
{
    double Time::m_Time = 0.0;
    double Time::m_OldTime = 0.0;
    double Time::m_DeltaTime = 0.0;

    double Time::GetTime()
    {
        return m_Time;
    }

    float Time::GetTimeF()
    {
        return static_cast<float>(m_Time);
    }

    double Time::GetDeltaTime()
    {
        return m_DeltaTime;
    }

    float Time::GetDeltaTimeF()
    {
        return static_cast<float>(m_DeltaTime);
    }

    void Time::CalcTime(const double timePassed)
    {
        m_Time = timePassed;
        m_DeltaTime = m_Time - m_OldTime;
        m_OldTime = m_Time;
    }

    size_t Time::GetFPS()
    {
        return static_cast<size_t>(1 / GetDeltaTime());
    }

    void Time::BusyWaitSleep(const double seconds)
    {
        const auto beginTime = std::chrono::high_resolution_clock::now();

        while (true)
        {
            const std::chrono::duration<double> passed = std::chrono::high_resolution_clock::now() - beginTime;
            if (passed.count() >= seconds) break;
        }
    }
}
