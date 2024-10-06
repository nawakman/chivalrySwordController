import serial.tools.list_ports#pip install pyserial

class SerialCommunication:

    def __init__(self,baudrate):
        self.ports=serial.tools.list_ports.comports()
        self.portsList=[]
        self.serialInst=serial.Serial()
        for port in self.ports:
            self.portsList.append(str(port))#add port name to portsList
            print(str(port))
        #com=input("Select COM port (input a number)")
        com=str(3)
        self.serialInst.port="COM"+com
        self.serialInst.baudrate=baudrate
        self.serialInst.timeout=0#non blocking mode,returns 0's if not enough data #https://pyserial.readthedocs.io/en/latest/pyserial_api.html
        try:
            self.serialInst.open()
        except:
            print("COM",com," already opened by something else")
            quit()

    def Send(self,packet):
        self.serialInst.write(packet.encode('utf-8'))

    def ReceiveLine(self):
        if self.serialInst.in_waiting:#in waiting is the number of bytes left in the serial 
            packet=self.serialInst.readline()
            try:
                msg=packet.decode('utf-8').rstrip('\n')#MIGHT not work if wrong baudrate
                #print(msg)
                return msg
            except:
                print("cannot decode utf-8 package, can happen if communication has just started")
                return None
        else:
            return None

    def ReceiveAll(self,sep=b"\n"):#https://stackoverflow.com/questions/67820228/pyserials-readlines-consumes-25x-times-as-much-cpu-time-as-read
        buffer = b""
        while True:
            buffer += self.serialInst.read(500)#make sure to set timeout to non blocking mode to avoid being stuck waiting for data
            #print(buffer)
            while sep in buffer:
                line, _, buffer = buffer.partition(sep)#https://www.w3schools.com/python/ref_string_partition.asp
                try:
                    yield line.decode('utf-8')
                except:
                    print("cannot decode utf-8 package, can happen if communication has just started")
                    print("cannot decode line: ",line)
                    return None