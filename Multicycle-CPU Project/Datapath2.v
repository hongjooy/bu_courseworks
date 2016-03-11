`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    00:46:29 12/06/2014 
// Design Name: 
// Module Name:    Datapath2 
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
module Datapath2(clk, reset, ALUOut, Instr, state, PCCount);

parameter DATA_WIDTH = 32;
parameter REG_LENTH = 5;

input clk, reset;
output [DATA_WIDTH-1:0] Instr;
output [DATA_WIDTH-1:0] PCCount;
output [DATA_WIDTH-1:0] ALUOut;
output [3:0] state;


//===================== Controller Signals ==================
wire [3:0] ALUOp; //ALUOp: for computation
wire [1:0] PCSource, ALUSrcB, ALUSrcA, MemtoReg; // One more option is added for the 0 (swi, lwi)

wire Zero, RegWrite,IRWrite, PCWrite, ReadSel, MemWrite, PCWriteCond, pc_wire, PCEn;

and and_ (pc_wire, Zero, PCWriteCond);
or or_ (PCEn, PCWrite, pc_wire);


//========================= Control ==========================================
Controller4 Controlunit (PCSource, ALUOp, IRWrite, ALUSrcA, ALUSrcB, RegWrite, MemtoReg, 
PCWrite, ReadSel,  MemWrite, PCWriteCond, 
state, clk, reset, Instr[31:26]);


//==================== Datapath Wires ================================

wire [DATA_WIDTH-1:0] PCIn, Instr1, ReadData11, ReadData1, ReadData21, ReadData2, Operand1, Operand2;
wire [DATA_WIDTH-1:0] ALUResult, ALUOut, MemData1, MemData, WriteData, SE_out, ZE_out;
wire [REG_LENTH-1:0] ReadReg2;
//============================= Instruction Memory ========================

IMem #(DATA_WIDTH)InstrMem (PCCount, Instr1);

//============================= General Purpose Register ====================

nbit_reg #(DATA_WIDTH) PCReg (PCIn, PCCount, PCEn, reset, clk);
nbit_reg #(DATA_WIDTH) InstrReg (Instr1, Instr, IRWrite, reset, clk);
nbit_reg #(DATA_WIDTH) A (ReadData11, ReadData1, 1'b1, reset, clk);
nbit_reg #(DATA_WIDTH) B (ReadData21, ReadData2, 1'b1, reset, clk);
nbit_reg #(DATA_WIDTH) ALUReg (ALUResult, ALUOut, 1'b1, reset, clk);
nbit_reg #(DATA_WIDTH) MemReg (MemData1, MemData, 1'b1, reset, clk);

//========================== Data Memory =============================

DMem #(DATA_WIDTH) Datamem (ReadData2, MemData1, ALUOut, MemWrite, clk);

//========================== ALU ====================================


Ideal_ALU #(DATA_WIDTH)	ALU_ (Zero ,ALUResult, Operand1, Operand2, ALUOp);

//========================== MUX ======================================

MUX31 #(DATA_WIDTH) PCMux (PCIn, ALUResult, ALUOut, ALUOut, PCSource);
MUX21 #(REG_LENTH) ReadSelMux (ReadReg2, Instr[25:21], Instr[15:11], ReadSel);
MUX4_1 #(DATA_WIDTH) MemtoRegMux (WriteData, ALUOut, MemData, {ReadData2[31:16], ALUOut[15:0]}, {ALUOut[15:0],ReadData2[15:0]}, MemtoReg);
MUX31  #(DATA_WIDTH) SrcA (Operand1, PCCount, ReadData1, 32'h00000000, ALUSrcA);
MUX4_1 #(DATA_WIDTH)SrcB (Operand2, ReadData2, 32'h00000001,SE_out, ZE_out, ALUSrcB);


//============================= Extentions =============================
SE	SE1 (SE_out, Instr[15:0]);
//Shif16 Sht161 (Sht_out, SE_out);
ZE ZE1 (ZE_out, Instr[15:0]);

//======================== Register File ===================================

nbit_register_file #(DATA_WIDTH, 5) RF (WriteData, ReadData11, ReadData21, Instr[20:16], ReadReg2, Instr[25:21], RegWrite, clk);


endmodule
