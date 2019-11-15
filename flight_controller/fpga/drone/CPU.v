`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    07:26:38 08/23/2016 
// Design Name: 
// Module Name:    RAM 
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
module RAM(
    input clk_system,
    input reset_n,
    input [7:0] rd_addr,
    input rd,
    input [7:0] wr_addr,
    input wr,
    input [7:0] wr_data,
    output reg [7:0] rd_data
    );
    
    reg [63:0] memory;
    
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n) rd_data <= 8'h00;
        else if (rd) begin
        case (rd_addr)
            0: rd_data <= memory[7:0];
            1: rd_data <= memory[15:8];
            2: rd_data <= memory[23:16];
            3: rd_data <= memory[31:24];
            4: rd_data <= memory[39:32];
            5: rd_data <= memory[47:40];
            6: rd_data <= memory[55:48];
            7: rd_data <= memory[63:56];
            default: rd_data <= rd_data;
            endcase
        end
        else rd_data <= rd_data;
    end
    
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n) memory = 64'h00;
        else if (wr) begin
            case (wr_addr)
            0: memory[7:0]   <= wr_data;
            1: memory[15:8]  <= wr_data;
            2: memory[23:16] <= wr_data;
            3: memory[31:24] <= wr_data;
            4: memory[39:32] <= wr_data;
            5: memory[47:40] <= wr_data;
            6: memory[55:48] <= wr_data;
            7: memory[63:56] <= wr_data;
            default: memory <= memory;
            endcase
        end
        else memory <= memory;
    end

endmodule
