`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    06:47:50 08/17/2016 
// Design Name: 
// Module Name:    SPISlave 
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
module SPISlave(
    input  clk_system,
    input  mosi,
    input  sclk,
    input  slave_select_n,
    input  latch,
    input  [7:0] wr_data,
    input  reset_n,
    output reg new_data,
    output reg miso,
    output reg [7:0] rd_data
    );

    /* buffers for miso and mosi */
    reg [7:0] in;
    reg [7:0] out;
    
    /* count the number of clock pulses to determine when data is ready */
    reg [3:0] clocks;
    
    /* load the data to be written out */
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n)   out <= 8'h00;
        else if (latch) out <= wr_data;
        else            out <= out;
    end
    
    /* increment a counter to track clock pulses */
    always @(posedge sclk, negedge reset_n) begin
        if (!reset_n) clocks <= 4'h0;
        else          clocks <= clocks + 1;
    end
    
    /* 
     * after 8 clock pulses, propogate the data from the input
     * buffer shift register to the output register
     */
    always @(posedge clk_system) begin
        if (clocks == 4'h8) begin
            clocks <= 4'h0;
            rd_data <= in;
        end
    end
    
    /* 
     * after 8 clock pulses, indicate that there is new data in
     * the output register
     */
    always @(posedge clk_system, negedge reset_n) begin
        if (clocks == 4'h8 &&
            !slave_select_n) new_data <= 1'h1;
        else                 new_data <= 1'h0;
    end
     
    /* 
     * shift bits from mosi into the input buffer shift register 
     * on each positive edge of sclk 
     */
    always @(posedge sclk) begin
        in <= { in[6:0], mosi };
    end
    
    /*
     * shift bits out of the output buffer shift register on each
     * negative edge of sclk, which propogate to miso on the next
     * positive edge of the system clock given that the module's
     * slave select is being driven low
     */
    always @(negedge sclk) begin
        out <= { out[6:0], 1'h0 };
    end
    
    /* control the miso line when slave select is low */
    always @(posedge clk_system, negedge reset_n) begin
        if (!reset_n)            miso <= 1'hZ;
        else if (slave_select_n) miso <= 1'hZ;
        else                     miso <= out[7];
    end
    
endmodule
