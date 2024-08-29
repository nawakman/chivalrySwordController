import ctypes

mouseAreaSize=(0,0)
screenResolution=(0,0)

def clamp(n, min_value, max_value):
    return max(min_value, min(n, max_value))

def MouseMoveRel(x,y):
    """coordinates given in screen resolution e.g. x in 0-1920, y in 0-1080"""
    global mouseAreaSize
    global screenResolution
    newX=(x/screenResolution[0])*mouseAreaSize[0]
    newY=(y/screenResolution[1])*mouseAreaSize[1]
    ctypes.windll.user32.mouse_event(0x0001,int(round(newX,0)),int(round(newY,0)),0,0)#result might depend on windows mouse setting, might be different on different machines
    return

def MouseMoveRelNoAwareness(x,y):
    """coordinates given in mouseAreaSize"""
    ctypes.windll.user32.mouse_event(0x0001,int(round(x,0)),int(round(y,0)),0,0)#result might depend on windows mouse setting, might be different on different machines
    return

def MouseMoveAbs(x,y):
    """coordinates given in screen resolution e.g. x in 0-1920, y in 0-1080"""
    global screenResolution
    newX=(x/screenResolution[0])*65535
    newY=(y/screenResolution[1])*65535
    ctypes.windll.user32.mouse_event(0x8000+0x0001,int(newX),int(newY),0,0)#coordinates between 0-65535 for x and y #https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-mouse_event
    return

def MouseMoveAbsPrecise(x,y):
    """coordinates given in 65535 for both x and y"""
    ctypes.windll.user32.mouse_event(0x8000+0x0001,int(x),int(y),0,0)#coordinates between 0-65535 for x and y #https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-mouse_event
    return

def MouseMoveMix(x,y,xAbs,yAbs):#if we want to set one coordinate absolute and the other relative
    global screenResolution
    newX=x#keep those values if relative
    newY=y
    if xAbs:
        newX=clamp(newX,0,screenResolution[0])#avoid mouse out of bounds
        newX-=float(GetMousePos()[0])#relative to abs coordinates
    if yAbs:
        newY=clamp(newY,0,screenResolution[1])#avoid mouse out of bounds
        newY-=float(GetMousePos()[1])#relative to abs coordinates 
    MouseMoveRel(newX,newY)

def GetMouseAreaSize():
    MouseMoveAbsPrecise(65535,65535)#bottom right corner, max values for both x and y
    areaSize=GetMousePos()
    return areaSize

def GetMousePos():#obtained without DPI awareness #relative movement correspond to those strange coordinates #https://stackoverflow.com/questions/32541475/win32api-is-not-giving-the-correct-coordinates-with-getcursorpos-in-python
    pt = ctypes.wintypes.POINT()
    ctypes.windll.user32.GetCursorPos(ctypes.byref(pt))
    return (pt.x,pt.y)

def InitMouse():
    global mouseAreaSize
    global screenResolution

    mousePos=GetMousePos()#save initial mouse pos
    mouseAreaSize=GetMouseAreaSize()
    MouseMoveAbsPrecise(0,0)#return to origin
    MouseMoveRelNoAwareness(mousePos[0],mousePos[1])#then return to initial mouse pos
    
    ctypes.windll.shcore.SetProcessDpiAwareness(2)#https://stackoverflow.com/questions/32541475/win32api-is-not-giving-the-correct-coordinates-with-getcursorpos-in-python
    
    mousePos=GetMousePos()#save initial mouse pos #different from the previous one because we changed DPI awareness
    screenResolution=GetMouseAreaSize()#result has changed because we changed DPI awareness
    MouseMoveAbsPrecise(0,0)#return to origin
    MouseMoveRel(mousePos[0],mousePos[1])#then return to initial mouse pos
    return

"""
-the coordinate used for relative movement by ctypes.windll.user32.mouse_event(0x0001,,,,) are the same as the one given by ctypes.windll.user32.GetCursorPos(ctypes.byref(pt)) BEFORE changing DPI awareness, those coordinate are kinda strange(0-1800 instead of 0-1920 in my case)
-Once DPI awareness is set to 2, ctypes.windll.user32.GetCursorPos(ctypes.byref(pt)) will return the mouse coordinates relative to screen resolution(1920*1080 in my case)
"""

def LeftClick(down):#https://stackoverflow.com/questions/11229808/right-click-in-python-using-ctypes
    ctypes.windll.user32.mouse_event(0x0002 if down else 0x0004,0,0,0,0)

def RightClick(down):#https://stackoverflow.com/questions/11229808/right-click-in-python-using-ctypes
    ctypes.windll.user32.mouse_event(0x0008 if down else 0x0010,0,0,0,0)

def MiddleClick(down):#https://stackoverflow.com/questions/11229808/right-click-in-python-using-ctypes
    ctypes.windll.user32.mouse_event(0x0020 if down else 0x0040,00,0,0,0)
    