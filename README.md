# IRR Lab 5
This ROS2 package is a navigation program that talks to the Nav2 stack, taking in 3 selected points and navigating between the three. The nav2 stack should be in your turtlebot3_ws, not using that in /opt/ros/humble...

## How to Run
1. `ros2 launch turtlebot3_bringup camera_robot.launch.py` Launch turtlebot stack (includes camera) or for SIM: `ros2 launch turtlebot3_gazebo 7785_maze.launch.py`
2. `ros2 launch turtlebot3_cartographer cartographer.launch.py use_sim_time:=false` Run cartographer
3. `ros2 run teleop_twist_keyboard teleop_twist_keyboard` Drive the robot around to create the map
4. `ros2 run nav2_map_server map_saver_cli -f ~/map` Save the map
5. Close cartographer
6. `ros2 launch turtlebot3_navigation2 navigation2.launch.py use_sim_time:=false map:=$HOME/map.yaml` Run Nav2 stack with the provided map
7. Apply pose estimate in Nav2 RVIZ
8. `ros2 run coords_pubsub coords_pubsub` Run node to subscribe to /clicked_points and send actions to /navigate_to_pose
9. In Nav2 RVIZ, select 3 points to traverse between
10. Change `params/burger.yaml` as needed (must build and source for each change)
