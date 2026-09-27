#pragma once
#include <boost/variant2.hpp>
#include <halp/callback.hpp>
#include <halp/controls.hpp>
#include <halp/messages.hpp>
#include <halp/meta.hpp>
#include <halp/static_string.hpp>
#include <jk/memory.hpp>

#include <array>
#include <map>
#include <memory>
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
template <typename T>
using vector = std::vector<T, jk::allocator<T>>;
template <typename K, typename V>
using map = std::map<K, V, std::less<>, jk::allocator<std::pair<const K, V>>>;
using string = std::basic_string<char, std::char_traits<char>, jk::allocator<char>>;
}
}

#include <jk/action_fun.hpp>
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
  halp_meta(manual_url, "https://ossia.io/score-docs/processes/object-filter.html")
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
    halp::callback<"bang", const value&> bang;
  } outputs;

  struct messages
  {
    ::halp::func_ref<"Input", &Filter::onMessage> input;
  };

private:
  std::shared_ptr<const std::vector<jk::action_fun>> actions;
  // Common messages use preallocated storage; explicit upstream growth keeps
  // large-message compatibility. Strict bounded contexts are available in jk.
  std::array<std::byte, 256 * 1024> evaluation_storage;
  jk::evaluation_context evaluation{evaluation_storage, jk::pmr::new_delete_resource()};
};
}
