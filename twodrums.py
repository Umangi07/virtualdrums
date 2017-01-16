import ctypes
from ctypes import wintypes
import time
import serial

user32 = ctypes.WinDLL('user32', use_last_error=True)
pvalue = 0



left = serial.Serial('COM12',115200)
right = serial.Serial('COM9',115200)

#left.write('a')
#right.write('a')

#time.sleep(1)
INPUT_MOUSE    = 0
INPUT_KEYBOARD = 1
INPUT_HARDWARE = 2

KEYEVENTF_EXTENDEDKEY = 0x0001
KEYEVENTF_KEYUP       = 0x0002
KEYEVENTF_UNICODE     = 0x0004
KEYEVENTF_SCANCODE    = 0x0008

MAPVK_VK_TO_VSC = 0

#msdn.microsoft.com/en-us/library/dd375731
VK_A = 0x41
VK_B = 0x42
VK_C = 0x43
VK_D = 0x44
VK_E = 0x45
VK_F = 0x46
VK_G = 0x47
VK_H = 0x48
VK_I = 0x49
VK_J = 0x4A
VK_K = 0x4B
VK_L = 0x4C
VK_M = 0x4D
VK_N = 0x4E
VK_O = 0x4F
VK_P = 0x50
VK_Q = 0x51
VK_R = 0x52
VK_S = 0x53
VK_T = 0x54
VK_U = 0x55
VK_V = 0x56
VK_W = 0x57
VK_X = 0x58
VK_Y = 0x59
VK_Z = 0x5A
VK_0 = 0x30
VK_1 = 0x31
VK_2 = 0x32
VK_3 = 0x33
VK_4 = 0x34
VK_5 = 0x35
VK_6 = 0x36
VK_7 = 0x37
VK_8 = 0x38
VK_9 = 0x39
# C struct definitions

wintypes.ULONG_PTR = wintypes.WPARAM

class MOUSEINPUT(ctypes.Structure):
    _fields_ = (("dx",          wintypes.LONG),
                ("dy",          wintypes.LONG),
                ("mouseData",   wintypes.DWORD),
                ("dwFlags",     wintypes.DWORD),
                ("time",        wintypes.DWORD),
                ("dwExtraInfo", wintypes.ULONG_PTR))

class KEYBDINPUT(ctypes.Structure):
    _fields_ = (("wVk",         wintypes.WORD),
                ("wScan",       wintypes.WORD),
                ("dwFlags",     wintypes.DWORD),
                ("time",        wintypes.DWORD),
                ("dwExtraInfo", wintypes.ULONG_PTR))

    def __init__(self, *args, **kwds):
        super(KEYBDINPUT, self).__init__(*args, **kwds)
        # some programs use the scan code even if KEYEVENTF_SCANCODE
        # isn't set in dwFflags, so attempt to map the correct code.
        if not self.dwFlags & KEYEVENTF_UNICODE:
            self.wScan = user32.MapVirtualKeyExW(self.wVk,
                                                 MAPVK_VK_TO_VSC, 0)

class HARDWAREINPUT(ctypes.Structure):
    _fields_ = (("uMsg",    wintypes.DWORD),
                ("wParamL", wintypes.WORD),
                ("wParamH", wintypes.WORD))

class INPUT(ctypes.Structure):
    class _INPUT(ctypes.Union):
        _fields_ = (("ki", KEYBDINPUT),
                    ("mi", MOUSEINPUT),
                    ("hi", HARDWAREINPUT))
    _anonymous_ = ("_input",)
    _fields_ = (("type",   wintypes.DWORD),
                ("_input", _INPUT))

LPINPUT = ctypes.POINTER(INPUT)

def _check_count(result, func, args):
    if result == 0:
        raise ctypes.WinError(ctypes.get_last_error())
    return args

user32.SendInput.errcheck = _check_count
user32.SendInput.argtypes = (wintypes.UINT, # nInputs
                             LPINPUT,       # pInputs
                             ctypes.c_int)  # cbSize

# Functions

def PressKey(hexKeyCode):
    x = INPUT(type=INPUT_KEYBOARD,
              ki=KEYBDINPUT(wVk=hexKeyCode))
    user32.SendInput(1, ctypes.byref(x), ctypes.sizeof(x))

def ReleaseKey(hexKeyCode):
    x = INPUT(type=INPUT_KEYBOARD,
              ki=KEYBDINPUT(wVk=hexKeyCode,
                            dwFlags=KEYEVENTF_KEYUP))
    user32.SendInput(1, ctypes.byref(x), ctypes.sizeof(x))


def Upkey():
	
	PressKey(VK_UP)
	time.sleep(2)
	ReleaseKey(VK_UP)

def Downkey():
	
	PressKey(VK_DOWN)
	time.sleep(2)
	ReleaseKey(VK_UP)

def akey():
	
	PressKey(VK_A)
	#time.sleep(2)
	ReleaseKey(VK_A)

def bkey():
	
	PressKey(VK_B)
	#time.sleep(1)
	ReleaseKey(VK_B)
def ckey():
	
	PressKey(VK_C)
	#time.sleep(1)
	ReleaseKey(VK_C)
def dkey():
	
	PressKey(VK_D)
	#time.sleep(1)
	ReleaseKey(VK_D)
	
def ekey():
	
	PressKey(VK_E)
	#time.sleep(1)
	ReleaseKey(VK_E)
def fkey():
	
	PressKey(VK_F)
	#time.sleep(1)
	ReleaseKey(VK_F)
def gkey():
	
	PressKey(VK_G)
	#time.sleep(1)
	ReleaseKey(VK_G)
def hkey():
	
	PressKey(VK_G)
	#time.sleep(1)
	ReleaseKey(VK_G)
def ckey():
	
	PressKey(VK_H)
	#time.sleep(1)
	ReleaseKey(VK_H)
def ikey():
	
	PressKey(VK_I)
	#time.sleep(1)
	ReleaseKey(VK_I)
def jkey():
	
	PressKey(VK_J)
	#time.sleep(1)
	ReleaseKey(VK_J)
def kkey():
	
	PressKey(VK_K)
	#time.sleep(1)
	ReleaseKey(VK_K)
def lkey():
	
	PressKey(VK_L)
	#time.sleep(1)
	ReleaseKey(VK_L)
def mkey():
	
	PressKey(VK_M)
	#time.sleep(1)
	ReleaseKey(VK_M)
def nkey():
	
	PressKey(VK_N)
	#time.sleep(1)
	ReleaseKey(VK_N)
def okey():
	
	PressKey(VK_O)
	#time.sleep(1)
	ReleaseKey(VK_O)
def pkey():
	
	PressKey(VK_P)
	#time.sleep(1)
	ReleaseKey(VK_P)
def qkey():
	
	PressKey(VK_Q)
	#time.sleep(1)
	ReleaseKey(VK_Q)
def rkey():
	
	PressKey(VK_R)
	#time.sleep(1)
	ReleaseKey(VK_R)
																
def skey():
	
	PressKey(VK_S)
	#time.sleep(1)
	ReleaseKey(VK_S)
def tkey():
	
	PressKey(VK_T)
	#time.sleep(1)
	ReleaseKey(VK_T)
def ukey():
	
	PressKey(VK_U)
	#time.sleep(1)
	ReleaseKey(VK_U)
def vkey():
	
	PressKey(VK_V)
	#time.sleep(1)
	ReleaseKey(VK_V)
def wkey():
	
	PressKey(VK_W)
	#time.sleep(1)
	ReleaseKey(VK_W)
def xkey():
	
	PressKey(VK_X)
	#time.sleep(1)
	ReleaseKey(VK_X)
def ykey():
	
	PressKey(VK_Y)
	#time.sleep(1)
	ReleaseKey(VK_Y)
def zkey():
	
	PressKey(VK_Z)
	#time.sleep(1)
	ReleaseKey(VK_Z)
def zerokey():
	
	PressKey(VK_0)
	#time.sleep(1)
	ReleaseKey(VK_0)
def onekey():
	
	PressKey(VK_1)
	#time.sleep(1)
	ReleaseKey(VK_1)
def twokey():
	
	PressKey(VK_2)
	#time.sleep(1)
	ReleaseKey(VK_2)
def threekey():
	
	PressKey(VK_3)
	#time.sleep(1)
	ReleaseKey(VK_3)
def fourkey():
	
	PressKey(VK_4)
	#time.sleep(1)
	ReleaseKey(VK_4)
def fivekey():
	
	PressKey(VK_5)
	#time.sleep(1)
	ReleaseKey(VK_5)
def sixkey():
	
	PressKey(VK_6)
	#time.sleep(1)
	ReleaseKey(VK_6)
def sevenkey():
	
	PressKey(VK_7)
	#time.sleep(1)
	ReleaseKey(VK_7)
def eightkey():
	
	PressKey(VK_8)
	#time.sleep(1)
	ReleaseKey(VK_8)
def ninekey():
	
	PressKey(VK_9)
	#time.sleep(1)
	ReleaseKey(VK_9)
											
						
pleftvalue = 'z'
prightvalue ='z'
#left.flush() 
#right.flush()
#if __name__ == "__main__":
while True:
	#print("done")
	rightdata=right.read()
	#print("reached")
	#print("reached again")
	leftdata =left.read()
	#print("reached2")
	right.flushInput()
	left.flushInput()
	print(leftdata,rightdata)
	if leftdata.decode('ascii') == 'b' and pleftvalue != leftdata :
		ikey()
	if leftdata.decode('ascii') == 'e' and pleftvalue != leftdata :
		ekey()
	if leftdata.decode('ascii') == 'd' and pleftvalue.decode('ascii') == 'z':
		twokey()
		ekey()
	if leftdata.decode('ascii') == 'd' and pleftvalue.decode('ascii') == 'f':
		ekey()	
	if leftdata.decode('ascii') == 'a' and pleftvalue.decode('ascii') == 'z':
		twokey()
		ikey()
	if leftdata.decode('ascii') == 'a' and pleftvalue.decode('ascii') == 'f':
		ikey()		
	if leftdata.decode('ascii') == 'f'and (pleftvalue.decode('ascii') == 'z' or pleftvalue.decode('ascii') == 'c'):
		twokey()
	
	if leftdata.decode('ascii') == 'c'and pleftvalue.decode('ascii') == 'z':
		twokey()					
				
	if rightdata.decode('ascii') == 'a'and prightvalue != rightdata:
		wkey()
	if rightdata.decode('ascii') == 'b' and prightvalue != rightdata :
		tkey()
	if rightdata.decode('ascii') == 'c' and prightvalue != rightdata :
		ekey()
	if rightdata.decode('ascii') == 'e' and prightvalue != rightdata :
		ikey()	
	if rightdata.decode('ascii') == 'f' and prightvalue != rightdata :
		ukey()	
	pleftvalue = leftdata
	prightvalue = rightdata