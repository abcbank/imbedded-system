import cv2;
import sys;

src = cv2.imread('C:/Users/DCL2023/test.jpg', cv2.IMREAD_GRAYSCALE)

if src is None:
    print('Image load failed!')
    sys.exit()

# dx와 dy를 합쳐서 출력해야합니다.
dx = cv2.Sobel(src, -1, 1) # delta 값을 지정해주지 않으면 미분이 - 부분은 0
dy = cv2.Sobel(src, -1, 0)

cv2.imwrite('', dx+dy)

cv2.waitKey()

cv2.destroyAllWindows()