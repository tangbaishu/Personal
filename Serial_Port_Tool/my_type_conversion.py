
def string_cover_uint(source_string):
	dest_uint = [0] * len(source_string)
	sub = 0
	print(f"len(source_string) = {len(source_string)}")
	for char in source_string:
		if char.isdigit():		# 是否为数字
			dest_uint[sub] = int(char)
		else:
			dest_uint[sub] = ord(char)
		sub += 1
	return dest_uint

if __name__ == "__main__":
	uint_array = string_cover_uint("test_654321")
	print(f"len(uint_array) = {len(uint_array)}")
	for i in range(0, len(uint_array), 1):
		print(f"uint_array[{i}] = {uint_array[i]}")