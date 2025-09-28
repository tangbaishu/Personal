import array


def Read_Bin_File(file_path):
	with open(file_path, "rb") as file:
		data = file.read()
		bin_data = data.hex()
		print("hex_str len =",len(bin_data))
	return bin_data

def Bin_Data_Convert_Bytes(bin_data):
	bin_byte_array = array.array('B')	# 
	for i in range (0, len(bin_data), 2):
		if( bin_data[i] < 58):
			h_bit_4 = bin_data[i] - 48
		else:
			h_bit_4 = bin_data[i] - 97 + 10
		if( bin_data[i+1] < 58):
			l_bit_4 = bin_data[i+1] - 48
		else:
			l_bit_4 = bin_data[i+1] - 97 + 10
		bin_byte_array.append(h_bit_4 << 4 | l_bit_4)
	return bin_byte_array
		

if __name__ == "__main__":
	Bin_File_Path = "H:/Personal_Project_V1.0/Working_Area/python/OTA_Tool/bin_test_1.bin"
	Hex_File_Path = "H:/Personal_Project_V1.0/Working_Area/python/OTA_Tool/bin_test.bin"
	binary_str = Read_Bin_File(Bin_File_Path).encode('UTF-8')	# 将读取的字符串 强制转换为 字节
	print(binary_str)	# 字节集
	bin_byte_array = Bin_Data_Convert_Bytes(binary_str)
	print(bin_byte_array)	# 十进制显示
	print("for end")