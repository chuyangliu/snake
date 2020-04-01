from datetime import datetime


def log(*msgs):
    msg_str = ""
    for msg in msgs:
        msg_str += str(msg)
    print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}]", msg_str)
