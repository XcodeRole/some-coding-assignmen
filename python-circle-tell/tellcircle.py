import cv2 as cv
import numpy as np
def detect_circle_demo(image):#霍夫圆检测
    dst = cv.pyrMeanShiftFiltering(image,10,95)
    #转化为灰度图
    cimage = cv.cvtColor(dst,cv.COLOR_BGR2GRAY)
    #如果参数设置有问题，会返回None，也就是说没有检测到circle
    circles = cv.HoughCircles(cimage,cv.HOUGH_GRADIENT,1,20,param1=50,param2=50,minRadius=50,maxRadius=1000)
    if circles is not None:
        #这里报错AttributeError: 'NoneType' object has no attribute 'rint'，说明circles返回值有问题
        circles = np.uint16(np.around(circles))
        # circles = np.round(circles[0, :]).astype("int")
        for i in circles[0,:]:
            cv.circle(image, (i[0], i[1]), i[2], (0, 0, 255), 2)
            cv.circle(image, (i[0], i[1]), 2, (255, 0, 0), 2)
        cv.imshow("circles",image)
    else:
        print("No circle found,please adjustment your HoughCircles parameter")
src = cv.imread('input.jpg')
cv.namedWindow("input image",cv.WINDOW_AUTOSIZE)
cv.imshow("input image",src)
detect_circle_demo(src)
cv.waitKey(10000)
cv.destroyAllWindows()