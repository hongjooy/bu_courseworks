`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    00:50:20 11/24/2014 
// Design Name: 
// Module Name:    ZE 
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
module ZE(out_32, in_16);

input [15:0] in_16;
output [31:0] out_32;

assign out_32 = {{16'b0000000000000000}, in_16};


endmodule
