from lxml import etree as ET
import os
import sys



def lauch_app():
	
	x = 0


if __name__ == '__main__':

	print "########################"
	print "SCANNER MECANIQUE PANORAMA"
	print "########################"
	print ""

	path_MP = ""
	if sys.platform.startswith('win'):
		path_MP = "E:/mecaniquePanorama"
	elif sys.platform.startswith('darwin'):
		path_MP = "/Users/aurelienconil/Documents/of_v0.9.8_osx_release/apps/mecaniquePanorama/ImageSeqFromOSC/bin/data"

	
	# List of global variable
	nbFolder = 0
	listOfFiles = []
	listOfnbFiles = []
	nbFiles = 0
	listOfFirst = []
	listOfDigit = []
	error = False

	# Scan Folder number
	for x in range(1,12):
		
		complete_path = path_MP+"/"+str(x)

		if(os.path.isdir(complete_path)):
			print str(x)+" exist"
			nbFolder = nbFolder + 1
		else :
			break

	print "Nb folder : "+str(nbFolder)

	# Scan each folder
	for x in range(nbFolder) : 
		print "============= "+"Folder "+str(x+1)+" =========="
		listOfFiles = os.listdir(path_MP+"/"+str(x+1))
		listOfnbFiles.append( len(listOfFiles))
		nbFiles = len(listOfFiles)
		nbImage = 0
		
		print "nb files : "+str(nbFiles)

		
		# Check intro.png file
		if (os.path.isfile(path_MP+"/"+str(x+1))):
			" File : intro.png    :::  EXIST "
			nbImage = nbFiles - 1
			error = True
		else :
			" File : intro.png    !!!  DON'T EXIST !!!"
			nbImage = nbFiles 
		
		listOfFirst.append(listOfFiles[0])
		numberOfFirst = int(listOfFirst[x].split(".")[0])
		print "1st image : " +listOfFirst[x]+ " number : "+str(numberOfFirst)

		# Digit calculation
		nbDigit =len(listOfFirst[x].split(".")[0])
		listOfDigit.append(nbDigit)

		for i in range( numberOfFirst, numberOfFirst + (nbImage-1)) :
			# fist calculate the number of digit in photo name file
			zeroToAdd = nbDigit - len(str(i))
			completeFileName = ""
			for j in range(zeroToAdd):
				completeFileName = completeFileName+"0"
			completeFileName = path_MP+"/"+str(x+1)+"/"+completeFileName+str(i)+".jpg"
			
			if( not(os.path.isfile(completeFileName))):
				print "ERREUR FICHIER MANQUANT : "+completeFileName
				error = True
		if(not(error)):
			print "-----------> " +str(nbFiles)+" FILES OK & INTRO.PNG EXISTS"

	#print for errors
	if(error):
		print "#################### ERROR DURING SCANNING ###########"
	else : 

		#end of scanning : time to print xml file	
		print ""
		print "=============== Writing XML file ==================="
		print "nb of foler",
		print nbFolder
		print "nbFiles : ",
		print listOfnbFiles 
		print "first file: ",
		print listOfFirst 
		print "nb of digit : ",
		print listOfDigit 

		scan = ET.Element('SCAN')
		for x in range(nbFolder) : 
			seq = ET.SubElement(scan, 'SEQ')
			nbFiles = ET.SubElement(seq, 'NBFILES')
			nbFiles.text = str(listOfnbFiles[x])
			nbDigit = ET.SubElement(seq, 'NBDIGIT')
			nbDigit.text = str(listOfDigit[x])
			nbDigit = ET.SubElement(seq, 'START')
			nbDigit.text = listOfFirst[x].split(".")[0]

		mydata = ET.tostring(scan)
		myfile = open(path_MP+"/scanNONO.xml", "w"	)
		myfile.write(mydata)


	print "################### END ##################"




			

















