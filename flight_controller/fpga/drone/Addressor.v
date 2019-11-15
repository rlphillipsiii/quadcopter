`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    07:26:38 08/23/2016 
// Design Name: 
// Module Name:    Addressor 
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
`define ADDR_RECEIVER  7'h1F
`define ADDR_ALTIMETER 7'h7C
`define ADDR_GPS       7'h05

`define READ  0
`define WRITE 1

module Addressor(
    input clk_system,
    input reset_n,
    input mosi,
    input slave_select_n,
    input sck,
    input [10:0] ctrl_data,
    output miso,
    output [1:0] ctrl_chan_sel
    );

    localparam STATE_IDLE     = 0;
    localparam STATE_ADDR     = 1;
    localparam STATE_RW       = 2;
    localparam STATE_READ     = 3;
    localparam STATE_WRITE    = 4;
    localparam STATE_READING  = 5;
    localparam STATE_WRITING1 = 6;
    localparam STATE_WRITING2 = 7;
    
    localparam SPI_RECEIVER = 0;
    
    reg [3:0] state;
    reg [3:0] next_state;
        
    reg [7:0] spi_out;
    reg [7:0] spi_in;
    reg spi_latch;
    
    reg [6:0] target;

    reg [1:0] spi_select;
    
    reg start_receiver_spi;
    
    SPISlave spi_slave(
        .clk_system(clk_system),
        .mosi(mosi),
        .sclk(sck),
        .slave_select_n(slave_select_n),
        .latch(spi_latch),
        .wr_data(spi_out),
        .reset_n(reset_n),
        .new_data(spi_data_rdy),
        .miso(miso),
        .rd_data(spi_in)
    );
    
    ReceiverSpiFSM(
        .clk_system(clk_system),
        .reset_n(reset_n),
        .spi_data_rdy(spi_data_rdy),
        .start(start_receiver_spi),
        .ctrl_data(ctrl_data),
        .spi_latch(receiver_latch),
        .spi_out(receiver_out),
        .ctrl_chan_sel(ctrl_chan_sel),
        .op_complete(receiver_done)
    );
    
    always @(*) begin
        case (spi_select)
            SPI_RECEIVER: spi_out <= receiver_out;
            default:      spi_out <= receiver_out;
        endcase
    end
        
    always @(*) begin
        case (spi_select)
            SPI_RECEIVER: spi_latch <= receiver_latch;
            default:      spi_latch <= receiver_latch;
        endcase
    end
    
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n)                 target <= 7'h00;
        else if (state == STATE_IDLE) target <= 7'h00;
        else if (state == STATE_RW)   target <= spi_in[7:1];
        else                          target <= target;
    end
    
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n) spi_select <= 2'h0;
        else if (state == STATE_READ) begin
            case (target)
                default:
                ADDR_RECEIVER: spi_select <= SPI_RECEIVER;
            endcase
        end
        else spi_select <= spi_select;
    end
    
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n) begin
            start_receiver_spi <= 0;
        end
        else if (state == STATE_READ) begin
            start_receiver_spi <= 0;
            
            case (target) 
                default:
                ADDR_RECEIVER: start_receiver_spi <= 1;
            endcase
        end
    end
    
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n) state <= STATE_IDLE;
        else          state <= next_state;
    end
    
    always @(*) begin
        case (state)
        STATE_IDLE: begin
            if (spi_in == ADDR_RECEIVER ||
                spi_in == ADDR_ALTIMETER ||
                spi_in == ADDR_GPS) begin
                
                next_state <= STATE_RW;
            end else next_state <= STATE_IDLE;
        end
        
        STATE_RW: begin
            if (spi_in[0] == READ) next_state <= STATE_READ;
            else                   next_state <= STATE_WRITE;
        end
        
        STATE_READ: begin
            next_state <= STATE_READING;
        end
        
        STATE_READING: begin
            if (receiver_done) next_state <= STATE_IDLE;
            else               next_state <= STATE_READING;
        end
        
        STATE_WRITE: begin
            if (spi_data_rdy) next_state <= STATE_WRITING1;
            else              next_state <= STATE_WRITE;
        end
        endcase
    end
endmodule
