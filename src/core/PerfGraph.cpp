#include "core/PerfGraph.hpp"
#include "core/RenderContext.hpp"
#include "nanovg/nanovg.h"
#include "widget/WidgetStyle.hpp"
#include "event/EventEnum.hpp"

namespace fui {

PerfGraph::PerfGraph(const std::string& name, Mode mode)
: Widget(nullptr)
, _name(name)
, _mode(mode)
, _lastest(-1) {
	std::fill(std::begin(_fps), std::end(_fps), 0.f);
}

PerfGraph::~PerfGraph() = default;
  
void PerfGraph::update(const std::chrono::microseconds& frameTime) {
	_lastest = (_lastest+1) % MAX_HISTORY_COUNT;
	_fps[_lastest] = 1000.f * 1000.f / frameTime.count();
}

void PerfGraph::draw(RenderContext& renderContext) {
	if (_lastest < 0) return;

  auto vg = renderContext.vg();

  const auto& prefGraphStyle = style();
  auto x = _position.x;
  auto y = _position.y;
  auto w = _size.x;
  auto h = _size.y;
	char str[64];

	nvgBeginPath(vg);
	nvgRect(vg, x,y, w,h);
	nvgFillColor(vg, nvgRGBA(0,0,0,128));
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgMoveTo(vg, x, y+h);

	if (hasFlags(_mode, Mode::GRAPH)) {
		if (hasFlags(_mode, Mode::FPS)) {
			for (auto i = 0; i < MAX_HISTORY_COUNT; i++) {
				float v = _fps[(_lastest+i) % MAX_HISTORY_COUNT];	
				float vx, vy;
				if (v > 320.0f) v = 320.0f;
				vx = x + ((float)i/(MAX_HISTORY_COUNT-1)) * w;
				vy = y + h - ((v / 320.0f) * h);
				nvgLineTo(vg, vx, vy);
			}
		} else if (hasFlags(_mode, Mode::TIME)) {
			for (auto i = 0; i < MAX_HISTORY_COUNT; i++) {
				auto fps = _fps[(_lastest+i) % MAX_HISTORY_COUNT];
				float v = 1000.f / fps;
				float vx, vy;
				if (v > 100.0f) v = 100.0f;
				vx = x + ((float)i/(MAX_HISTORY_COUNT-1)) * w;
				vy = y + h - ((v / 100.0f) * h);
				nvgLineTo(vg, vx, vy);
			}
		}
	}
	nvgLineTo(vg, x+w, y+h);
	nvgFillColor(vg, nvgRGBA(255,192,0,128));
	nvgFill(vg);

	nvgFontFaceId(vg, prefGraphStyle.fontStandard);

	if (!_name.empty()) {
		nvgFontSize(vg, 14.0f);
		nvgTextAlign(vg, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
		nvgFillColor(vg, nvgRGBA(240,240,240,192));
		nvgText(vg, x+3,y+1, _name.c_str(), NULL);
	}

	if (hasFlags(_mode, Mode::FPS)) {
		nvgFontSize(vg, 18.0f);
		nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_TOP);
		nvgFillColor(vg, nvgRGBA(240,240,240,255));
		sprintf(str, "%.1f FPS", _fps[_lastest]);
		nvgText(vg, x+w-3,y+1, str, NULL);
	}

	if (hasFlags(_mode, Mode::TIME)) {
		nvgFontSize(vg, 15.0f);
		nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_BOTTOM);
		nvgFillColor(vg, nvgRGBA(240,240,240,160));
		sprintf(str, "%.2f ms", 1000.f/_fps[_lastest]);
		nvgText(vg, x+w-3,y+h-1, str, NULL);
	}
}

} // namespace fui
