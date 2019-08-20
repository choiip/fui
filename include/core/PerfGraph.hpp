#pragma once

#include <chrono>
#include "widget/Widget.hpp"

namespace fui {

class RenderContext;

class PerfGraph : public Widget {
public:
  enum class Mode {
    FPS   = 0x01,
    TIME  = 0x02,
    GRAPH = 0x04,
    ALL   = FPS | TIME | GRAPH,
  };

  PerfGraph(const std::string& name, Mode mode = Mode::ALL);

  virtual ~PerfGraph();

  /// update the FPS
  void update(const std::chrono::microseconds& frameTime);

  /// draw the graph
  virtual void draw(RenderContext& renderContext) override;

protected:
  FUI_WIDGET_PROPERTY(std::string, name, "");

  FUI_WIDGET_PROPERTY(Mode, mode, Mode::ALL);

private:
  constexpr static size_t MAX_HISTORY_COUNT = 100;

	float _fps[MAX_HISTORY_COUNT];
	int _lastest;
};

} // namespace fui
