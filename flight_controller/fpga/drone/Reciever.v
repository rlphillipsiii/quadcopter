`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:52:11 08/22/2016 
// Design Name: 
// Module Name:    Reciever 
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
module Receiver(
    input clk_system,
    input reset_n,
    input [3:0] inputs,
    input [1:0] select,
    output data_rdy,
    output reg [10:0] data
    );
    
    localparam CH1_SELECT = 0;
    localparam CH2_SELECT = 1;
    localparam CH3_SELECT = 2;
    localparam CH4_SELECT = 3;
    
    reg [3:0] ready;

    ClockDivider clk_div (
        .clk_system(clk_system),
        .reset_n(reset_n),
        .clk_16(clk_16)
    );
    
    ReceiverChannel ch1 (
        .clk_system(clk_16), 
		.reset_n(reset_n), 
		.signal(inputs[0]), 
		.data(data_ch1), 
		.count(count_ch1)
    );
    
    ReceiverChannel ch2 (
        .clk_system(clk_16), 
		.reset_n(reset_n), 
		.signal(inputs[1]), 
		.data(data_ch2), 
		.count(count_ch2)
    );
    
    ReceiverChannel ch3 (
        .clk_system(clk_16), 
		.reset_n(reset_n), 
		.signal(inputs[2]), 
		.data(data_ch3), 
		.count(count_ch3)
    );
    
    ReceiverChannel ch4 (
        .clk_system(clk_16), 
		.reset_n(reset_n), 
		.signal(inputs[3]), 
		.data(data_ch4), 
		.count(count_ch4)
    );
    
    always @(*) begin
        case (select)
        CH1_SELECT: data <= count_ch1;
        CH2_SELECT: data <= count_ch2;
        CH3_SELECT: data <= count_ch3;
        CH4_SELECT: data <= count_ch4;
        default:    data <= 11'h00;
        endcase
    end
    
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n)          ready <= 4'h0;
        else if (data == 4'hF) ready <= 4'h0;
        else begin
            if (data_ch1) ready[0] <= 1'h1;
            if (data_ch2) ready[1] <= 1'h1;
            if (data_ch3) ready[2] <= 1'h1;
            if (data_ch4) ready[3] <= 1'h1;
        end
    end
    
    assign data_rdy = (ready == 4'hF);
    
endmodule
