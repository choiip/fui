// CC0 Public Domain: https://codereview.stackexchange.com/questions/114532/simple-stopwatch-class

#pragma once

#include <chrono>

namespace fui
{
template
<
  typename TimeUnit = std::chrono::milliseconds,
  typename Clock = std::chrono::high_resolution_clock
>
class Stopwatch
{
public:
  explicit Stopwatch(bool startStopwatch = false) noexcept;

  void start() noexcept;
  void stop() noexcept;

  TimeUnit elapsed() noexcept;
  void reset() noexcept;

  template <typename F, typename... FArgs>
  inline static TimeUnit measure( F&&, FArgs&&... );

private:
  bool stopped_;
  std::chrono::time_point<Clock> stop_;

  bool started_;
  std::chrono::time_point<Clock> start_;
};

template <typename TimeUnit, typename Clock>
inline Stopwatch<TimeUnit, Clock>::Stopwatch(bool startStopwatch) noexcept :
  stopped_{ false },
  stop_{ TimeUnit{ 0 } },
  started_{ startStopwatch },
  start_{ startStopwatch ? Clock::now() : stop_ }
{
}

template <typename TimeUnit, typename Clock>
inline void Stopwatch<TimeUnit, Clock>::start() noexcept {
  if ( started_ ) {
      return;
      //throw std::logic_error( "stopwatch: already called start()" );
  }

  start_ = Clock::now();
  started_ = true;
}

template <typename TimeUnit, typename Clock>
inline void Stopwatch<TimeUnit, Clock>::stop() noexcept {
  if ( !started_ ) {
    return;
    //throw std::logic_error( "stopwatch: called stop() before start()" );
  }

  stop_ = Clock::now();
  stopped_ = true;
}

template <typename TimeUnit, typename Clock>
inline TimeUnit Stopwatch<TimeUnit, Clock>::elapsed() noexcept {
  if ( !started_ ) {
    return TimeUnit{ 0 };
  }

  if ( stopped_ ) {
    return std::chrono::duration_cast<TimeUnit>( stop_ - start_ );
  }

  return std::chrono::duration_cast<TimeUnit>( Clock::now() - start_ );
}

template <typename TimeUnit, typename Clock>
inline void
Stopwatch<TimeUnit, Clock>::reset() noexcept {
  started_ = false;
  stop_ = start_;
  stopped_ = false;
}

template <typename TimeUnit, typename Clock>
template <typename F, typename... FArgs>
inline TimeUnit Stopwatch<TimeUnit, Clock>::measure( F&& f, FArgs&&... f_args ) {
  auto start_time = Clock::now();
  std::forward<F>( f )( std::forward<FArgs>( f_args )... );
  auto stop_time = Clock::now();

  return std::chrono::duration_cast<TimeUnit>( stop_time - start_time );
}

} // namespace fui
