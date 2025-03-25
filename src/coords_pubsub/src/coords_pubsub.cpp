#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include <queue>

using std::placeholders::_1;
using namespace std::chrono_literals;
using NavigateToPose = nav2_msgs::action::NavigateToPose;
using GoalHandleNavigateToPose = rclcpp_action::ClientGoalHandle<NavigateToPose>;

class CoordsPubSub : public rclcpp::Node
{
  public:
    CoordsPubSub() : Node("coords_pubsub")
    {

      // publish to /goal_pose
      client_ptr_ = rclcpp_action::create_client<NavigateToPose>(this, "navigate_to_pose");

      // subscribe to /clicked_points
      subscriber_clicks = this->create_subscription<geometry_msgs::msg::PointStamped>(
        "/clicked_point", 10, std::bind(&CoordsPubSub::coords_callback, this, _1));

    }

  private:

    // Basic instantiations
    std::queue<geometry_msgs::msg::PointStamped> coords_queue;
    geometry_msgs::msg::PointStamped current_goal;

    // Capture recent coordinate clicks and publish goal poses after queue if full
    void coords_callback(const geometry_msgs::msg::PointStamped & coords)
    {
      // Store the coordinates in a queue until it reaches 3 elements
      if ((coords_queue.size() < 3))
      {
        coords_queue.push(coords);
        RCLCPP_INFO(this->get_logger(), "Pushed Coords: '%f %f %f'", coords.point.x, coords.point.y, coords.point.z);
      }
      
      // Start recursive action sending if the queue has 3 elements
      if (coords_queue.size() == 3)
      {
        send_goal();
      }
    }

    // Send next the goal poses
    void send_goal()
    {
      // Stop if no goals
      if (coords_queue.empty())
      {
        RCLCPP_INFO(this->get_logger(), "All goals completed.");
        return;
      }

      // Get goal poses
      current_goal = coords_queue.front();

      // Action client variables
      auto goal_msg = NavigateToPose::Goal();
      goal_msg.pose.header.frame_id = "map";
      goal_msg.pose.header.stamp = this->now();
      auto send_goal_options = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();

      goal_msg.pose.pose.position.x = current_goal.point.x;
      goal_msg.pose.pose.position.y = current_goal.point.y;
      goal_msg.pose.pose.orientation.w = 1.0;

      send_goal_options.result_callback = 
      [this](const GoalHandleNavigateToPose::WrappedResult & result) {
        if (result.code == rclcpp_action::ResultCode::SUCCEEDED) {
          RCLCPP_INFO(this->get_logger(), "Goal reached. Moving to next.");
          coords_queue.pop();
          send_goal();  // Recursive call to send next goal
        }
      };

      // Wait a second before sending the goal
      std::this_thread::sleep_for(std::chrono::seconds(2));

      RCLCPP_INFO(this->get_logger(), "Going to goal: %f, %f", current_goal.point.x, current_goal.point.y);
      client_ptr_->async_send_goal(goal_msg, send_goal_options);
    }

    // Memory management
    rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr subscriber_clicks;
    rclcpp_action::Client<NavigateToPose>::SharedPtr client_ptr_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CoordsPubSub>());
  rclcpp::shutdown();
  return 0;
}