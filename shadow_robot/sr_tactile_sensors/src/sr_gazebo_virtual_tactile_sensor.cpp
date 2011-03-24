/**
 * @file   sr_gazebo_virtual_tactile_sensor.cpp
 * @author Ugo Cupcic <ugo@shadowrobot.com> updated by Ravin de Souza <rsouza@isr.ist.utl.pt>, Contact <contact@shadowrobot.com>
 * @date
 * 
 * @brief  This is the virtual implementation of the SrGenericTactileSensor that publishes
 * touch data obtained from the robot hand simulation in gazebo.
 * 
 */

#include "sr_tactile_sensors/sr_gazebo_virtual_tactile_sensor.hpp"
#include <boost/algorithm/string.hpp>
#include <string>
#include <math.h>
#include <ros/ros.h>

namespace shadowrobot
{
/**********************************
 *         TACTILE SENSOR         *
 **********************************/
  SrGazeboVirtualTactileSensor::SrGazeboVirtualTactileSensor(std::string name,
                                                             std::string gazebo_bumper_topic) :
    SrGenericTactileSensor(name, "", ""),
    touch_value(0.0)
  {
    sub = nh.subscribe(gazebo_bumper_topic, 2, &SrGazeboVirtualTactileSensor::callback, this);
  }

  SrGazeboVirtualTactileSensor::~SrGazeboVirtualTactileSensor()
  {}

  void SrGazeboVirtualTactileSensor::callback(const gazebo_plugins::ContactsState& msg)
  {
    double tmp_value;
    const ::geometry_msgs::Vector3& v = msg.states[0].wrenches[0].force;

    ROS_INFO("Touch by %s", msg.header.frame_id.c_str());
    // Parse the message to retrieve the relevant touch pressure information
    // Currently just taking the first contact.
    // More sophisticated analysis can be done to take the contact that is the most aligned with the distal link normal
    tmp_value = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));

    touch_mutex.lock();
    touch_value = tmp_value;
    touch_mutex.unlock();
  }

  double SrGazeboVirtualTactileSensor::get_touch_data()
  {
    double return_value;

    touch_mutex.lock();
    return_value = touch_value;
    touch_mutex.unlock();

    return return_value;
  }

  double SrGazeboVirtualTactileSensor::get_temp_data()
  {
    return 0;
  }


/**********************************
 *     TACTILE SENSOR MANAGER     *
 **********************************/
  SrGazeboVirtualTactileSensorManager::SrGazeboVirtualTactileSensorManager() :
    SrTactileSensorManager()
  {
    std::vector<std::vector<std::string> > all_names = get_all_names();

    for( unsigned int i=0; i<5; ++i)
    {
      tactile_sensors.push_back(
        boost::shared_ptr<SrGazeboVirtualTactileSensor>(
          new SrGazeboVirtualTactileSensor(all_names[0][i],
                                           "/"+all_names[0][i]+"distal_bumper/state")
        )
      );
    }
  }

  SrGazeboVirtualTactileSensorManager::~SrGazeboVirtualTactileSensorManager()
  {}
}


/** 
 * Initializes a set of virtual tactile sensors and publish.
 * 
 * @param argc 
 * @param argv 
 * 
 * @return -1 if error linking with the robot (i.e. robot code not started)
 */
int main(int argc, char** argv)
{
  ros::init(argc, argv, "sr_tactile_sensor");
  ros::NodeHandle n;

  shadowrobot::SrGazeboVirtualTactileSensorManager tact_sens_mgr;

  while( ros::ok() )
    tact_sens_mgr.publish_all();

  return 0;
}

/* For the emacs weenies in the crowd.
Local Variables:
   c-basic-offset: 2
End:
*/
