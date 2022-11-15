#include "score_addon_jk.hpp"

#include <Avnd/Factories.hpp>
#include <Jk/Filter.hpp>
#include <score/plugins/FactorySetup.hpp>
#include <score_plugin_engine.hpp>

static_assert(oscr::is_variant<jk::value>::value);
static_assert(oscr::is_variant_vector<std::vector<jk::value>>::value);
static_assert(oscr::is_variant_list_vector<std::vector<std::vector<jk::value>>>::value);
// static_assert(boost::mp11::mp_find_if<oscr::is_variant_list_vector, jk::variant>>);
/**
 * This file instantiates the classes that are provided by this plug-in.
 */
score_addon_jk::score_addon_jk() = default;
score_addon_jk::~score_addon_jk() = default;

std::vector<std::unique_ptr<score::InterfaceBase>> score_addon_jk::factories(
    const score::ApplicationContext& ctx, const score::InterfaceKey& key) const
{
  std::vector<std::unique_ptr<score::InterfaceBase>> fx;
  Avnd::instantiate_fx<Jk::Filter>(fx, ctx, key);
  return fx;
}

std::vector<score::PluginKey> score_addon_jk::required() const
{
  return {score_plugin_engine::static_key()};
}

#include <score/plugins/PluginInstances.hpp>
SCORE_EXPORT_PLUGIN(score_addon_jk)
