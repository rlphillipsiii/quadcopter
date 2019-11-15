`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:50:51 08/22/2016 
// Design Name: 
// Module Name:    DRP001 
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
module DRP001(
    input clk_system,
    input reset_n,
    input [3:0] ctrl_inputs,
    input mosi,
    input slave_select_n,
    input sck,
    output miso,
    output ctrl_data_rdy
    );

    Receiver ctrl_recv (
        .clk_system(clk_system),
        .inputs(ctrl_inputs),
        .select(ctrl_select),
        .reset_n(reset_n),
        .data_rdy(ctrl_data_rdy),
        .data(data)
    );
    
endmodule
