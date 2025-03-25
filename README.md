# IRR Lab 5
This ROS2 package is a navigation program that talks to the Nav2 stack, taking in 3 selected points and navigating between the three.

## How to Run
1. `ros2 launch turtlebot3_bringup camera_robot.launch.py` Launch turtlebot stack (includes camera)
2. `ros2 launch turtlebot3_cartographer cartographer.launch.py use_sim_time:=false` Run cartographer
3. `ros2 launch turtlebot3_navigation2 navigation2.launch.py use_sim_time:=false` Run Nav2 stack
4. `ros2 run coords_pubsub coords_pubsub` Run node to subscribe to /clicked_points and send actions to /navigate_to_pose
5. in Nav2 RVIZ, select 3 points to traverse between
