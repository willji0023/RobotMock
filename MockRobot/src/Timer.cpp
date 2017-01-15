/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008-2016. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Timer.hpp"

#include <chrono>

/**
 * @brief Gives real-time clock system time with nanosecond resolution
 * @return The time, just in case you want the robot to start autonomous at 8pm
 *         on Saturday.
 */
double GetTime() {
  using namespace std::chrono;
  return duration_cast<duration<double>>(system_clock::now().time_since_epoch())
      .count();
}

/**
 * Create a new timer object.
 *
 * Create a new timer object and reset the time to zero. The timer is initially
 * not running and
 * must be started.
 */
Timer::Timer() {
  // Creates a semaphore to control access to critical regions.
  // Initially 'open'
  Reset();
}

/**
 * Get the current time from the timer. If the clock is running it is derived
 * from the current system clock the start time stored in the timer class. If
 * the clock is not running, then return the time when it was last stopped.
 *
 * @return Current time value for this timer in seconds
 */
double Timer::Get() const {
  double result;
  double currentTime = GetTime();

  std::lock_guard<std::mutex> sync(m_mutex);
  if (m_running) {
    result = (currentTime - m_startTime) + m_accumulatedTime;
  } else {
    result = m_accumulatedTime;
  }

  return result;
}

/**
 * Reset the timer by setting the time to 0.
 *
 * Make the timer startTime the current time so new requests will be relative to
 * now.
 */
void Timer::Reset() {
  std::lock_guard<std::mutex> sync(m_mutex);
  m_accumulatedTime = 0;
  m_startTime = GetTime();
}

/**
 * Start the timer running.
 *
 * Just set the running flag to true indicating that all time requests should be
 * relative to the system clock.
 */
void Timer::Start() {
  std::lock_guard<std::mutex> sync(m_mutex);
  if (!m_running) {
    m_startTime = GetTime();
    m_running = true;
  }
}

/**
 * Stop the timer.
 *
 * This computes the time as of now and clears the running flag, causing all
 * subsequent time requests to be read from the accumulated time rather than
 * looking at the system clock.
 */
void Timer::Stop() {
  double temp = Get();

  std::lock_guard<std::mutex> sync(m_mutex);
  if (m_running) {
    m_accumulatedTime = temp;
    m_running = false;
  }
}

/**
 * Check if the period specified has passed and if it has, advance the start
 * time by that period. This is useful to decide if it's time to do periodic
 * work without drifting later by the time it took to get around to checking.
 *
 * @param period The period to check for (in seconds).
 * @return True if the period has passed.
 */
bool Timer::HasPeriodPassed(double period) {
  if (Get() > period) {
    std::lock_guard<std::mutex> sync(m_mutex);
    // Advance the start time by the period.
    m_startTime += period;
    // Don't set it to the current time... we want to avoid drift.
    return true;
  }
  return false;
}
