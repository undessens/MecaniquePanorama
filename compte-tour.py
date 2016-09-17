import serial
import time
import os
import sys
import glob
from OSC import OSCClient, OSCMessage

#OS choose ofApp working directory
os.chdir('ImageSeqFromOSC/')

def lauch_app():
	
	os.system('make run')

def stop_app():
	os.system('killall "ImageSeqFromOSC" ')

def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    ports = []
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result


if __name__ == '__main__':

	#Serial print port available
	print serial_ports()

	#Serial connect
	try:
		if sys.platform.startswith('darwin'):
			ser = serial.Serial((serial_ports())[0],115200)
		elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
			ser = serial.Serial('/dev/ttyACM0',115200)
			#ser = serial.Serial('/dev/ttyACM1', 115200)
		else:
			ser = None
	except :
		print "Impossible to connect to Serial"
		ser = None	

	#Start OF app
	#stop_app()
	#time.sleep(1)
	#lauch_app(	)

	#OSC
	client = OSCClient()
	client.connect( ("localhost", 12345))


	value = 0
	param = []
	msg = []


	time.sleep(1)



	while 1 :
		
		if(ser):
			msg = (ser.readline())
		else :	
			msg = ""


		if (len(msg)>1):


			oscMsg = OSCMessage()
			value = ord(msg[1]) - ord("0")

			if ( msg[0]== "+") :
				oscMsg.setAddress("/transport/next")
				oscMsg.append(value)
				print "next frames :"+str(value)
			elif ( msg[0] == '-') :
				oscMsg.setAddress("/transport/previous")
				oscMsg.append(value)
				print "previous frames : "+str(value)
			elif (msg[0] == 's'):
				#change Sequence
				oscMsg.setAddress("/transport/changeSeq")
				oscMsg.append(value)
				print "change Sequence:"+str(value)
			try:
				client.send(oscMsg)
			except Exception, e:
				print e
			
		time.sleep(0.01)


