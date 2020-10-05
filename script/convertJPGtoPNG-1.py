import os

# get images from folder
def getImgPaths(_path):
    files = []
    for r, d, f in os.walk(_path):
        for file in f:
            if(file[0] == '.'):
                continue
            files.append(file)
    return files

# set folders

for i in range(1):

    folderToProcess = "E:/MecaniquePanorama/"+str(i+1)
    #folderToProcess = "/Volumes/FAT32/MecaniquePanorama/"+str(i+1)
    folderToPopulate = "E:/MecaniquePanorama/"+str(i+1)+"-png/"
    #folderToPopulate = "/Volumes/FAT32/MecaniquePanorama/"+str(i+1)+"-png/"
    fileNames = getImgPaths(folderToProcess)
    filesListFull = [folderToProcess + "/" + f for f in fileNames]

    # get img size
    count = 0
    for file in filesListFull:
        print(file)
        fileProcessed = file.split('/')[-1]
        fileProcessed = folderToPopulate + fileProcessed.split('.')[0]+".png"


        # compress
        #ffmpegCmd = "ffmpeg.exe -i {}  {}".format(file, fileProcessed)
        ffmpegCmd = "ffmpeg -i {}  {}".format(file, fileProcessed)
        print(ffmpegCmd)
        os.system(ffmpegCmd)

        count += 1
