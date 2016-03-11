`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   02:37:29 12/08/2014
// Design Name:   Datapath2
// Module Name:   /ad/eng/users/h/o/hongjooy/Xlinix/New Folder/Project/new_test2.v
// Project Name:  Project
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: Datapath2
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module new_test2;

	// Inputs
	reg clk;
	reg reset;

	// Outputs
	wire [31:0] ALUOut;
	wire [31:0] Instr;
	wire [3:0] state;
	wire [31:0] PCCount;

	// Instantiate the Unit Under Test (UUT)
	Datapath2 uut (
		.clk(clk), 
		.reset(reset), 
		.ALUOut(ALUOut), 
		.Instr(Instr), 
		.state(state), 
		.PCCount(PCCount)
	);

	
	initial begin
		// Initialize Inputs
		clk = 0;
		reset = 1;

		// Wait 100 ns for global reset to finish
		#93;
		reset = 0;
        
		// Add stimulus here

	end
	
	always begin
	#20 clk = ~clk;
	
	end
      
endmodule

