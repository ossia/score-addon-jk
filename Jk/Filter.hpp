#pragma once
#include <boost/variant2.hpp>
#include <halp/callback.hpp>
#include <halp/controls.hpp>
#include <halp/messages.hpp>
#include <halp/meta.hpp>
#include <halp/static_string.hpp>

#include <map>
#include <string>
#include <vector>

#define JK_CONFIG_CUSTOMIZATION 1
namespace jk
{
namespace config
{
namespace variant_ns = ::boost::variant2;
template <typename... Args>
using variant = boost::variant2::variant<Args...>;
template <typename... Args>
using vector = std::vector<Args...>;
template <typename... Args>
using map = std::map<Args...>;
using string = std::string;
}
}

#include <jk/generator.hpp>
#include <jk/value.hpp>
namespace Jk
{
using value = jk::value;
struct Filter
{
  halp_meta(name, "Object filter")
  halp_meta(c_name, "object_filter")
  halp_meta(category, "Control/Data processing")
  halp_meta(author, "Jean-Michaël Celerier")
  halp_meta(description, "Object query filter")
  halp_meta(uuid, "4cf2f89f-45b6-4878-92ff-23d3cee1c67e")

  void onMessage(const value& in);
  void updateProgram(const std::string& value);

  struct
  {
    struct : halp::lineedit<"Filter", "">
    {
      void update(Filter& obj) { obj.updateProgram(this->value); }
    } program;
  } inputs;

  struct
  {
    halp::callback<"bang", value> bang;
  } outputs;

  struct messages
  {
    ::halp::func_ref<"Input", &Filter::onMessage> input;
  };

private:
  std::vector<jk::action_fun> actions;
};
}
