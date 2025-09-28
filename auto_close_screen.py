<<<<<<< HEAD
# import win32gui
# import win32con
import ctypes
import time

delay_minutes = 60
=======
import win32gui
import win32con
import time

delay_minutes = 1
>>>>>>> 6885071a4f0b6edc131375d09f4455e538c6f1b6

def lock_screen_after_delay(delay_minutes):
    """
    设置电脑在指定时间后自动锁屏。
    
    :param delay_minutes: 延迟时间（分钟）
    """
<<<<<<< HEAD
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
=======
    time.sleep(delay_minutes * 10)  # 等待指定的分钟数
    win32gui.LockWorkStation()  # 调用Windows API锁屏

if __name__ == "__main__":
    print(f"电脑将在 {delay_minutes} 分钟后自动锁屏。")
    lock_screen_after_delay(delay_minutes)
>>>>>>> 6885071a4f0b6edc131375d09f4455e538c6f1b6
