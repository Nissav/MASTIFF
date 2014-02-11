module motorDirDecode (A1, A2, A3, A4, in0, in1);
	//States
	//0:	both on, right forward, left reverse	0110
	//1:	both on, left forward, right reverse	1001
	//2:	both forward							1010
	//3:	both reverse 							0101
	//A1 & A2 are for left, A3 & A4 are for right
	
	input	in0, in1;
	output	A1, A2, A3, A4;
	
	assign A1 = ~in1 & in0 | in1 & ~in0;
	assign A2 = ~in1 & ~in0 | in1 & in0;
	assign A3 = ~in1 & ~in0 | in1 & ~in0;
	assign A4 = ~in1 & in0 | in1 & in0;
	

	endmodule 