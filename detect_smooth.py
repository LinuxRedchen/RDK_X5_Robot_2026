import cv2
import torch
import time
import threading
import numpy as np

from ultralytics import YOLO


# =============================
# 摄像头线程
# =============================

class CameraThread:

    def __init__(self, source=0):

        self.cap = cv2.VideoCapture(source)

        # 设置摄像头缓存
        self.cap.set(
            cv2.CAP_PROP_BUFFERSIZE,
            1
        )

        self.ret = False
        self.frame = None

        self.running = True

        threading.Thread(
            target=self.update,
            daemon=True
        ).start()


    def update(self):

        while self.running:

            ret, frame = self.cap.read()

            if ret:
                self.ret = ret
                self.frame = frame


    def read(self):

        return self.ret, self.frame


    def stop(self):

        self.running=False
        self.cap.release()



# =============================
# EMA目标框滤波
# =============================

class EMAFilter:

    def __init__(self, alpha=0.6):

        self.alpha=alpha
        self.history={}


    def update(self,obj_id,bbox):

        if obj_id not in self.history:

            self.history[obj_id]=bbox

        else:

            old=self.history[obj_id]

            new=[
                self.alpha*bbox[i]+
                (1-self.alpha)*old[i]
                for i in range(4)
            ]

            self.history[obj_id]=new


        return self.history[obj_id]



# =============================
# YOLO检测器
# =============================

class Detector:


    def __init__(self):

        self.device="cuda" if torch.cuda.is_available() else "cpu"

        print(
            "Device:",
            self.device
        )


        self.model=YOLO(
            "yolo11n.pt"
        )


        self.model.to(
            self.device
        )


        self.filter=EMAFilter(
            alpha=0.65
        )


        self.frame_id=0



    def detect(self,frame):


        self.frame_id+=1


        # ====================
        # 跳帧
        # ====================

        if self.frame_id%3!=0:

            return frame



        # ====================
        # 缩放
        # ====================

        small=cv2.resize(
            frame,
            (640,360)
        )


        # ====================
        # YOLO FP16
        # ====================

        results=self.model.track(

            small,

            conf=0.35,

            persist=True,

            tracker="bytetrack.yaml",

            half=True if self.device=="cuda" else False,

            verbose=False

        )



        if results[0].boxes is None:

            return frame



        scale_x=frame.shape[1]/640

        scale_y=frame.shape[0]/360



        for box in results[0].boxes:


            if box.id is None:
                continue


            obj_id=int(box.id)


            cls=int(box.cls)

            conf=float(box.conf)



            x1,y1,x2,y2=box.xyxy[0]


            bbox=[

                x1*scale_x,

                y1*scale_y,

                x2*scale_x,

                y2*scale_y

            ]


            bbox=self.filter.update(

                obj_id,

                bbox

            )


            x1,y1,x2,y2=map(
                int,
                bbox
            )


            name=self.model.names[cls]


            cv2.rectangle(

                frame,

                (x1,y1),

                (x2,y2),

                (0,255,0),

                2

            )


            cv2.putText(

                frame,

                f"{name} {conf:.2f}",

                (x1,y1-10),

                cv2.FONT_HERSHEY_SIMPLEX,

                0.6,

                (0,255,0),

                2

            )


        return frame




# =============================
# 主程序
# =============================

def main():


    cam=CameraThread()


    detector=Detector()


    last=time.time()


    while True:


        ret,frame=cam.read()


        if not ret:

            continue



        frame=detector.detect(frame)



        # FPS显示

        now=time.time()

        fps=1/(now-last)

        last=now


        cv2.putText(

            frame,

            f"FPS:{fps:.1f}",

            (20,40),

            cv2.FONT_HERSHEY_SIMPLEX,

            1,

            (0,0,255),

            2

        )



        cv2.imshow(

            "AI Object Detection",

            frame

        )



        if cv2.waitKey(1)==ord('q'):

            break



    cam.stop()

    cv2.destroyAllWindows()



if __name__=="__main__":

    main()