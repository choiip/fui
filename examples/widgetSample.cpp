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
    auto renderContext = _renderWindow->renderContext();
#ifdef __EMSCRIPTEN__
    // load fonts
    auto standardFontId = renderContext->loadFont("sans", "examples/assets/fonts/Roboto-Regular.ttf");
    auto boldFontId = renderContext->loadFont("sans-bold", "examples/assets/fonts/Roboto-Bold.ttf");
    static auto emscriptenStyle = std::make_shared<WidgetStyle>(*renderContext);
    emscriptenStyle->fontStandard = standardFontId;
    emscriptenStyle->fontBold = boldFontId;
    _renderWindow->style(emscriptenStyle);
#endif
    auto vg = renderContext->vg();

    // load image
    char file[128];
    snprintf(file, 128, "examples/assets/images/image1.jpg");
    auto pictureId = renderContext->loadImage(file);
    if (pictureId == 0) {
      LOGE << "Could not load " << file;
    }

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

    _renderWindow->addChild<Window>("Other Window")->position({x, y})->size({320, 240});
    y += 40;

    x = 210, y = 10;
    _renderWindow->addChild<Button>("Normal Button")
        ->icon(ENTYPO_ICON_CLOUD)
        ->backgroundColor({0.7f, 0.0f, 0.3f, 1.f})
        ->position({x, y})
        ->size({150, 28});
    y += 40;
    _renderWindow->addChild<Button>("Toggle Button")
        ->type(Button::Type::TOGGLE)
        ->icon(ENTYPO_ICON_NEWS)
        ->backgroundColor({0.3f, 0.7f, 0.0f, 1.f})
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

    (_progressBar = _renderWindow->addChild<ProgressBar>())->maxValue(360)->position({x, y})->size({150, 28});
    y += 40;

    x = 410, y = 210;
    Window* windowWidget = nullptr;
    (windowWidget = _renderWindow->addChild<Window>("1st Window"))->position({x, y})->size({320, 240});
    {
      int lx = 10, ly = 10;
      windowWidget->addChild<Button>("Normal Button")
          ->icon(ENTYPO_ICON_CLOUD)
          ->backgroundColor({0.7f, 0.0f, 0.3f, 1.f})
          ->position({lx, ly})
          ->size({150, 28});
      ly += 40;
      windowWidget->addChild<Button>("Toggle Button")
          ->type(Button::Type::TOGGLE)
          ->icon(ENTYPO_ICON_NEWS)
          ->backgroundColor({0.3f, 0.7f, 0.0f, 1.f})
          ->position({lx, ly})
          ->size({150, 28});
      ly += 40;
      (_pictureBox = windowWidget->addChild<PictureBox>())
          ->orientation(0.f)
          ->picture(pictureId, vg)
          ->position({lx, ly})
          ->size({256, 128});
    }
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
    if (_progress >= _progressBar->maxValue())
      _progress = 0;
    else
      ++_progress;

    auto text = std::to_string(_progress);
    _progressBar->value(_progress)->text(text);
    _pictureBox->orientation((float)_progress)->fit();

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

  ProgressBar* _progressBar;
  PictureBox* _pictureBox;
  int _progress = 0;
};

int main() {
  Logger logger;
  LOGD << "Sample start";

  auto graphicAPI = GraphicsAPI::OPENGL;
#ifdef __EMSCRIPTEN__
  graphicAPI = GraphicsAPI::OPENGL_ES;
#endif
  GlfwWindowManager windowManager;
  auto graphicsProfile = windowManager.createGraphicsProfile(graphicAPI, 3, 2);
  if (!graphicsProfile)
    return -1;

  auto window = windowManager.createWindow(1000, 600, *graphicsProfile);
  delete graphicsProfile;

  if (!window)
    return -1;

  std::unique_ptr<ApplicationContext> app(new WidgetSample(window));
  app->run(windowManager);
}
