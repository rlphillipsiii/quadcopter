`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    13:24:53 09/10/2016 
// Design Name: 
// Module Name:    ReceiverSpiFSM 
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
module ReceiverSpiFSM(
    input clk_system,
    input reset_n,
    input spi_data_rdy,
    input start,
    input [10:0] ctrl_data,
    output spi_latch,
    output reg [7:0] spi_out,
    output reg [1:0] ctrl_chan_sel,
    output op_complete
    );

    localparam STATE_IDLE     = 0;
    localparam STATE_LOAD     = 1;
    localparam STATE_INC      = 2;
    localparam STATE_LATCH    = 3;
    localparam STATE_WAIT     = 4;
    localparam STATE_COMPLETE = 5;
    
    reg [2:0] state;
    reg [2:0] next;
    
    reg [1:0] high_byte;
    
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n)                 ctrl_chan_sel <= 3'h0;
        else if (state == STATE_IDLE) ctrl_chan_sel <= 3'h0;
        else if (state == STATE_INC)  ctrl_chan_sel <= ctrl_chan_sel + 1;
        else                          ctrl_chan_sel <= ctrl_chan_sel;
    end
    
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n)                  high_byte <= 2'h0;
        else if (state == STATE_INC)   high_byte <= 2'h0;
        else if (state == STATE_IDLE)  high_byte <= 2'h0;
        else if (state == STATE_LATCH) high_byte <= high_byte + 1;
        else                           high_byte <= high_byte;
    end
    
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n) spi_out <= 8'h00;
        else if (state == STATE_LOAD) begin
            if (high_byte == 2'h0) spi_out <= { 5'h0, ctrl_data[10:8] };
            else                   spi_out <= ctrl_data[7:0];
        end
        else spi_out <= spi_out;
    end
    
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n) state <= STATE_IDLE;
        else          state <= next;
    end
    
    always @(*) begin
        case (state)
        STATE_IDLE: begin
            if (start) next <= STATE_LOAD;
            else       next <= STATE_IDLE;
        end
        
        STATE_INC: begin
            next <= STATE_LOAD;
        end
        
        STATE_LOAD: begin
            next <= STATE_LATCH;
        end
        
        STATE_LATCH: begin
            next <= STATE_WAIT;
        end
        
        STATE_WAIT: begin
            if (spi_data_rdy) begin
                if (high_byte == 3'h1)          next <= STATE_LOAD;
                else if (ctrl_chan_sel == 3'h3) next <= STATE_COMPLETE;
                else                            next <= STATE_INC;
            end
            else next <= STATE_WAIT;
        end
        
        STATE_COMPLETE: begin
            next <= STATE_IDLE;
        end
        
        endcase
    end
    
    assign spi_latch   = (state == STATE_LATCH);
    assign op_complete = (state == STATE_COMPLETE);
    
endmodule
