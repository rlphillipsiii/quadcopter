`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:  ThirdGen ComPro
// Engineer: Robert Phillips III
// 
// Create Date:    22:27:02 08/11/2016 
// Design Name: 
// Module Name:    ReceiverChannel 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//     Measures a 50Hz PPM signal meant to be sent to an ESC and converts the
//     length of the pulse to a 10 bit number.  If the module goes 2 cycles 
//     without seeing a positive edge signifying the beginning of the pulse,
//     then the module asserts an error bit.  The bit is then deasserted when/if
//     the PPM signal returns.  The minimum duration of the pulse width is 1ms,
//     and the maximimum duration of the pulse width is 2ms.
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module ReceiverChannel(
    input clk_system,
    input reset_n,
    input signal,
    output data,
    output reg [10:0] count
    );
    
    reg ready;
    
    reg [1:0] state;
    reg [1:0] next_state;
    
    reg [10:0] count_buffer;
    
    localparam IDLE     = 0;
    localparam COUNTING = 1;
    localparam LATCH    = 2;
    localparam DATA_RDY = 3;
    
    /////////////////////////////////////////////////////////////
    // Pulse Counter
    //
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n)               count_buffer <= 10'h0;
        else if (state == IDLE)     count_buffer <= 10'h0;
        else if (state == COUNTING) count_buffer <= count_buffer + 1;
        else                        count_buffer <= count_buffer;
    end
    
    /////////////////////////////////////////////////////////////
    // Pulse Counter
    //
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n)            count <= 10'h0;
        else if (state == LATCH) count <= count_buffer;
        else                     count <= count;
    end

    /////////////////////////////////////////////////////////////
    // State Machine
    //        
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n) state <= IDLE;
        else          state <= next_state;
    end
    
    always @(*) begin
        case (state)
        IDLE:     if (signal) next_state <= COUNTING;
                  else        next_state <= IDLE;
                    
        COUNTING: if (!signal) next_state <= LATCH;
                  else         next_state <= COUNTING;
        
        LATCH:    next_state <= DATA_RDY;
        DATA_RDY: next_state <= IDLE;
        default:  next_state <= IDLE;
        endcase
    end
    
    always @(negedge clk_system) begin
        if (!reset_n)               ready <= 1'h0;
        else if (state == DATA_RDY) ready <= 1'h1;
        else                        ready <= 1'h0;
    end
    
    assign data = (ready == 1'h1);
endmodule
