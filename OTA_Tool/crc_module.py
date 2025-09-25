import array

# brief@ CRC32循环冗余校验 函数
# param@ data_32bit: 待校验数据(32位 十六进制码)
# param@ poly: 进行循环冗余校验的多项式
def CRC32_Calculate(data_32bit, en_debug, poly=0x4C11DB7):
	if(en_debug != 0):						# 若 en_debug==1,打印CRC32计算的相关数据
		print("CRC32_Calculate() -> Input_Data:")
	# 将32位整数数据转换为4字节的字节序列
	crc = 0xffffffff						# 初始化CRC寄存器值为0xFFFFFFFF
	for byte in data_32bit:						# 遍历 待校验数据 中的每个字节
		if(en_debug != 0):					# 若 en_debug==1 ,打印CRC32计算的相关数据
			print(f"0x{byte:08x}")			 
		crc ^= byte							# 将CRC寄存器的当前值与当前字节进行异或运算
		for _ in range(32):					# 进行32次循环，对应于待校验数据的 32位
			if crc & 0x80000000:			# 如果CRC寄存器的最低位为1，则与 生成多项式poly 进行异或运算
				crc = (crc << 1 ) ^ poly
			else:
				crc = crc << 1
	crc &= 0xffffffff						 # 确保CRC寄存器保持32位
	return crc

# brief@ 8位字节数据集 转为 32位数据集
# param@ bytes 8位字节数据集
def Byte_Convert_32bit(bytes):
	# 创建一个32位整数数组
	data_32bit = array.array('I')				# 'I' : 表示无符号整数			
	cycle_num = int(len(bytes) / 4)				# 获取传入 字节列表长度
	sub = 0
	# print("Byte_Convert_32bit()")
	# print("bytes len = ", cycle_num)
	for i in range(0, cycle_num, 1):			# 4个字节 合成 一个32位数据
		data_32bit.append(bytes[sub]  | bytes[sub+1]<<8 | bytes[sub+2]<<16 | bytes[sub+3]<<24)
		sub += 4
	if(len(bytes)%4 != 0):						# 若传入数据长度无法被 4 整除
		last_data = 0
		num = len(bytes) - sub
		for i in range(0, num, 1):
			last_data |=  bytes[sub] << (i*8)
			sub += 1
		# print("last_data = ", last_data)
		data_32bit.append(last_data)
	# print("data_32bit = ", data_32bit)
	return data_32bit

# brief@ 对 8位字节数据集 进行 CRC校验 函数
# param@ Bytes: 8位字节数据集
# param@ en_debug: 是否打印debug数据
def Bytes_CRC_Check_Func(Bytes, en_debug):
	data_32bit = Byte_Convert_32bit(Bytes)
	crc_check_code = CRC32_Calculate(data_32bit, en_debug)
	return crc_check_code


# def crc32(data_int):
#     # """
#     # 计算给定32位整数数据的CRC-32校验码。
    
#     # 参数:
#     # data_int (int): 需要计算CRC-32的32位整数数据。
    
#     # 返回:
#     # int: 计算得到的CRC-32校验码。
#     # """
#     # 将32位整数数据转换为4字节的字节序列
#     data_bytes = struct.pack('<I', data_int)  # '<I'表示小端序的无符号整数
    
#     # 初始化CRC寄存器值为0xFFFFFFFF
#     crc = 0xffffffff
    
#     # 遍历数据中的每个字节
#     for byte in data_bytes:
#         # 将CRC寄存器的当前值与当前字节进行异或运算
#         crc = crc ^ byte
#         # 进行8次循环，对应于一个字节的8位
#         for _ in range(8):
#             # 如果CRC寄存器的最低位为1，则与生成多项式0xEDB88320进行异或运算
#             if (crc & 1) != 0:
#                 crc = (crc >> 1) ^ 0xedb88320
#             else:
#                 # 否则，将CRC寄存器向右移动一位
#                 crc = crc >> 1
#             # 确保CRC寄存器保持32位
#             crc &= 0xffffffff
    
#     # 返回最终的CRC-32校验码，与0xFFFFFFFF进行异或运算后的结果
#     return crc ^ 0xffffffff

# 使用示例
# data_int = 0x12345678  # 一个32位的整数
# crc_value = crc32(data_int)
# print(f"CRC-32 value: {crc_value:#010x}")  # 以十六进制形式打印CRC-32值

if __name__ == "__main__":
	print("CRC_Module Start")
	byte_array = [0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x20,0x21]
	print(byte_array)
	crc_value = Bytes_CRC_Check_Func(byte_array, 1)
	print(f"CRC-32 Hex: 0x{crc_value:08x}", "value = ", crc_value)

	byte_array = [0x03020100, 0x07060504, 0x11100908, 0x15141312, 0x19181716]
	print(byte_array)
	crc_value = CRC32_Calculate(byte_array, 1)
	print(f"CRC-32 Hex: 0x{crc_value:08x}", "value = ", crc_value)

	
