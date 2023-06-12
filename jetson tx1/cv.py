import cv2
import numpy as np
import pyrealsense2 as rs

# 创建RealSense深度摄像头对象
pipeline = rs.pipeline()
config = rs.config()
config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)

# 开始捕获深度图像
pipeline.start(config)

try:
    while True:
        # 获取深度图像
        frames = pipeline.wait_for_frames()
        depth_frame = frames.get_depth_frame()

        # 将深度图像转换为灰度图像
        depth_image = np.asanyarray(depth_frame.get_data())
        depth_image = cv2.convertScaleAbs(depth_image, alpha=0.03)
        # 显示深度图像
        cv2.imshow("Depth Image", depth_image)
        key = cv2.waitKey(1)
        if key == 'q':
            break

finally:
    # 停止捕获深度图像
    pipeline.stop()
    cv2.destroyAllWindows()
