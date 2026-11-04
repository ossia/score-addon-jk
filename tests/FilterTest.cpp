// The object filter driven as the score executor drives it: the program
// control's update() compiles the text, the message inlet calls onMessage, and
// every value the program produces leaves through the bang callback.

#include <Jk/Filter.hpp>

#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <string>
#include <vector>

namespace
{
struct Harness
{
  // The object carries its evaluation arena inline: keep it off the stack.
  std::unique_ptr<Jk::Filter> filter = std::make_unique<Jk::Filter>();
  std::vector<Jk::value> out;

  Harness()
  {
    auto& cb = filter->outputs.bang.call;
    cb.context = this;
    cb.function = [](void* ctx, const Jk::value& v) {
      static_cast<Harness*>(ctx)->out.push_back(v);
    };
  }

  void program(std::string text)
  {
    filter->inputs.program.value = std::move(text);
    filter->inputs.program.update(*filter);
  }

  std::vector<Jk::value> send(const Jk::value& in)
  {
    out.clear();
    filter->onMessage(in);
    return out;
  }
};

double number(const Jk::value& v)
{
  if(auto i = boost::variant2::get_if<int64_t>(&v.v))
    return double(*i);
  if(auto d = boost::variant2::get_if<double>(&v.v))
    return *d;
  FAIL("not a number");
  return 0.;
}

Jk::value list(std::initializer_list<int> values)
{
  jk::list_type l;
  for(int v : values)
    l.emplace_back(v);
  return Jk::value{std::move(l)};
}
}

TEST_CASE("Without a program the filter produces nothing", "[jk][filter]")
{
  Harness h;
  CHECK(h.send(Jk::value{1}).empty());
  h.program("");
  CHECK(h.send(Jk::value{1}).empty());
}

TEST_CASE("A program maps each message to its outputs", "[jk][filter]")
{
  Harness h;
  h.program(". + 1");
  auto res = h.send(Jk::value{41});
  REQUIRE(res.size() == 1);
  CHECK(number(res[0]) == 42.);

  h.program(".[]");
  res = h.send(list({1, 2, 3}));
  REQUIRE(res.size() == 3);
  CHECK(number(res[0]) == 1.);
  CHECK(number(res[1]) == 2.);
  CHECK(number(res[2]) == 3.);
}

TEST_CASE("An empty string message is ignored", "[jk][filter]")
{
  Harness h;
  h.program(".");
  CHECK(h.send(Jk::value{std::string{}}).empty());
  CHECK(h.send(Jk::value{std::string{"x"}}).size() == 1);
}

TEST_CASE("A malformed program clears the previous one", "[jk][filter]")
{
  Harness h;
  h.program(".");
  REQUIRE(h.send(Jk::value{1}).size() == 1);
  h.program("(((");
  CHECK(h.send(Jk::value{1}).empty());
}

TEST_CASE("A runtime type error ends the outputs of that message", "[jk][filter]")
{
  Harness h;
  h.program(".a");
  CHECK_NOTHROW(h.send(Jk::value{1}));
  CHECK(h.send(Jk::value{1}).empty());

  // The next message still runs.
  h.program(". * 2");
  auto res = h.send(Jk::value{3});
  REQUIRE(res.size() == 1);
  CHECK(number(res[0]) == 6.);
}
