`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    00:53:17 08/20/2016 
// Design Name: 
// Module Name:    I2CMaster 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module I2CMaster(
    input clk_system,
    input reset_n,
    input sda,
    input scl,
    input rw,
    input addr
    );

    parameter [3:0] 
        idle = 4'h0,
        start = 4'h1,
        rdwr  = 4'h2;
        
   always @(posedge clk_system, negedge reset_n) begin
   
   end
   
endmodule
