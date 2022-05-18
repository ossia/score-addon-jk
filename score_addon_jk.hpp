#pragma once
#include <score/application/ApplicationContext.hpp>
#include <score/plugins/Interface.hpp>
#include <score/plugins/qt_interfaces/FactoryInterface_QtInterface.hpp>
#include <score/plugins/qt_interfaces/PluginRequirements_QtInterface.hpp>

#include <verdigris>

class score_addon_jk final
    : public score::FactoryInterface_QtInterface
    , public score::Plugin_QtInterface
{
  SCORE_PLUGIN_METADATA(1, "c8a00c5e-71ea-4eb7-bc21-4af7b7c6030f")
public:
  score_addon_jk();
  ~score_addon_jk() override;

private:
  std::vector<std::unique_ptr<score::InterfaceBase>> factories(
      const score::ApplicationContext&,
      const score::InterfaceKey& factoryName) const override;

  std::vector<score::PluginKey> required() const override;
};
