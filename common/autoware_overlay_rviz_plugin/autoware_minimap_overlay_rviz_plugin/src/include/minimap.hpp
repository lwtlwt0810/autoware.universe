#ifndef MINIMAP_H
#define MINIMAP_H

#include "overlay_utils.hpp"
#include "rviz_common/properties/color_property.hpp"
#include "rviz_common/properties/float_property.hpp"
#include "rviz_common/properties/int_property.hpp"
#include "rviz_common/properties/ros_topic_property.hpp"
#include "rviz_common/properties/string_property.hpp"
#include "tile_field.hpp"

#include <QImage>
#include <QVBoxLayout>
#include <rclcpp/rclcpp.hpp>
#include <rviz_common/display.hpp>
#include <rviz_common/display_context.hpp>
#include <rviz_common/logging.hpp>
#include <rviz_common/render_panel.hpp>
#include <rviz_common/ros_topic_display.hpp>
#include <rviz_common/view_manager.hpp>
#include <rviz_common/window_manager_interface.hpp>
#include <rviz_rendering/render_window.hpp>

#include <sensor_msgs/msg/nav_sat_fix.hpp>

#include <OgreColourValue.h>
#include <OgreMaterial.h>
#include <OgreTexture.h>
#include <OgreTextureManager.h>

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

namespace autoware_minimap_overlay_rviz_plugin
{

class VehicleMapDisplay : public rviz_common::Display
{
  Q_OBJECT
public:
  VehicleMapDisplay();
  virtual ~VehicleMapDisplay();

  void onInitialize() override;
  void reset() override;
  void update(float, float) override;

protected Q_SLOTS:
  void updateOverlaySize();
  void updateOverlayPosition();
  void onTilesUpdated();
  void updateZoomLevel();

protected:
  void onEnable() override;
  void onDisable() override;

private:
  void drawWidget(QImage & hud);
  void drawCircle(QPainter & painter, const QRectF & backgroundRect);
  void navSatFixCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg);

  rviz_satellite::OverlayObject::SharedPtr overlay_;

  std::mutex mutex_;
  std::mutex property_mutex_;
  std::mutex tile_mutex_;

  rviz_common::properties::IntProperty * property_width_;
  rviz_common::properties::IntProperty * property_height_;
  rviz_common::properties::IntProperty * property_left_;
  rviz_common::properties::IntProperty * property_top_;
  rviz_common::properties::IntProperty * property_zoom_;
  rviz_common::properties::FloatProperty * alpha_property_;
  rviz_common::properties::ColorProperty * background_color_property_;

  int zoom_;

  std::unique_ptr<TileField> tile_field_;
  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr nav_sat_fix_sub_;

  int center_x_tile_;
  int center_y_tile_;

  double latitude_;
  double longitude_;
};

}  // namespace autoware_minimap_overlay_rviz_plugin

#endif  // MINIMAP_H
