`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   07:42:23 08/17/2016
// Design Name:   SPISlave
// Module Name:   C:/Users/Robert/Documents/Hobbies/Quadcopter/flight_controller/fpga/drone/SPISlave_tb.v
// Project Name:  drone
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: SPISlave
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////
`include "spilib.v"

module SPISlave_tb;

	// Inputs
	reg clk_system;
	reg mosi;
	reg sclk;
	reg slave_select_n;
	reg latch;
    reg reset_n;
    
    reg [7:0] wr_data;
    
    wire [7:0] rd_data;
    wire new_data;
    
	// Instantiate the module
    SPISlave uut (
        .clk_system(clk_system), 
        .mosi(mosi), 
        .sclk(sclk), 
        .slave_select_n(slave_select_n), 
        .latch(latch), 
        .wr_data(wr_data), 
        .reset_n(reset_n), 
        .miso(miso), 
        .rd_data(rd_data),
        .new_data(new_data)
        );

    /* create a 20MHz system clock */
    always begin
        clk_system = 0; #(`CLK / 2);
        clk_system = 1; #(`CLK / 2);
    end
    
    reg [7:0] miso_data  = 8'h00;
    reg [7:0] master_out = 8'hAA;
    reg [7:0] slave_out = 8'hB7;
    
	initial begin
		// Initialize Inputs
		mosi = 0;
		sclk = 0;
		slave_select_n = 1;
		latch = 0;

		// Wait 100 ns for global reset to finish
        reset_n = 0;
		#100;
        reset_n = 1;
        
        /* load the slave output register */
        spi_load(slave_out, latch, wr_data);
        
		// Add stimulus here
        slave_select_n = 1; #`CLK
        slave_select_n = 0;
        
        //spi_send(miso, sclk, master_out, miso_data, mosi);
        
        slave_select_n = 1;
        #2000;
	end
      
    
    
endmodule

