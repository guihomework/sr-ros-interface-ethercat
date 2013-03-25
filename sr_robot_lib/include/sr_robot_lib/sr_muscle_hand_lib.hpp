/**
 * @file   sr_muscle_hand_lib.hpp
 * @author Ugo Cupcic <ugo@shadowrobot.com>, Toni Oliver <toni@shadowrobot.com>, contact <software@shadowrobot.com>
 * @date   Tue Mar  19 17:12:13 2013
 *
 *
 * Copyright 2013 Shadow Robot Company Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @brief This is a library for the etherCAT muscle hand.
 * You can find it instantiated in the sr_edc_ethercat_drivers.
 *
 *
 */

#ifndef _SR_MUSCLE_HAND_LIB_HPP_
#define _SR_MUSCLE_HAND_LIB_HPP_

#include "sr_robot_lib/sr_muscle_robot_lib.hpp"
#include <std_srvs/Empty.h>

//to be able to load the configuration from the
//parameter server
#include <ros/ros.h>
#include <string>

namespace shadow_robot
{
  template <class StatusType, class CommandType>
  class SrMuscleHandLib : public SrMuscleRobotLib<StatusType, CommandType>
  {
  public:
    SrMuscleHandLib(pr2_hardware_interface::HardwareInterface *hw);
    ~SrMuscleHandLib();


    /**
     * Reset the motor at motor index.
     *
     * @param request empty
     * @param response empty
     * @param joint A pair containing the index of the motor for the given
     *              joint followed by the name of the joint we're resetting
     *
     * @return true if success
     */
    bool reset_motor_callback(std_srvs::Empty::Request& request,
                              std_srvs::Empty::Response& response,
                              std::pair<int,std::string> joint);

#ifdef DEBUG_PUBLISHER
    /**
     * This is a service callback: we set the debug data we want to publish
     * at full speed in the debug topics.
     *
     * @param request Contains the motor index and the MOTOR_DATA type
     * @param response True if succeedded.
     *
     * @return true if succeeded.
     */
    bool set_debug_data_to_publish(sr_robot_msgs::SetDebugData::Request& request,
                                   sr_robot_msgs::SetDebugData::Response& response);
#endif

  protected:
    /**
     * Initializes the hand library with the needed values.
     *
     * @param joint_names A vector containing all the joint names.
     * @param actuator_ids A vector containing the corresponding actuator ids.
     * @param joint_to_sensors A vector mapping the joint to the sensor index we read from the palm.
     * @param actuators A vector containing the actuators for the different joints.
     */
    virtual void initialize(std::vector<std::string> joint_names, std::vector<int> actuator_ids,
                            std::vector<shadow_joints::JointToSensor> joint_to_sensors,
                            std::vector<sr_actuator::SrGenericActuator*> actuators);

    /**
     * Initializes the hand library with the needed values.
     *
     * @param joint_names A vector containing all the joint names.
     * @param actuator_ids A vector containing the corresponding actuator ids.
     * @param joint_to_sensors A vector mapping the joint to the sensor index we read from the palm.
     * @param actuators A vector containing the actuators for the different joints.
     */
    void initialize(std::vector<std::string> joint_names, std::vector<shadow_joints::JointToMuscle> actuator_ids,
                            std::vector<shadow_joints::JointToSensor> joint_to_sensors,
                            std::vector<sr_actuator::SrGenericActuator*> actuators);


    /**
     * Finds the joint name for a certain motor index
     *
     * @param motor_index The integer motor index
     */
    std::string find_joint_name(int motor_index);

  private:

    /**
     * Reads the mapping associating a joint to a muscle.
     * If the muscle index is -1, then no muscle is associated
     * to this joint.
     *
     *
     * @return a vector of JointToMuscle structures (containing the indexes of the muscles for the joint), ordered by joint.
     */
    std::vector<shadow_joints::JointToMuscle> read_joint_to_muscle_mapping();


    static const int nb_muscle_data;
    static const char* human_readable_muscle_data_types[];
    static const int32u muscle_data_types[];

    /// a service server for reconfiguring the debug data we want to publish
    ros::ServiceServer debug_service;

    /**
     * A map used to keep the timers created in reset_motor_callback alive.
     * We're using a map to keep only one timer per joint.
     */
    std::map<std::string, ros::Timer> pid_timers;
  };

}

/* For the emacs weenies in the crowd.
Local Variables:
   c-basic-offset: 2
End:
*/

#endif