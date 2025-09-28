from bin_dispose import *	# 将 bin_dispose 模块内的所有内容导入到当前的 命名空间
from crc_module import *	# 将 crc_module 模块内的所有内容导入到当前的 命名空间
from serial_port import *

Bin_File_Path = "H:/Personal_Project_V1.0/Working_Area/python/OTA_Tool/bin_test_1.bin"


if __name__ == "__main__":
	binary_str = Read_Bin_File(Bin_File_Path).encode('UTF-8')	# 将读取的字符串 强制转换为 字节
	print(binary_str)	# 字节集
	bin_byte_array = Bin_Data_Convert_Bytes(binary_str)
	print(bin_byte_array)	# 十进制显示

	# Bin_Data_Convert_Bytes()
	# print("hex_str[0]=", hex_str[0], "hex_str[1]=", hex_str[1])
	hex_crc_data = Bytes_CRC_Check_Func(bin_byte_array, False)
	print("hex_crc_data = ", hex_crc_data)
	

	User_Serial_Port = Serial_Port_APP_Init("CH340", 921600)
	if(User_Serial_Port != False):
		Serial_Transfer(User_Serial_Port, bin_byte_array,50)