#include "Filter.hpp"

#include "../3rdparty/jk/src/parser.cpp"

#include <jk/parser.hpp>
#include <jk/print.hpp>

namespace Jk
{
void Filter::onMessage(const value& v)
{
  using namespace jk;
  if(v == value{std::string{}}.v)
    return;
  // An output callback may recompile this node. Keep this message's program
  // alive until all its suspended producers have finished.
  const auto active_actions = actions;
  if(!active_actions || active_actions->empty())
    return;

  // A jq type error - `.a` on a number, `1 + "x"` - is a normal outcome of
  // pointing a filter at data it was not written for, and it unwinds out of
  // the generator as an exception. This runs on the execution thread, so
  // nothing may escape: the program simply produces no more output for this
  // message, exactly as jq produces none for that input.
  //
  // The catch-all is deliberate. bad_alloc from a pathological program must
  // not take the process down either.
  try
  {
    evaluation_scope scope{evaluation};
    for(auto&& res : action::process_sequence(v, *active_actions))
    {
      // Conversion and any callback-retained copies must outlive the arena.
      allocation_scope output_storage{nullptr};
      outputs.bang.call.function(outputs.bang.call.context, res.get());
    }
  }
  catch(const jk::error&)
  {
  }
  catch(...)
  {
  }
}

void Filter::updateProgram(const std::string& value)
{
  // Parsing allocates and can throw on a malformed program; it happens when
  // the text changes, not per message, but it is still not worth crashing on.
  try
  {
    if(auto res = jk::parse(this->inputs.program.value))
      actions = std::make_shared<const std::vector<jk::action_fun>>(
          std::move(res->current_seq));
    else
      actions.reset();
  }
  catch(...)
  {
    actions.reset();
  }
}
}

// Sanity-checks
#include <avnd/binding/ossia/from_value.hpp>
#include <avnd/binding/ossia/to_value.hpp>
static_assert(oscr::is_variant<jk::value>::value);
static_assert(oscr::is_variant_vector<std::vector<jk::value>>::value);
static_assert(oscr::is_variant_list_vector<std::vector<std::vector<jk::value>>>::value);
