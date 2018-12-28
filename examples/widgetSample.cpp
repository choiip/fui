#include <memory>
#include <GL/glad.h>
#include <fui.hpp>
using namespace fui;

class WidgetSample : public ApplicationContext {
public:
  WidgetSample(RenderWindow* renderWindow)
  : _renderWindow(renderWindow) {}

protected:
  virtual Status onEnter() override {
    // load fonts

    // Gui
    int x = 10, y = 10;
    _renderWindow->addChild<Label>("1st Label")->textColor({0.7f, 0.0f, 0.3f, 1.f})->position({x, y})->size({150, 28});
    y += 40;
    _renderWindow->addChild<Label>("2nd Label")->textColor({0.3f, 0.7f, 0.0f, 1.f})->position({x, y})->size({150, 28});
    y += 40;
    _renderWindow->addChild<Label>("3rd Label")->textColor({0.f, 0.3f, 0.7f, 1.f})->position({x, y})->size({150, 28});
    y += 40;
    _renderWindow->addChild<Label>("Disabled Label")
        ->textColor({0.f, 0.3f, 0.7f, 1.f})
        ->position({x, y})
        ->size({150, 28})
        ->enabled(false);
    y += 40;

    x = 210, y = 10;
    _renderWindow->addChild<Button>("1st Button")
        ->icon(ENTYPO_ICON_CLOUD)
        ->backgroundColor({0.7f, 0.0f, 0.3f, 1.f})
        ->position({x, y})
        ->size({150, 28});
    y += 40;
    _renderWindow->addChild<Button>("2nd Button")
        ->icon(ENTYPO_ICON_NEWS)
        ->backgroundColor({0.3f, 0.7f, 0.0f, 1.f})
        ->position({x, y})
        ->size({150, 28});
    y += 40;
    _renderWindow->addChild<Button>("3rd Button")
        ->icon(ENTYPO_ICON_SIGNAL)
        ->backgroundColor({0.f, 0.3f, 0.7f, 1.f})
        ->position({x, y})
        ->size({150, 28});
    y += 40;
    _renderWindow->addChild<Button>("Disabled Button")
        ->icon(ENTYPO_ICON_STAR)
        ->backgroundColor({0.f, 0.3f, 0.7f, 1.f})
        ->position({x, y})
        ->size({150, 28})
        ->enabled(false);
    y += 40;

    // Events
    _renderWindow->onKey([this](Key key, ButtonAction action, Modifier mods) {
      if (key == Key::KEY_ESCAPE) {
        LOGD << "Window closing...";
        _renderWindow->close();
      }
    });

    // Show window
    _renderWindow->show();
    return Status::OK;
  }

  void onDraw() override {
    int winWidth, winHeight, fbWidth, fbHeight;
    _renderWindow->getWindowSize(winWidth, winHeight);
    _renderWindow->getDrawableSize(fbWidth, fbHeight);
    // Calculate pixel ration for hi-dpi devices.
    auto pxRatio = (float)fbWidth / (float)winWidth;

    glViewport(0, 0, fbWidth, fbHeight);
    glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    _renderWindow->drawGui();

    _renderWindow->swapBuffer();
  }

private:
  RenderWindow* _renderWindow;
};

int main() {
  Logger logger;
  LOGD << "Sample start";

  GlfwWindowManager windowManager;
  auto graphicsProfile = windowManager.createGraphicsProfile(GraphicsAPI::OPENGL, 3, 2);
  if (!graphicsProfile)
    return -1;

  auto window = windowManager.createWindow(1000, 600, *graphicsProfile);
  delete graphicsProfile;

  if (!window)
    return -1;

  std::unique_ptr<ApplicationContext> app(new WidgetSample(window));
  app->run(windowManager);
}
