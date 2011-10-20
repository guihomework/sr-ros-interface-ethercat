/**
 * @file   generic_tactiles.hpp
 * @author Toni Oliver <toni@shadowrobot.com>
 * @date   Th Oct 20 10:06:14 2011
 *
* Copyright 2011 Shadow Robot Company Ltd.
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
*
 * @brief This is a class for accessing the data from the
 *        PSTs tactiles.
 *
 *
 */

#ifndef _SHADOW_PSTS_HPP_
#define _SHADOW_PSTS_HPP_

#include "sr_robot_lib/generic_tactiles.hpp"

namespace tactiles
{
  class ShadowPSTs :
    public GenericTactiles
  {
  public:
    ShadowPSTs();
    ~ShadowPSTs() {};

    /**
     * This function is called each time a new etherCAT message
     * is received in the sr06.cpp driver. It  updates the tactile
     * sensors values contained in tactiles_vector.
     *
     * @param status_data the received etherCAT message
     */
    virtual void update(ETHERCAT_DATA_STRUCTURE_0200_PALM_EDC_STATUS* status_data);

    /**
     * Updates the command to send to the hand. This function is called
     * at each packCommand() call. Ask for the relevant information for the tactiles.
     * If an unimportant data is waiting then we send it, otherwise, we send the next
     * important data.
     *
     * @param command The command which will be sent to the palm.
     */
    virtual void build_command(ETHERCAT_DATA_STRUCTURE_0200_PALM_EDC_COMMAND* command);

  private:
    boost::shared_ptr< Tactile<PST3Data> > tactiles_vector;
  };//end class
}//end namespace

/* For the emacs weenies in the crowd.
Local Variables:
   c-basic-offset: 2
End:
*/

#endif /* _SHADOW_PSTS_HPP_ */
