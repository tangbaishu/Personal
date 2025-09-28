import serial.tools
import serial.tools.list_ports
import time
import array


# 获取所有可用的串行端口
Ports = serial.tools.list_ports.comports()
Receive_UTF8 = "这是一个全局变量"


# 变量可用的串行端口，并输出第一个检索到的
def Get_Serial_Port_Info(device_info):
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

def Serial_Port_APP_Init(select_device_info, baud_rate):
	Port_Name = Get_Serial_Port_Info(select_device_info) 
	if "COM" in Port_Name:					# 进一步判断是否存在可用端口
		print(f"准备打开串口：{Port_Name}")
		User_Serial_Port = serial.Serial(Port_Name, baudrate=baud_rate,timeout=1)
		if User_Serial_Port.is_open:
			print(f"串口{Port_Name}已打开")
			return User_Serial_Port
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
	else:
		print("无可用串行端口")
	return False

def Serial_Transfer(User_Serial_Port, send_byte_array, once_send_len):
	trigger_target_string = 0
	send_finish_flag = 0
	while True:
		try:
			if(send_finish_flag == 0):
				try:
					for i in range (0, int(len(send_byte_array) / once_send_len), 1):
						# cache_data = (send_byte_array[i*once_send_len : once_send_len + i*once_send_len]).encode()
						cache_data = (send_byte_array[i*once_send_len : once_send_len + i*once_send_len])
						print(cache_data)
						User_Serial_Port.write((cache_data))	# 发送字符串数据，记得转换为字节串
						time.sleep(0.5)
					if(int(len(send_byte_array) % once_send_len ) != 0):
						# cache_data = (send_byte_array[(int(len(send_byte_array) / once_send_len) * once_send_len) : len(send_byte_array)]).encode()
						cache_data = (send_byte_array[(int(len(send_byte_array) / once_send_len) * once_send_len) : len(send_byte_array)])
						print(cache_data)
						User_Serial_Port.write((cache_data))	# 发送字符串数据，记得转换为字节串
						time.sleep(0.5)
					send_finish_flag = 1
				except serial.SerialException as e:
					print(f"写入串口失败: {e}")
			if User_Serial_Port.in_waiting > 0:
				print("接受的数据长度=", User_Serial_Port.in_waiting)
				read_data = User_Serial_Port.readline()
				# print("read_data = ", read_data)
				# receive_string = read_data.decode('utf-8')
				receive_string = read_data.decode().strip()
				Receive_UTF8 = receive_string
				print(f"Rec: {Receive_UTF8}")
				if "CRC ERR regAddr=" in Receive_UTF8:
					trigger_target_string = Receive_UTF8
			else:
				time.sleep(1)
				if trigger_target_string != 0:
					print(f"接收到目标字符串：{trigger_target_string}");
				# print("runing")
		except serial.SerialException as e:
					print(f"串口通信错误：{e}")
					break

if __name__ == "__main__":
	# test_str = "test_str12345"
	test_str = [0,1,2,3,4,5,6]
	User_Serial_Port = Serial_Port_APP_Init("CH340", 921600)
	if(User_Serial_Port != False):
		# test_str = array.array('B');
		Serial_Transfer(User_Serial_Port, test_str,3)
	
	# Send_Info()
	input()