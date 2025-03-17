import win32gui
import win32con
import time

delay_minutes = 1

def lock_screen_after_delay(delay_minutes):
    """
    设置电脑在指定时间后自动锁屏。
    
    :param delay_minutes: 延迟时间（分钟）
    """
    time.sleep(delay_minutes * 10)  # 等待指定的分钟数
    win32gui.LockWorkStation()  # 调用Windows API锁屏

if __name__ == "__main__":
    print(f"电脑将在 {delay_minutes} 分钟后自动锁屏。")
    lock_screen_after_delay(delay_minutes)