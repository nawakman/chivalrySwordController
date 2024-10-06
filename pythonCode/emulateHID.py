from threading import Timer
from serialCommunication import SerialCommunication
from mouse import *
from directkeys import *
from utils import *
import vgamepad as vg
from time import *
from math import *

s1=SerialCommunication(115200)
lastUpdate=time()

#SENSOR DATA GLOBAL VARIABLES
values=[]#will temporarily store each value received in serial communication
swordYaw,swordPitch,swordRoll=0,0,0
swordAccX,swordAccY,swordAccZ=0,0,0

headYaw,headPitch,headRoll=0,0,0
lastHeadYaw,lastHeadPitch,lastHeadRoll=0,0,0
absoluteHeadYaw=0#only on yaw because it is the only axis to drift
headAccX,headAccY,headAccZ=0,0,0

joyX,joyY=0,0#since we use either a joystick or a nunchuk we write to only one variable
nunchukPitch,nunchukRoll=0,0
nunchukAccX,nunchukAccY,nunchukAccZ=0,0,0
nunchukButtonZ,nunchukButtonC=False,False

#INPUT DATA GLOBAL VARIABLES
z,q,s,d=False,False,False,False
swordAttack=False#any attack related to the sword
specialAttack=False#chivalry special attack
leftAttack=False#choose wich side to swing sword# leftAttack is from left to right
guard=False
footKick=False

#gamepad = vg.VX360Gamepad()#can be used to emulate an Xbox 360 controller instead of mouse/keyboard
InitMouse()

#FOR CLARITY ALL ACTIONS ARE EXECUTED IN THIS ORDER #set var #do action(press keys, mouse) #prepare and run timer #print
#FOR CLARITY GLOBAL VARIABLES ARE DECLARED IN THIS ORDER #SENSOR RELATED #INPUT RELATED

def ypr():
    global swordYaw,swordPitch,swordRoll

    global guard
    
    swordYaw,swordPitch,swordRoll=float(values[1]),float(values[2]),float(values[3])
    
    #print(swordYaw,"/",swordPitch,"/",swordRoll)
    if not guard and 40<abs(swordYaw)<100 and -70>swordPitch>-130:#middle at 100
        guard=True
        RightClick(True)
        t=Timer(0.5,GuardFalse)
        t.start()
        print("guard")
        #print(abs(swordYaw),"/",abs(swordPitch),"/",abs(swordRoll))

def ypr2Yaw():#control X using yaw #SUBJECT TO DRIFT
    global headYaw,headPitch,headRoll

    global absoluteHeadYaw,lastHeadYaw,lastHeadPitch

    lastHeadYaw,lastHeadPitch=headYaw,headPitch
    headYaw,headPitch,headRoll=float(values[1]),float(values[2]),float(values[3])

    yawDiff=headYaw-lastHeadYaw
    if abs(yawDiff)<0.08:#desperate try to cancel drift
        yawDiff=0
    if abs(yawDiff)>170:#mpu angles go from -180 to 180, when it exceeds 180 it goes back to -180, when it go under -180 it goes back to 180, since both defines the same rotation
        yawDiffSign=-1 if yawDiff>0 else 1
        yawDiff=yawDiffSign*abs(abs(headYaw)-abs(lastHeadYaw))#fix the difference affected by the abrupt 360 degrees change

    Xcoord=yawDiff*10#standard control #you can change sensitivity here by adjusting the constant
    absoluteHeadYaw+=yawDiff#tracks absolute yaw at all time #subject to drift

    ###FNAF TYPE CONTROLS###
    #comment out to disable
    Xcoord=absoluteHeadYaw*3#FNAF controls #move independently from head movement
    ########################
    
    Ycoord=(-headRoll/45)*540+(1080/2)#(lastHeadPitch-headPitch)*10
    MouseMoveMix(Xcoord,Ycoord,False,True)

def ypr2Pitch():#control X using pitch
    global headYaw,headPitch,headRoll

    global lastHeadPitch

    lastHeadPitch,lastHeadRoll=headPitch,headRoll
    headYaw,headPitch,headRoll=float(values[1]),float(values[2]),float(values[3])

    pitchDiff=headPitch-lastHeadPitch

    Xcoord=pitchDiff*10#standard control #you can change sensitivity here by adjusting the constant
    
    ###FNAF TYPE CONTROLS###
    #comment out to disable
    Xcoord=-headPitch*2#FNAF controls #move independently from head movement
    ########################
    
    Ycoord=(-headRoll/45)*540+(1080/2)
    MouseMoveMix(Xcoord,Ycoord,False,True)

def GuardFalse():
    global swordYaw,swordPitch,swordRoll
    
    global guard
    
    if 40<abs(swordYaw)<100 and 70<abs(swordPitch)<130:#still guarding
        t=Timer(0.5,GuardFalse)
        t.start()
    else:
        guard=False
        RightClick(False)

def joy():
    global joyX,joyY

    global z,q,s,d
    global footKick

    joyX,joyY=float(values[1]),float(values[2])
    button=bool(int(values[3]))
    #print(joyX,"/",joyY)
    if abs(joyX)>0.5:
        if joyX>0 and not d:
            d=True
            PressKey(0x44)
            #print("pressed D ",joyX)
        elif joyX<0 and not q:
            q=True
            PressKey(0x51)
            #print("pressed Q ",joyX)
    else:
        if(d):
            d=False
            ReleaseKey(0x44)
            #print("released D")
        if(q):
            q=False
            ReleaseKey(0x51)
            #print("released Q")

    if abs(joyY)>0.5:
        if joyY>0 and not z:
            z=True
            PressKey(0x5a)
            print("pressed z ",joyY)
        elif joyY<0 and not s:
            s=True
            PressKey(0x53)
            print("pressed S ",joyY)
    else:
        if z:
            z=False
            ReleaseKey(0x5a)
            #print("released Z")
        if s:
            s=False
            ReleaseKey(0x53)
            #print("released S")
    if not footKick and button:
        footKick=True
        PressKey(0x46)
        t=Timer(1,FootKickFalse)
        t.start()
        print("foot kick")

def nchk():
    global absoluteHeadYaw
    global joyX,joyY
    global nunchukAccX,nunchukAccY,nunchukAccZ
    global nunchukPitch,nunchukRoll
    global nunchukButtonZ,nunchukButtonZ

    global z,q,s,d
    global footKick

    joyX,joyY=float(values[1]),float(values[2])
    nunchukAccX,nunchukAccY,nunchukAccZ=float(values[3]),float(values[4]),float(values[5])
    nunchukPitch,nunchukRoll=float(values[6]),float(values[7])
    nunchukButtonZ,nunchukButtonC=bool(float(values[8])),bool(float(values[9]))
    #print(joyX,"/",joyY)
    if abs(joyX)>0.5:
        if joyX>0 and not d:
            d=True
            PressKey(0x44)
            #print("pressed D ",joyX)
        elif joyX<0 and not q:
            q=True
            PressKey(0x51)
            #print("pressed Q ",joyX)
    else:
        if(d):
            d=False
            ReleaseKey(0x44)
            #print("released D")
        if(q):
            q=False
            ReleaseKey(0x51)
            #print("released Q")

    if abs(joyY)>0.5:
        if joyY>0 and not z:
            z=True
            PressKey(0x5a)
            print("pressed z ",joyY)
        elif joyY<0 and not s:
            s=True
            PressKey(0x53)
            print("pressed S ",joyY)
    else:
        if z:
            z=False
            ReleaseKey(0x5a)
            #print("released Z")
        if s:
            s=False
            ReleaseKey(0x53)
            #print("released S")
    if not footKick and nunchukButtonZ:
        footKick=True
        PressKey(0x46)
        t=Timer(1,FootKickFalse)
        t.start()
        print("foot kick")
    if nunchukButtonC:
        absoluteHeadYaw=0

def FootKickFalse():
    global footKick
    footKick=False
    ReleaseKey(0x46)

def acc():
    global swordAccX,swordAccY,swordAccZ

    global swordAttack
    global specialAttack
    global leftAttack
    global swordRoll

    swordAccX,swordAccY,swordAccZ=float(values[1]),float(values[2]),float(values[3])

    if not swordAttack:
        if swordAccY<-17000 and abs(swordAccX)>9000:#y acc is negative during slash because of centrifugal force #we ignore accX sign meaning we can swing sword both ways 
            swordAttack=True#there will be an attack, either special or normal
            #print(swordAccX,"/",swordAccY,"/",swordAccZ,"/",values[1])
            if specialAttack:
                PressKey(0x41)
                t=Timer(0.1,SpecialAttackFalse)
                t.start()
                print("special attack")
            else:
                if(abs(swordRoll)>100):#if sword is upside down its roll value is approximately in this range #accZ is negative when the accelerometer is upside down, so the sword too(but not always, can go positive while swinging)
                    leftAttack=True
                    PressKey(0x12)
                    t=Timer(0.1,LeftAttackFalse)
                    t.start()
                    print("left slash")
                else:
                    print("rightslash")

                LeftClick(True)
                t=Timer(0.1,SwordAttackFalse)
                t.start()
                print("slash")
                #print(swordAccX,"/",swordAccY,"/",swordAccZ,"/",values[1])
        elif swordAccY>6500:#y acc is positive while stabbing because we are pushing the blade away, thus acelerating in Y direction
            if abs(swordAccY)>abs(swordAccX) and abs(swordAccZ)>abs(swordAccX): 
                swordAttack=True
                MiddleClick(True)
                t=Timer(0.1,StabAttackFalse)
                t.start()
                print("stab")
                #print(swordAccX,"/",swordAccY,"/",swordAccZ,"/",values[1])

def acc2():
    global headAccX,headAccY,headAccZ

    headAccX,headAccY,headAccZ=float(values[1]),float(values[2]),float(values[3])

def LeftAttackFalse():
    global leftAttack
    leftAttack=False
    ReleaseKey(0x12)

def SwordAttackFalse():
    global swordAttack
    swordAttack=False
    LeftClick(False)

def StabAttackFalse():
    global swordAttack
    swordAttack=False
    MiddleClick(False)
            
def btn():#if the button is still pressed for next sword swing, it will be a special attack
    global specialAttack

    button=bool(int(values[1]))
    if not specialAttack and button:
        specialAttack=True

def SpecialAttackFalse():#A TESTER
    global swordAttack
    global specialAttack
    swordAttack=False
    specialAttack=False
    ReleaseKey(0x41)

while True:
    for line in s1.ReceiveAll():
        try:
            for chunk in line.split('\t'):
                values=chunk.split(' ')
                match values[0]:
                    case "ypr1":
                        ypr()
                    case "ypr2":
                        ypr2Pitch()
                    case "joy":
                        joy()
                    case "nchk":
                        nchk()
                    case "acc1":
                        acc()
                    case "acc2":
                        acc2()
                    case "btn":
                        btn()
                    case "\r":#do not print line skip in terminal
                        pass
                    case _:
                        """includeNonPrintable = [char for char in line]#display for exemple '\r','\n','\t' and all other non displayed character that might cause malfunction
                        print(includeNonPrintable)#everything else"""
                        print("does not match any command: ",line)
        except:
            print("bad line: ",line)

    while((time()-lastUpdate)<0.008):#0.008 for 125Hz polling rate, the same as a xBox controller
        pass
    lastUpdate=time()