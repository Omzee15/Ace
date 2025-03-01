# By_SciCraft
import cv2
from cvzone.FaceDetectionModule import FaceDetector
import numpy as np
import pyfirmata

# Initialize Arduino connection for servo control
port = "COM11"
board = pyfirmata.Arduino(port)

def setServoAngle(pin, angle):
    board.digital[pin].write(angle)

# Define servo pins
Yarm = 5  # Y-axis movement
Xarm = 4  # X-axis movement
top_left_eyelid = 11
bottom_left_eyelid = 10
top_right_eyelid = 9
bottom_right_eyelid = 6

cap = cv2.VideoCapture(0)
ws, hs = 1280, 720
cap.set(3, ws)
cap.set(4, hs)

if not cap.isOpened():
    print("Camera couldn't Access!!!")
    exit()

detector = FaceDetector()
servoPos = [90, 90]  # initial servo position
selected_face = None

while True:
    success, img = cap.read()
    img, bboxs = detector.findFaces(img, draw=False)
    
    if bboxs:
        # Mark and label all detected faces
        for i, bbox in enumerate(bboxs):
            fx, fy = bbox["center"]
            x, y, w, h = bbox["bbox"]
            cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
            cv2.putText(img, f"Face {i+1}", (x, y - 10), cv2.FONT_HERSHEY_PLAIN, 2, (0, 255, 0), 2)
            cv2.circle(img, (fx, fy), 80, (0, 255, 0), 2)
        
        # Display instruction to select face
        cv2.putText(img, "Press 1-9 to select a face", (50, 150), cv2.FONT_HERSHEY_PLAIN, 2, (0, 255, 255), 2)
        
        # Check for key presses to select a face
        key = cv2.waitKey(1) & 0xFF
        if ord('1') <= key <= ord('9'):
            face_index = key - ord('1')
            if face_index < len(bboxs):
                selected_face = face_index
        
        # Track the selected face
        if selected_face is not None and 0 <= selected_face < len(bboxs):
            fx, fy = bboxs[selected_face]["center"]
            pos = [fx, fy]

            servoX = np.interp(fx, [0, ws], [180, 0])
            servoY = np.interp(fy, [0, hs], [180, 0])
            servoX = np.clip(servoX, 0, 180)
            servoY = np.clip(servoY, 0, 180)

            servoPos[0] = servoX
            servoPos[1] = servoY

            # Move the eyeballs based on detected face position
            setServoAngle(Xarm, int(servoX))
            setServoAngle(Yarm, int(servoY))
            
            cv2.putText(img, "TARGET LOCKED", (850, 50), cv2.FONT_HERSHEY_PLAIN, 3, (255, 0, 255), 3)
    else:
        cv2.putText(img, "NO TARGET", (880, 50), cv2.FONT_HERSHEY_PLAIN, 3, (0, 0, 255), 3)
        selected_face = None

    cv2.putText(img, f'Servo X: {int(servoPos[0])} deg', (50, 50), cv2.FONT_HERSHEY_PLAIN, 2, (255, 0, 0), 2)
    cv2.putText(img, f'Servo Y: {int(servoPos[1])} deg', (50, 100), cv2.FONT_HERSHEY_PLAIN, 2, (255, 0, 0), 2)

    print(f'Simulated Servo Positions -> X: {int(servoPos[0])} deg, Y: {int(servoPos[1])} deg')

    cv2.imshow("Image", img)
    key = cv2.waitKey(1) & 0xFF
    if key == 27 or key == ord('q'):  # Exit on ESC or 'q' key
        break

cap.release()
cv2.destroyAllWindows()
