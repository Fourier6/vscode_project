import cv2
import numpy as np
import time

cap = cv2.VideoCapture(0) # 0表示默认摄像头

# 定义颜色范围
lower_brown = np.array([0, 50, 100])
upper_brown = np.array([50, 150, 200])

# 初始化帧数计数器和时间戳
frame_count = 0
start_time = time.time()

while True:
    ret, frame = cap.read()
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # 使用颜色过滤器来过滤出棕色区域
    mask = cv2.inRange(hsv, lower_brown, upper_brown)

    # 对过滤后的图像进行形态学操作，如腐蚀和膨胀，以去除噪声和填充空洞
    kernel = np.ones((5,5),np.uint8)
    mask = cv2.erode(mask, kernel, iterations=1)
    mask = cv2.dilate(mask, kernel, iterations=1)

    # 使用cv2.findContours函数找到所有的轮廓
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # 遍历所有轮廓，找到最大的轮廓
    max_area = 0
    max_contour = None
    for contour in contours:
        area = cv2.contourArea(contour)
        if area > max_area:
            max_area = area
            max_contour = contour

    # 使用cv2.boundingRect函数获取最大轮廓的边界框，并在原始图像上绘制边界框
    if max_contour is not None:
        x, y, w, h = cv2.boundingRect(max_contour)
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

        # 计算边界框的中心点坐标
        cx = x + w // 2
        cy = y + h // 2
        cv2.circle(frame, (cx, cy), 5, (0, 0, 255), -1)
        # 在屏幕上显示边界框的面积和色块的面积
        cv2.putText(frame, "Bounding box area: {}".format(w*h), (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
        cv2.putText(frame, "Contour area: {}".format(max_area), (10, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
        bl=max_area/float(w*h)
        cv2.putText(frame, "bl: {}".format(bl), (10, 90), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
        if(bl>0.9):
            cv2.putText(frame, "zhengfang", (10, 70), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
        elif (bl<=0.9)and(bl>=0.75):
            cv2.putText(frame, "round", (10, 70), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
        elif (bl<=0.75)and(max_area/w*h>0.30):
            cv2.putText(frame, "sanjiao", (10, 70), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

    # 计算帧率并在屏幕上显示
    frame_count += 1
    if frame_count % 10 == 0:
        end_time = time.time()
        fps = frame_count / (end_time - start_time)
        cv2.putText(frame, "FPS: {:.2f}".format(fps), (10, 70), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
        frame_count = 0
        start_time = end_time

    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
