`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    00:09:33 11/24/2014 
// Design Name: 
// Module Name:    MUX31 
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
module MUX31(MuxOut, MuxIn0, MuxIn1, MuxIn2, Sel);

parameter DATA_WIDTH = 32;
input [DATA_WIDTH-1:0] MuxIn0, MuxIn1, MuxIn2;
input [1:0]Sel;
output reg [DATA_WIDTH-1:0] MuxOut;

always @ (Sel, MuxIn0, MuxIn1, MuxIn2)			

	begin
		case (Sel)
			2'b00: MuxOut = MuxIn0; 
			2'b01: MuxOut = MuxIn1;
			2'b10: MuxOut = MuxIn2;
		endcase
	end
endmodule
