module Sign_Extend(
	data_i,	// inst[15:0]
	data_o	// MUX_ALUSrc.data2_i
);

input	[15:0]	data_i;
output	[31:0]	data_o;

reg		[31:0]	tmp;
assign	data_o = tmp;

always@(*) begin
	tmp[15:0] = data_i[15:0];
	tmp[31:16] = data_i[15];
end

endmodule
