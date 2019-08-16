#pragma once

#include <chrono>
#include <string>

namespace fui {

class RenderContext;

class PerfGraph {
public:
  enum Mode {
    FPS   = 0x01,
    TIME  = 0x02,
    GRAPH = 0x04,
    ALL   = FPS | TIME | GRAPH,
  };

  PerfGraph(const std::string& name, Mode mode = Mode::ALL);

  /// update the FPS
  void update(const std::chrono::microseconds& frameTime);

  /// draw the graph
  void draw(RenderContext& renderContext);

private:
  constexpr static size_t MAX_HISTORY_COUNT = 100;
	std::string _name;
	Mode _mode;
	float _fps[MAX_HISTORY_COUNT];
	int _lastest;
};

} // namespace fui
