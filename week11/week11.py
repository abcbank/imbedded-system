import cv2;
import sys;

src = cv2.imread('C:/Users/DCL2023/init.bmp', cv2.IMREAD_GRAYSCALE)

# cv2.imwrite('C:/Users/DCL2023/test.bmp', src)
if src is None:
    print('Image load failed!')
    sys.exit()
# aspect_ratio = float(480) / src.shape[1]
# dsize = (480, int(src.shape[0] * aspect_ratio))

# src = cv2.resize(src, dsize)

# src = cv2.GaussianBlur(src, (3,3),1.5)
# cv2.imwrite('C:/Users/DCL2023/test3.bmp', src)

# dx와 dy를 합쳐서 출력해야합니다.
dx = cv2.Sobel(src, cv2.CV_8UC1, 0,1) # delta 값을 지정해주지 않으면 미분이 - 부분은 0
dy = cv2.Sobel(src, cv2.CV_8UC1, 1,0)


cv2.imwrite('C:/Users/DCL2023/test2.bmp', dx+dy)