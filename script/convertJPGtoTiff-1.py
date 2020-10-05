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
folderToProcess = "E:/MecaniquePanorama/1"
folderToPopulate = "E:/MecaniquePanorama/1-tif/"
fileNames = getImgPaths(folderToProcess)
filesListFull = [folderToProcess + "/" + f for f in fileNames]

# get img size
count = 0
print "1st file : "+filesListFull[0]
for file in filesListFull:
    print(file)
    fileProcessed = file.split('/')[-1]
    fileProcessed = folderToPopulate + fileProcessed.split('.')[0]+".tiff"


    # compress
    ffmpegCmd = "ffmpeg.exe -i {}  {}".format(file, fileProcessed)
    print(ffmpegCmd)
    os.system(ffmpegCmd)

    count += 1
