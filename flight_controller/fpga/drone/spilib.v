//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:34:08 09/11/2016 
// Design Name: 
// Module Name:    spilib 
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
`ifndef CLK
`define CLK 50
`endif

`ifndef CLK_SPI
`define CLK_SPI 400
`endif

task spi_send;
    input miso;
    input [7:0] master_out;
    
    output sclk;
    output [7:0] miso_data;
    output mosi;
    
    LOAD : begin      
        integer i;
        for (i = 0; i < 8; i = i + 1) begin
            // put data on mosi on the negative edge of sclk
            sclk = 1'h0; 
            mosi = master_out[7-i];
            #(`CLK_SPI / 2);
        
            sclk = 1'h1;
            miso_data = { miso_data[6:0], miso };
            #(`CLK_SPI / 2);
            
            sclk = 0;
        end
    end
endtask

task spi_load;
    input [7:0] data;
    output latch;
    output [7:0] wr_data;
    
    begin
        wr_data = data; #`CLK;
        
        latch = 1; 
        #`CLK;
        latch = 0; 
        #`CLK;
    end
endtask
