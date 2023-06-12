import cv2
import numpy as np

# 打开摄像头
cap = cv2.VideoCapture(0)

# 定义红色范围
# lower_red = np.array([0, 100, 100])
# upper_red = np.array([10, 255, 255])
# lower_red2 = np.array([170, 100, 100])
# upper_red2 = np.array([180, 255, 255])

lower_red = np.array([0, 50, 100])
upper_red = np.array([50, 150, 200])
lower_red2 = np.array([0, 50, 100])
upper_red2 = np.array([50, 150, 200])


# 循环读取每一帧图像
while True:
    ret, frame = cap.read()

    # 转换为HSV颜色空间
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # 创建红色掩码
    mask1 = cv2.inRange(hsv, lower_red, upper_red)
    mask2 = cv2.inRange(hsv, lower_red2, upper_red2)
    mask = cv2.bitwise_or(mask1, mask2)

    # 形态学操作
    kernel = np.ones((5, 5), np.uint8)
    mask = cv2.erode(mask, kernel, iterations=1)
    mask = cv2.dilate(mask, kernel, iterations=1)

    # 查找轮廓
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # 遍历每个轮廓
    for contour in contours:
        # 计算轮廓面积和周长
        area = cv2.contourArea(contour)
        perimeter = cv2.arcLength(contour, True)

        # 筛选符合要求的轮廓
        if area > 100 and perimeter > 100:
            # 计算轮廓重心和外接矩形
            moments = cv2.moments(contour)
            cx = int(moments['m10'] / moments['m00'])
            cy = int(moments['m01'] / moments['m00'])
            x, y, w, h = cv2.boundingRect(contour)

            # 在图像中绘制目标区域
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
            cv2.circle(frame, (cx, cy), 5, (0, 0, 255), -1)

    # 显示图像
    cv2.imshow('frame', frame)

    # 按下q键退出循环
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 释放摄像头并关闭窗口
cap.release()
cv2.destroyAllWindows()
