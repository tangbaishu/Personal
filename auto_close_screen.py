# import win32gui
# import win32con
import ctypes
import time

delay_minutes = 60

def lock_screen_after_delay(delay_minutes):
    """
    设置电脑在指定时间后自动锁屏。
    
    :param delay_minutes: 延迟时间（分钟）
    """
    time.sleep(delay_minutes * 60)  # 等待指定的分钟数
    try:
        ctypes.windll.user32.LockWorkStation()  # 调用Windows API锁屏
        print("电脑已成功锁屏。")
    except Exception as e:
        print(f"锁屏失败：{e}")

if __name__ == "__main__":
    print(f"电脑将在 {delay_minutes} 分钟后自动锁屏。")
    while 1:
        lock_screen_after_delay(delay_minutes)