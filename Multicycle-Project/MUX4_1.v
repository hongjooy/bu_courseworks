`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    15:00:45 12/05/2014 
// Design Name: 
// Module Name:    MUX4_1 
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
module MUX4_1(MuxOut, MuxIn0, MuxIn1, MuxIn2, MuxIn3, Sel);

parameter DATA_WIDTH = 32; 

input [1:0] Sel;
input [DATA_WIDTH-1:0] MuxIn0, MuxIn1, MuxIn2, MuxIn3;
output reg [DATA_WIDTH-1:0] MuxOut;

always @ (MuxIn0 or MuxIn1 or MuxIn2 or MuxIn3 or Sel)
begin
		case (Sel)
			2'b00: MuxOut = MuxIn0; 
			2'b01: MuxOut = MuxIn1;
			2'b10: MuxOut = MuxIn2; //LI
			2'b11: MuxOut = MuxIn3; //LUI
		endcase
end

endmodule
