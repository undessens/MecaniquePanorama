import serial
import time
from OSC import OSCClient, OSCMessage




client = OSCClient()
client.connect( ("localhost", 12345))
#ser = serial.Serial('/dev/ttyACM1', 115200)
ser = serial.Serial('/dev/ttyACM0',115200)
value = 0
param = []
msg = []


time.sleep(1)



while 1 :
	msg = (ser.readline())
	if (len(msg)>0):
		
		

		oscMsg = OSCMessage()
		if ( msg[0]== "+") :
			oscMsg.setAddress("/transport/next")
			print "next frame"
		else :
			oscMsg.setAddress("/transport/previous")
			print "previous frame"
		oscMsg.append(1)
		try:
			client.send(oscMsg)
		except Exception, e:
			print e
		
	time.sleep(0.01)