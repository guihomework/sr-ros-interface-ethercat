#!/usr/bin/env python
# -*- coding: utf-8 -*-

########### this folder plays the role of a main file for U_map computation ##############

### import of other class
#
from  inputs.u_map_data_acquisition_python import U_Map_Data_Acquisition_Python
from  outputs.u_map_output_file import U_Map_Output_File
from  utils.utilitarian import Utilitarian



### Run the friction compensation process
#
class FrictionCompensation(object):
            
   def __init__(self,joint_name, n, P, I, D, shift, hand_number = '48'):
        self.joint_name = joint_name
        self.n = n        
        self.hand_number = hand_number        
        self.P = P
        self.I = I
        self.D = D
        self.shift = shift
    
   def run(self):
        
	computation_steps = ['forward', 'backward']
            
	for direction in computation_steps:
                    
          # Class instanciations                                                
            data_acquisition_python = U_Map_Data_Acquisition_Python(self.joint_name, direction)            
            output_file = U_Map_Output_File(self.joint_name, self.hand_number, direction)
            	  
	  # Data acquisition			    	    
	    [ position, pid_out ] = data_acquisition_python.run_data_acquisition(self.P, self.I, self.D, self.shift)          
	  
	  # Output computation	    
	    [u_map_position_float, u_map_pid_out_float, final_u_map_position, final_u_map_pid_out] = output_file.u_map_computation(position, pid_out, self.n)
	  
	  # Set results in different variables according to the direction
	    if direction == 'forward':      
		position_forward = position
		pid_out_forward = pid_out
		u_map_position_forward = u_map_position_float
		u_map_pid_out_forward = u_map_pid_out_float
		final_u_map_position_forward  = final_u_map_position
		final_u_map_pid_out_forward = final_u_map_pid_out	    
	    else:	      
		position_backward = position
		pid_out_backward = pid_out
		u_map_position_backward = u_map_position_float
		u_map_pid_out_backward = u_map_pid_out_float
		final_u_map_position_backward = final_u_map_position
		final_u_map_pid_out_backward = final_u_map_pid_out
	      
	  # Send U_map table 	                            
            output_file.send_umap(final_u_map_position, final_u_map_pid_out)
            
	
### main execution
#
if __name__ == "__main__":
  FC = FrictionCompensation(joint_name = "FFJ4", n = 15,P=0, I=0, D=0, shift=0)
  
  FC.run()
