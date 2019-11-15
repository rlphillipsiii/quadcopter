`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   22:46:20 08/19/2016
// Design Name:   ReceiverChannel
// Module Name:   C:/Users/Robert/Documents/Hobbies/Quadcopter/flight_controller/fpga/drone/ReceiverChannel_tb.v
// Project Name:  drone
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: ReceiverChannel
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module ReceiverChannel_tb;

	// Inputs
	reg clk_system;
	reg reset_n;
	reg signal;

	// Outputs
	wire data;
	wire [10:0] count;
    
	// Instantiate the Unit Under Test (UUT)
	ReceiverChannel uut (
		.clk_system(clk_system), 
		.reset_n(reset_n), 
		.signal(signal), 
		.data(data), 
		.count(count)
	);

    /* create a 625KHz system clock */
    always begin
        clk_system = 0; #800;
        clk_system = 1; #800;
    end
    
	initial begin
		// Initialize Inputs
		signal = 0;

		// Wait 100 ns for global reset to finish
 		reset_n = 0;
		#2000;
        reset_n = 1;
        
		// Add stimulus here
        signal = 1; #1000000;
        signal = 0; #19000000;
        
        signal = 1; #1500000;
        signal = 0; #18500000;
        
        signal = 1; #2000000;
        signal = 0; #18000000;
	end
      
endmodule

