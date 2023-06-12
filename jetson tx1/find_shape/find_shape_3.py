import cv2
import numpy as np

# 创建摄像头对象
cap = cv2.VideoCapture(0)

# 设置摄像头分辨率
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

# 定义棕色色块的颜色范围
# lower_brown = np.array([10, 50, 20])
# upper_brown = np.array([20, 255, 255])
lower_brown = np.array([0, 50, 100])
upper_brown = np.array([50, 150, 200])

while True:
    # 读取摄像头图像
    ret, frame = cap.read()

    # 转换为HSV颜色空间
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # 对图像进行二值化处理，得到棕色色块的掩膜
    mask = cv2.inRange(hsv, lower_brown, upper_brown)

    # 对掩膜进行形态学操作，去除噪点
    kernel = np.ones((5,5),np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)

    # 查找并绘制最大的棕色色块
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if len(contours) > 0:
        max_contour = max(contours, key=cv2.contourArea)
        (x,y), radius = cv2.minEnclosingCircle(max_contour)
        if radius > 10:
            cv2.circle(frame, (int(x),int(y)), int(radius), (0,255,0), 2)
            approx = cv2.approxPolyDP(max_contour, 0.01*cv2.arcLength(max_contour,True),True)
            if len(approx) == 3:
                shape = "Triangle"
            elif len(approx) == 4:
                shape = "Rectangle"
            else:
                shape = "Circle"
            cv2.putText(frame, shape, (int(x-radius),int(y-radius)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,255), 2)
            cv2.putText(frame, "Center: ({}, {})".format(int(x),int(y)), (int(x-radius),int(y+radius)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,255), 2)

    # 显示结果
    cv2.imshow("Result", frame)

    # 按下q键退出循环
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 释放摄像头资源
cap.release()
cv2.destroyAllWindows()
