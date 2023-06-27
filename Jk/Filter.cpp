#include "Filter.hpp"

#include "../3rdparty/jk/src/parser.cpp"

#include <jk/parser.hpp>
#include <jk/print.hpp>

namespace Jk
{
void Filter::onMessage(const value& v)
{
  using namespace jk;
  if(v != value{std::string{}}.v)
  {
    if(!actions.empty())
    {
      for(auto&& v : action::process_sequence(v, actions))
      {
        outputs.bang(std::move(v.data));
      }
    }
  }
}

void Filter::updateProgram(const std::string& value)
{
  if(auto res = jk::parse(this->inputs.program.value))
    actions = std::move(res->current_seq);
  else
    actions.clear();
}
}
