#include "Filter.hpp"
#include <jk/parser.hpp>
#include "../3rdparty/jk/src/parser.cpp"

namespace Jk
{
void Filter::onMessage(const value& v)
{
  using namespace jk;
  if(v != value{std::string{}}.v)
  {
    if(!actions.empty()) {
      for(auto&& v : action::collapse(v, actions)) {
        outputs.value.call(std::move(v.data));
      }
    }
  }
}

void Filter::updateProgram(const std::string& value)
{
  if(auto res = jk::parse(this->inputs.program.value))
    actions = std::move(res->total);
  else
    actions.clear();
}
}
