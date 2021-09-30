import serial
import time
import os
import sys
import glob
import subprocess 
from OSC import OSCClient, OSCMessage

#OS choose ofApp working directory
#os.chdir('ImageSeqFromOSC/')

def start_app():

	global mpPath
	global ofAppName
	print("========= START OF APP======")
	if sys.platform.startswith('darwin'):
		cmd = [mpPath+"/"+ofAppName+"/bin/"+ofAppName+"Debug.app/Contents/MacOS/"+ofAppName+"Debug"]
	elif sys.platform.startswith('win'):
		cmd = [os.path.join(mpPath, ofAppName, 'bin', ofAppName+'.exe')]
	print("launch : ")
	print(cmd)
	subprocess.Popen(cmd)
	print(cmd)
	print("======== OF APP STARTED ====")

def kill_app():
	print("========= KILL OF APP ======")
	path = os.path.join(mpPath, "script")
	os.chdir(path)
	print("Path is "+path)
	if sys.platform.startswith('win'):
		subprocess.call([r'quit_app.bat'])
	else:
		subprocess.call(['./quit_app.sh'])

def shutdown_computer():
	print("========= SHUTDOWN COMPUTER ======")
	path = os.path.join(mpPath, "script")
	os.chdir(path)
	print("Path is "+path)
	if sys.platform.startswith('win'):
		subprocess.call(["shutdown", "/s"])
	else:
		subprocess.call(['sudo', 'shutdown', 'now'])
	

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
            s = serial.Serial(port,115200)
            s.close()
            print "Serial found in "+port
            result.append(port)
        except :
            print "probleme ouverture serial : "+port
            pass
    return result


if __name__ == '__main__':

	#Serial print port available
	serialNames = serial_ports()
	print "liste des ports serial"
	print serialNames

	#Final Path
	global mpPath
	global ofAppName
	ofAppName = "ImageSeqFromOSC"
	if sys.platform.startswith('win'):
		mpPath = os.path.join('C:',os.sep,'Users','Aurelien', 'Documents', 'OPENFRAMEWORKS', 'of_v0.10.0_vs2017_release', 'apps', 'MecaniquePanorama')
	elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
		# this excludes your current terminal "/dev/tty"
		mpPath = "/home/nano/Dev/of/apps/MecaniquePanorama/"
	elif sys.platform.startswith('darwin'):
		mpPath = "/Users/adminmac/Boulot/Mecanique-Panorama/GIT/MecaniquePanorama"
	else:
		raise EnvironmentError('Unsupported platform')



	#Serial connect
	try:
		if sys.platform.startswith('darwin'):
			ser = serial.Serial((serial_ports())[1],115200)
		elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
			ser = serial.Serial('/dev/ttyACM0',115200)
			#ser = serial.Serial('/dev/ttyACM1', 115200)
		elif sys.platform.startswith('win'):
			serialId=1
			print("connection au Serial Windows : "+serialNames[serialId])
			ser = serial.Serial(serialNames[serialId],115200)
		else:
			ser = None
	except :
		print "Impossible to connect to Serial"
		ser = None	


	#Start OF app
	#stop_app()
	time.sleep(1)
	start_app()

	#OSC
	client = OSCClient()
	client.connect( ("localhost", 12345))


	value = 0
	param = []
	msg = []



	while 1 :

		isMessage = False
		finalValue = -1000
		while ser.in_waiting:

			if(ser):
				msg = (ser.readline())
			else :	
				msg = ""

			if (len(msg)>1):

				oscMsg = OSCMessage()
				value = ord(msg[1]) - ord("0")
				
				if ( msg[0]== "-") :
					if finalValue == -1000:
						finalValue = 1
					#oscMsg.setAddress("/transport/next")
					#oscMsg.append(value*(value/4))
					#oscMsg.append(value)
					finalValue += value
					isMessage = True
					#print "next frames :"+str(value)
				elif ( msg[0] == '+') :
					if finalValue == -1000:
						finalValue = -1
					#oscMsg.setAddress("/transport/previous")
					#oscMsg.append(value*(value/4))
					#oscMsg.append(value) 
					finalValue -= value
					isMessage = True
					#print "previous frames : "+str(value)
				#elif (msg[0] == 's'):
					#change Sequence
					#oscMsg.setAddress("/transport/changeSeq")
					#oscMsg.append(value)
					#isMessage = True
					#print "change Sequence:"+str(value)
				elif (msg[0] == 'q'):
					#Shutdown computer
					#oscMsg.setAddress("/transport/changeSeq")
					#oscMsg.append(value)
					#isMessage = True
					#print "change Sequence:"+str(value)
					shutdown_computer()
				
		if(isMessage ):
			try:
				if(finalValue>0):
					oscMsg.setAddress("/transport/next")
					print "next frames :"+str(finalValue)
				else:
					oscMsg.setAddress("/transport/previous")
					print "previous frames : "+str(finalValue)
				oscMsg.append(abs(finalValue))
				client.send(oscMsg)
			except Exception, e:
				print e	
		
		time.sleep(0.055)


