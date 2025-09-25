import serial
import time
import copy
import struct

import serial.tools
import serial.tools.list_ports

# 获取所有可用的串行端口
Ports = serial.tools.list_ports.comports()
Receive_UTF8 = "这是一个全局变量"

def crc32(data, poly=0x4C11DB7):
	# 将32位整数数据转换为4字节的字节序列
	print(data)
	crc = 0xffffffff
	for byte in data:
		print(f"{byte:08x}")
		crc ^= byte
		for _ in range(32):
			if crc & 0x80000000:
				crc = (crc << 1 ) ^ poly
			else:
				crc = crc << 1

	crc &= 0xffffffff
			# crc = (crc >> 1) ^ poly if (crc & 1) else crc >> 1
	return crc

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


# 变量可用的串行端口，并输出第一个检索到的
def Output_Serial_Port_Info(device_info):
	if ( len(Ports) > 0 ):									# 可用串行端口号为0，即存在可用端口
		for port in Ports:
			print(f"找到串行端口:{port.device}")		    # 打印串口号
			print(f"端口描述：{port.description}")		    # 打印端口描述信息
			print(f"端口硬件ID:{port.hwid}")			    # 打印硬件ID
			print("-" * 20)								    # 打印分割线
			if device_info in port.description.strip():		# 作用：判断可用端口信息是否为CH340 USB-TTL端口
														    # "CH340" in ...检查处理后的port.device字符串中是否包含子字符串"CH340" 
														    # strip()方法被用来移除port.device字符串两端的空白字符
				return port.device						    # 返回串行端口号（变量类型：字符串）   
	else:
		return ""
	return ""

def Serial_Port_APP(com_port, baud_rate):
	print(f"准备打开串口：{com_port}")
	User_Serial_Port = serial.Serial(com_port, baudrate=baud_rate,timeout=1)
	if User_Serial_Port.is_open:
		print(f"串口{com_port}已打开")
		User_Serial_Port.write(b'0x40')
		while True:
			try:
				if User_Serial_Port.in_waiting > 0:
					print("接受的数据长度=", User_Serial_Port.in_waiting)
					read_data = User_Serial_Port.readline()
					receive_string = read_data.decode('utf-8')
					Receive_UTF8 = receive_string
					print(f"接受到的数据：{Receive_UTF8}")
					try:
						User_Serial_Port.write(b'Hello, Serial Port!')	# 发送字符串数据，记得转换为字节串
					except serial.SerialException as e:
						print(f"写入串口失败: {e}")
				else:
					time.sleep(1)
					print("runing")
				
			except serial.SerialException as e:
				print(f"串口通信错误：{e}")
				break
	else:
		print("无法打开串口")
	print("close")
	User_Serial_Port.close()


if __name__ == "__main__":
	print("OTA_Updata Start")
	byte_array = bytes()
	byte_array = int(5)
	# byte_array = bytes([0x01,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19])
	byte_array = [0x03020100, 0x07060504, 0x11100908, 0x15141312, 0x19181716]
	print(byte_array)
	# byte_array = {0x03020100}
	crc_value = crc32(byte_array)
	print(f"CRC-32 Hex: 0x{crc_value:08X}", "value = ", crc_value)

	Port_Name = Output_Serial_Port_Info("CH340") 
	if "COM" in Port_Name:					# 进一步判断是否存在可用端口
		Serial_Port_APP(Port_Name, 921600)
	else:
		print("无可用串行端口")
	# Send_Info()
	input()
