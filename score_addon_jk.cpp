#include "score_addon_jk.hpp"
#include <Jk/AnEffect.hpp>

#include <Avnd/Factories.hpp>
#include <score/plugins/FactorySetup.hpp>
#include <score_plugin_engine.hpp>

/**
 * This file instantiates the classes that are provided by this plug-in.
 */
score_addon_jk::score_addon_jk() = default;
score_addon_jk::~score_addon_jk() = default;

std::vector<std::unique_ptr<score::InterfaceBase>>
score_addon_jk::factories(
    const score::ApplicationContext& ctx,
    const score::InterfaceKey& key) const
{
  return Avnd::instantiate_fx<Jk::AnEffect>(ctx, key);
}

std::vector<score::PluginKey> score_addon_jk::required() const
{
  return {score_plugin_engine::static_key()};
}

#include <score/plugins/PluginInstances.hpp>
SCORE_EXPORT_PLUGIN(score_addon_jk)
