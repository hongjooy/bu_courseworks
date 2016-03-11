`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    02:34:22 12/08/2014 
// Design Name: 
// Module Name:    Controller3 
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
`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:27:54 12/07/2014 
// Design Name: 
// Module Name:    Controller2 
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
module Controller4 (PCSource, ALUOp, IRWrite, ALUSrcA, ALUSrcB, RegWrite, MemtoReg, 
PCWrite, ReadSel,  MemWrite, PCWriteCond, 
state, clk, reset, op_in);

parameter state0 = 4'b0000, state1 = 4'b0001, state2 = 4'b0010, 
          state3 = 4'b0011, state4 = 4'b0100, state5 = 4'b0101, state6 = 4'b0110, 
			 state7 = 4'b0111, state8 = 4'b1000, state9 = 4'b1001, state10 = 4'b1010, state11 = 4'b1011,
			 state12 = 4'b1100;

input clk, reset;
input [5:0] op_in;

output reg [3:0] state, ALUOp; //ALUOp: for computation
output reg [1:0] PCSource, ALUSrcB, ALUSrcA, MemtoReg; // One more option is added for the 0 (swi, lwi)

output reg RegWrite,IRWrite,  PCWrite, ReadSel, MemWrite, PCWriteCond;

reg [3:0] next_state;



always @ (posedge clk)
begin
state <= next_state;
end

always @(posedge clk or state or reset)
begin
if (reset)
begin
ALUOp <= 4'b0000;
PCSource <= 2'b00;
ALUSrcA <= 2'b00;
ALUSrcB <= 2'b01;
IRWrite <= 1'b0;
RegWrite <= 1'b0;
MemtoReg <= 2'b00;
PCWrite <= 1'b1;
ReadSel <= 1'b1;
MemWrite <= 1'b0;
PCWriteCond <= 1'b0;
next_state <= state0;
end
else
begin
	case (state)
	state0: 
		begin
ALUOp <= 4'b0010;
PCSource <= 2'b00;
ALUSrcA <= 2'b00;
ALUSrcB <= 2'b01;
IRWrite <= 1'b1;
RegWrite <= 1'b0;
MemtoReg <= 2'b00;
PCWrite <= 1'b1;
ReadSel <= 1'b0;
MemWrite <= 1'b0;
PCWriteCond <= 1'b0;
		next_state <= state1;
		end
	
	
	state1: 
		begin
		RegWrite <= 1'b0;
		MemtoReg <= 2'b00;

		ReadSel <= 1'b0;
		MemWrite <= 1'b0;
		PCWriteCond <= 1'b0;
		IRWrite <= 1'b0;
		ALUSrcA <= 2'b00;
		ALUSrcB <= 2'b10; //Sign Extended for Branch address calculation
		ALUOp <= 4'b0010;
		PCSource <= 2'b00;
		PCWrite <= 1'b0;
		if (op_in == 6'b000000) //  noop
		begin
		next_state <= state1;
		end
		else if (op_in == 6'b010000 || op_in == 6'b010001 || op_in == 6'b010010 || op_in == 6'b010011 || op_in == 6'b010100 || op_in == 6'b010101 || op_in == 6'b 010110 || op_in == 6'b010111) // R-type operations goes to state 3
		begin
		next_state <= state2;
		end
		else if (op_in == 6'b000001) // Jump
		begin
		next_state <= state3;
		end
		else if (op_in == 6'b100000) // beq
		begin
		next_state <= state4;
		end
		else if (op_in == 6'b110100 || op_in == 6'b110101 || op_in == 6'b110111)
		begin
		next_state <= state5;
		end
		else if (op_in == 6'b 110010 || op_in == 6'b 110011 || op_in == 6'b110100)
		begin
		next_state <= state6;
		end
		else if (op_in == 6'b111001 || op_in == 6'b111010 || op_in == 6'b111011 || op_in == 6'b111100) // load store
		begin
		next_state <= state7;
		end
		end
		
	state2: // R-TYPE
		begin
	
		PCSource <= 2'b00;

		IRWrite <= 1'b0;
		RegWrite <= 1'b0;
		MemtoReg <= 2'b00;
		PCWrite <= 1'b0;
		ReadSel <= 1'b1;
		MemWrite <= 1'b0;
		PCWriteCond <= 1'b0;
		ALUSrcA <= 2'b01;
		ALUSrcB <= 2'b00;
	
		ALUOp <= op_in[3:0];
		next_state <= state8;
		end
		
	state3: //JUMP
		begin
		ALUOp <= 4'b0010;
		ALUSrcA <= 2'b00;
		ALUSrcB <= 2'b10;
		IRWrite <= 1'b0;
		RegWrite <= 1'b0;
		MemtoReg <= 2'b00;
		ReadSel <= 1'b1;
		MemWrite <= 1'b0;
		PCWriteCond <= 1'b0;
		PCWrite <= 1'b1;
		PCSource <= 2'b10;
		next_state <= state0;
		end
		
	state4: // BEQ
		begin
		IRWrite <= 1'b0;
		RegWrite <= 1'b0;
		MemtoReg <= 2'b00;
		PCWrite <= 1'b0;
		ReadSel <= 1'b0;
		MemWrite <= 1'b0;
		ALUSrcA <= 2'b01;
		ALUSrcB <= 2'b00;
		ALUOp <= 4'b0011; //Sub;
		PCWriteCond <= 1'b1;
		PCSource <= 2'b01;
		next_state <= state0;
		end
		
	state5: //ZE Operations
		begin
		

		PCSource <= 2'b00;

		IRWrite <= 1'b0;
		RegWrite <= 1'b0;
		MemtoReg <= 2'b00;
		PCWrite <= 1'b0;
		ReadSel <= 1'b1;
		MemWrite <= 1'b0;
		PCWriteCond <= 1'b0;
		ALUSrcA <= 2'b01;
		ALUSrcB <= 2'b11;
		ALUOp <= op_in[3:0];
		next_state <= state8;
		end
		
	state6: // SE Operations
		begin
				
		PCSource <= 2'b00;

		IRWrite <= 1'b0;
		RegWrite <= 1'b0;
		MemtoReg <= 2'b00;
		PCWrite <= 1'b0;
		ReadSel <= 1'b0;
		MemWrite <= 1'b0;
		PCWriteCond <= 1'b0;
		ALUSrcA <= 2'b01;
		ALUSrcB <= 2'b10;
		ALUOp <= op_in[3:0];
		next_state <= state8;
		end
		
	state7: // LI, LUI, SWI, LWI
		begin
			
		PCSource <= 2'b00;

		IRWrite <= 1'b0;
		RegWrite <= 1'b0;
		MemtoReg <= 2'b00;
		PCWrite <= 1'b0;
		ReadSel <= 1'b0;
		MemWrite <= 1'b0;
		PCWriteCond <= 1'b0;
		ALUSrcA <= 2'b10;
		ALUSrcB <= 2'b11;
		ALUOp <= 4'b0100;
		if (op_in == 6'b111001) //li
		begin
		next_state <= state9;
		end
		else if (op_in <= 6'b111010) //lui
		begin
		next_state <= state10;
		end
		else if (op_in <= 6'b111011) // lwi
		begin
		next_state <= state11;
		end
		else if (op_in <= 6'b111100) // swi
		begin
		next_state <= state12;
		end
		
		end
	state8:
		begin	
		ALUOp <= op_in[3:0];
		PCSource <= 2'b00;
		ALUSrcA <= 2'b00;
		ALUSrcB <= 2'b00;
		IRWrite <= 1'b0;

		PCWrite <= 1'b0;
		ReadSel <= 1'b1;
		MemWrite <= 1'b0;
		PCWriteCond <= 1'b0;
		RegWrite <= 1'b1;
		MemtoReg <= 2'b00;
		next_state <= state0;
		end
		
	state9: // li
		begin
		ALUOp <= 4'b0000;
		PCSource <= 2'b00;
		ALUSrcA <= 2'b00;
		ALUSrcB <= 2'b00;
		IRWrite <= 1'b0;

		PCWrite <= 1'b0;
		ReadSel <= 1'b0;
		MemWrite <= 1'b0;
		PCWriteCond <= 1'b0;
		RegWrite <= 1'b1;
		MemtoReg <= 2'b10;
		next_state <= state0;
		end
		
	state10: // lui
		begin
		ALUOp <= 4'b0000;
		PCSource <= 2'b00;
		ALUSrcA <= 2'b00;
		ALUSrcB <= 2'b00;
		IRWrite <= 1'b0;

		PCWrite <= 1'b0;
		ReadSel <= 1'b0;
		MemWrite <= 1'b0;
		PCWriteCond <= 1'b0;
		RegWrite <= 1'b1;
		MemtoReg <= 2'b11;
		next_state <= state0;
		end
		
	state11:
		begin
		ALUOp <= 4'b0000;
		PCSource <= 2'b00;
		ALUSrcA <= 2'b00;
		ALUSrcB <= 2'b00;
		IRWrite <= 1'b0;

		PCWrite <= 1'b0;
		ReadSel <= 1'b0;
		MemWrite <= 1'b0;
		PCWriteCond <= 1'b0;
		RegWrite <= 1'b1;
		MemtoReg <= 2'b01;
		next_state <= state0;
		
		end
		
	state12:
		begin
		ALUOp <= 4'b0000;
		PCSource <= 2'b00;
		ALUSrcA <= 2'b00;
		ALUSrcB <= 2'b00;
		IRWrite <= 1'b0;
		RegWrite <= 1'b0;
		MemtoReg <= 2'b00;
		PCWrite <= 1'b0;
		ReadSel <= 1'b0;

		PCWriteCond <= 1'b0;
		MemWrite <= 1'b1;
		next_state <= state0;
		end
	endcase
	
	end
	end
	
		
endmodule