// Copyright 2020 Apex.AI GmbH
// Copyright 2021 Tier IV, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>

#include <rclcpp_generic/generic_publisher.hpp>
#include <rclcpp_generic/generic_subscription.hpp>

#include <memory>
#include <string>

namespace topic_tools
{

class RelayNode : public rclcpp::Node
{
public:
  explicit RelayNode(const rclcpp::NodeOptions & node_options);

private:
  rclcpp_generic::GenericSubscription::SharedPtr sub_;
  rclcpp_generic::GenericPublisher::SharedPtr pub_;
};

RelayNode::RelayNode(const rclcpp::NodeOptions & node_options)
: rclcpp::Node("relay", node_options)
{
  auto input_topic = declare_parameter("input_topic").get<std::string>();
  auto output_topic = declare_parameter("output_topic").get<std::string>();
  auto type = declare_parameter("type").get<std::string>();
  auto reliability = declare_parameter("reliability", "default");
  auto durability = declare_parameter("durability", "default");

  rclcpp::QoS qos{1};
  if (reliability == "default") {
    // default reliability
  } else if (reliability == "reliable") {
    qos.reliable();
  } else if (reliability == "best_effort") {
    qos.best_effort();
  } else {
    RCLCPP_ERROR(get_logger(), "unknown reliability: " + reliability);
  }
  if (durability == "default") {
    // default durability
  } else if (durability == "volatile") {
    qos.durability_volatile();
  } else if (durability == "transient_local") {
    qos.transient_local();
  } else {
    RCLCPP_ERROR(get_logger(), "unknown durability: " + durability);
  }

  pub_ = rclcpp_generic::GenericPublisher::create(
    get_node_topics_interface(), output_topic, type, qos);
  sub_ = rclcpp_generic::GenericSubscription::create(
    get_node_topics_interface(), input_topic, type, qos,
    [this](std::shared_ptr<rclcpp::SerializedMessage> msg) {
      auto msg_rcl = std::make_shared<rcl_serialized_message_t>(msg->get_rcl_serialized_message());
      pub_->publish(msg_rcl);
    });
}

}  // namespace topic_tools

RCLCPP_COMPONENTS_REGISTER_NODE(topic_tools::RelayNode)
