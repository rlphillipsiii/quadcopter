`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    06:46:26 08/17/2016 
// Design Name: 
// Module Name:    SPIMaster 
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
module SPIMaster(
    input  clk_system,
    input  reset_n,
    input [7:0] wr_data,
    input  write,
    output miso,
    output mosi,
    output sclk,
    output reg [7:0] wr_data
    );


endmodule
