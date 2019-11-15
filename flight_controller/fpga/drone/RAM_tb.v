`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   19:35:42 09/04/2016
// Design Name:   CPU
// Module Name:   C:/Users/Robert/Documents/Hobbies/Quadcopter/flight_controller/fpga/drone/RAM_tb.v
// Project Name:  drone
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: CPU
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////
`define CLK  50

module RAM_tb;

	// Inputs
	reg clk_system;
	reg reset_n;
	reg [7:0] rd_addr;
	reg rd;
	reg [7:0] wr_addr;
	reg wr;
	reg [7:0] wr_data;

    reg [7:0] data;
    reg [7:0] addr;
    
    reg failure;
    
	// Outputs
	wire [7:0] rd_data;

	// Instantiate the Unit Under Test (UUT)
	RAM uut (
		.clk_system(clk_system), 
		.reset_n(reset_n), 
		.rd_addr(rd_addr), 
		.rd(rd), 
		.wr_addr(wr_addr), 
		.wr(wr), 
		.wr_data(wr_data), 
		.rd_data(rd_data)
	);

    /* create a 20MHz system clock */
    always begin
        clk_system = 0; #(`CLK / 2);
        clk_system = 1; #(`CLK / 2);
    end
    
	initial begin
		// Initialize Inputs
		failure = 0;
        
		rd_addr = 0;
		rd = 0;
		wr_addr = 0;
		wr = 0;
		wr_data = 0;

		// Wait 100 ns for global reset to finish
		reset_n = 0; #(`CLK * 2);
        reset_n = 1;
        
		// Add stimulus here
        data = 8'h72;
        addr = 8'h00;
        CPU_write(addr, data);
        CPU_read(addr);
                
        validate_io(addr, data);        
        #(`CLK * 4);
        
        data = 8'h00;
        CPU_read(8'h01);
        
        validate_io(addr, data);
        
        addr = 8'h01;
        data = 8'hAA;
        CPU_write(addr, data);
        CPU_read(addr);

        validate_io(addr, data);

        if (!failure) $display("SUITE PASSED");
        else          $display("SUITE FAILED");
        
        #(`CLK * 2) $finish;
	end
    
    task CPU_write;
        input [7:0] addr;
        input [7:0] data;
        
        begin
            @(negedge clk_system);
            wr_addr = addr;
            wr_data = data;
            
            wr = 1; #`CLK;
            wr = 0; #`CLK;
        
            wr_data = 0;
            wr_addr = 0;
        end
    endtask
    
    task CPU_read;
        input [7:0] addr;
        
        begin
            @(negedge clk_system);
            rd_addr = addr;
            
            rd = 1; #`CLK;
            rd = 0; #`CLK;
            
            rd_addr = 0;
        end
    endtask
    
    task validate_io;
        input [7:0] address;
        input [7:0] expected_data;
        
        begin
            if (rd_data != expected_data) begin
                $display("Read/Write Failure:");
                $display("    Address: %02x", address);
                $display("    Expected Data: %02x", expected_data);
                $display("    Actual Data:   %02d", rd_data);
                
                failure = 1;
            end
        end
    endtask
    
endmodule

