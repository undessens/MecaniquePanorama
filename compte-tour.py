import serial
import time
import os
from OSC import OSCClient, OSCMessage

#OS choose ofApp working directory
os.chdir('ImageSeqFromOSC/')

def lauch_app():
	
	os.system('make run')

def stop_app():
	os.system('killall "ImageSeqFromOSC" ')

#Serial
try:
	ser = serial.Serial('/dev/ttyACM0',115200)
	#ser = serial.Serial('/dev/ttyACM1', 115200)
except :
	print "ValueError"
	ser = None	

#Start OF app
stop_app()
time.sleep(1)
lauch_app(	)

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


	if (len(msg)>0):
	##########
	#TODO
	# ajouter un numero en plus au msg, lié a la vitesse
	#rajouter cette vitesse dans le message osc
	# changer les images en fonction de cette vitesse
	#########
		
		

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


