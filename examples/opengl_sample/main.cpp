#include <memory>
#include <GL/header.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fui.hpp>
using namespace fui;

#include "GL/GLPrimitive.h"
#include "GL/GLProgram.h"

class WidgetSample : public ApplicationContext {
public:
  explicit WidgetSample(RenderWindow* renderWindow)
  : _renderWindow(renderWindow)
  , _progressBar(nullptr)
  , _pictureBox(nullptr)
  {}

protected:
  virtual Status onEnter() override {
    auto renderContext = _renderWindow->renderContext<OpenGLContext>();
    // LOGD << glGetString(GL_VERSION);

    auto vg = renderContext->vg();

    // Gui
    int x = 10, y = 10;

    Window* canvasWindow = nullptr;
    (canvasWindow = _renderWindow->addChild<Window>("Canvas Window"))->position({x, y})->size({660, 500});
    canvasWindow->addChild<GLCanvas>()
        ->framebuffer(960, 720, 0, vg)
        ->drawFunction([this](int width, int height) {
          glViewport(0, 0, width, height);
          glClearColor(.6f, .6f, .6f, 0.f);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
          glEnable(GL_DEPTH_TEST);
          glDisable(GL_BLEND);

          if (_programId == 0) {
            auto renderContext = _renderWindow->renderContext<OpenGLContext>();
            // load shaders
            auto vertSource = renderContext->loadVertexShader("examples/assets/shaders/sample.vert");
            auto fragSource = renderContext->loadFragmentShader("examples/assets/shaders/sample.frag");
            _programId = createProgram("sample", vertSource.c_str(), fragSource.c_str());
          }
          if (_cube == nullptr && _programId != 0) { _cube = createCube(_programId); }

          float halfWidth = width * .5f;
          float halfHeight = height * .5f;
          // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
          // glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
          // Or, for an ortho camera :
          glm::mat4 projection =
              glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, 0.0f, 1000.0f); // In world coordinates

          // Camera matrix
          glm::mat4 view = glm::lookAt(glm::vec3(400, 300, 300), // Camera is at (4,3,3), in World Space
                                       glm::vec3(0, 0, 0),       // and looks at the origin
                                       glm::vec3(0, 1, 0)        // Head is up (set to 0,-1,0 to look upside-down)
          );

          glm::mat4 model = glm::mat4(1.0f);

          model = glm::rotate(model, glm::radians((float)_progress), glm::vec3(0, 1, 0));
          model = glm::scale(model, glm::vec3(100, 100, 100));
          glm::mat4 mvp = projection * view * model;

          drawCube(_cube, &mvp[0][0]);
        })
        ->position({10, 10})
        ->size({640, 480});
    y += 40;

    x = 680, y = 10;
    Window* widgetWindow = nullptr;
    (widgetWindow = _renderWindow->addChild<Window>("Widget Window"))->position({x, y})->size({330, 500});
    {
      int lx = 10, ly = 10;
      widgetWindow->addChild<Label>("1st Label")
          ->textColor({0.7f, 0.0f, 0.3f, 1.f})
          ->position({lx, ly})
          ->size({150, 28});
      ly += 40;
      widgetWindow->addChild<Label>("2nd Label")
          ->textColor({0.3f, 0.7f, 0.0f, 1.f})
          ->position({lx, ly})
          ->size({150, 28});
      ly += 40;
      widgetWindow->addChild<Label>("3rd Label")
          ->textColor({0.f, 0.3f, 0.7f, 1.f})
          ->position({lx, ly})
          ->size({150, 28});
      ly += 40;
      widgetWindow->addChild<Label>("Disabled Label")
          ->textColor({0.f, 0.3f, 0.7f, 1.f})
          ->position({lx, ly})
          ->size({150, 28})
          ->enabled(false);
      lx += 160;
      ly = 10;

      widgetWindow->addChild<Button>("Normal Button")
          ->icon(ENTYPO_ICON_CLOUD)
          ->backgroundColor({0.7f, 0.0f, 0.3f, 1.f})
          ->position({lx, ly})
          ->size({150, 28});
      ly += 40;
      widgetWindow->addChild<Button>("Toggle Button")
          ->type(Button::Type::TOGGLE)
          ->icon(ENTYPO_ICON_NEWS)
          ->backgroundColor({0.3f, 0.7f, 0.0f, 1.f})
          ->onToggled([canvasWindow](bool v) { canvasWindow->visible(!v); })
          ->position({lx, ly})
          ->size({150, 28});
      ly += 40;
      widgetWindow->addChild<Slider>()
          ->onValueChanged([](float newValue) { LOGD << "Slider value=" << newValue; })
          ->position({lx, ly})
          ->size({150, 28});
      ly += 40;
      (_progressBar = widgetWindow->addChild<ProgressBar>())->maxValue(360)->position({lx, ly})->size({150, 28});
      ly += 40;

      lx = 10;
      (_pictureBox = widgetWindow->addChild<PictureBox>())
          ->orientation(0.f)
          ->picture("examples/assets/images/image1.jpg", vg)
          ->position({lx, ly})
          ->size({256, 128});
    }

    // fps setup
    _perfGraph = std::make_shared<PerfGraph>("FPS");
    _perfGraph->style(std::make_shared<WidgetStyle>(*renderContext))
        ->size({80, 40})
        ->position({_renderWindow->size().x - _perfGraph->size().x - 10, 10});
    _stopwatch.start();
    _frameCount = 0;
    _renderWindow->perfGraph(_perfGraph);

    // Events
    _renderWindow->onKey([this](Key key, ButtonAction action, Modifier mods) {
      if (key == Key::KEY_ESCAPE) {
        LOGD << "Window closing...";
        _renderWindow->close();
      }
    });

    _renderWindow->onResize([this](int width, int height) {
      _perfGraph->position({_renderWindow->size().x - _perfGraph->size().x - 10, 10});
    });
    canvasWindow->focused(true);

    // Show window
    _renderWindow->show();
    return Status::OK;
  }

  void onDraw() override {
    if (++_progress > _progressBar->maxValue()) _progress = 0;

    auto text = std::to_string(_progress);
    _progressBar->value(_progress)->text(text);
    _pictureBox->orientation((float)_progress)->fit();

    Recti renderArea = {0, 0, 0, 0};
    Color clearColor = {0.3f, 0.3f, 0.32f, 1.0f};
    _renderWindow->getDrawableSize(renderArea.w, renderArea.h);
    auto renderContext = _renderWindow->renderContext<OpenGLContext>();
    renderContext->preDraw(renderArea, &clearColor);

    _renderWindow->drawGui();

    renderContext->postDraw();

    _renderWindow->swapBuffer();

    auto timeElapsed = _stopwatch.elapsed();
    _frameCount++;
    if (timeElapsed > std::chrono::milliseconds(500)) {
      _perfGraph->update(timeElapsed / _frameCount);
      _stopwatch.reset();
      _stopwatch.start();
      _frameCount = 0;
    }
  }

private:
  RenderWindow* _renderWindow;

  ProgressBar* _progressBar;
  PictureBox* _pictureBox;
  int _progress = 0;

  std::shared_ptr<PerfGraph> _perfGraph;
  Stopwatch<std::chrono::microseconds> _stopwatch;
  size_t _frameCount = 0;

  struct cube* _cube = nullptr;
  unsigned int _programId = 0;
};

int main() {
  Logger logger;
  LOGD << "Sample start";

#ifdef __EMSCRIPTEN__
  // setup fonts
  setupWebFonts({{
                     "examples/assets/fonts/Roboto-Regular.ttf", // path
                     "",                                         // postscriptName
                     "",                                         // family
                     "Regular",                                  // style
                     "zh-hk",                                    // lang
                     FontWeightNormal,                           // weight
                     FontWidthNormal,                            // width
                     false,                                      // italic
                     false,                                      // monospace
                 },
                 {
                     "examples/assets/fonts/Roboto-Bold.ttf", // path
                     "",                                      // postscriptName
                     "",                                      // family
                     "Bold",                                  // style
                     "zh-hk",                                 // lang
                     FontWeightNormal,                        // weight
                     FontWidthNormal,                         // width
                     false,                                   // italic
                     false,                                   // monospace
                 }});
#endif

  auto graphicAPI = GraphicsAPI::OPENGL;
#ifdef __EMSCRIPTEN__
  graphicAPI = GraphicsAPI::OPENGL_ES;
#endif
  GlfwWindowManager windowManager;
  auto graphicsProfile = windowManager.createGraphicsProfile(graphicAPI, 3, 2);
  if (!graphicsProfile) return -1;

  auto window = windowManager.createWindow(1024, 576, *graphicsProfile);
  delete graphicsProfile;

  if (!window) return -1;

  std::unique_ptr<ApplicationContext> app(new WidgetSample(window));
  app->run(windowManager);
}
