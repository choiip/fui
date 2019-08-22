
namespace fui {

class Widget;

enum class Focus { IN, OUT };

class FocusEvent {
public:
  const Focus value; 
  Widget* source;

  FocusEvent(Focus value, Widget* source)
  : value(value)
  , source(source) {}
};

} // namespace fui
