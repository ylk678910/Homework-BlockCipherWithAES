import sys
import os
from PIL import Image

ImagePath = sys.argv[1]
OutputImage_StorePath = sys.argv[2]
Extension = sys.argv[3]

ImagePathSpilt = os.path.split(sys.argv[1])
ImageNameOutputImage = ImagePathSpilt[-1]
Temp = ImageNameOutputImage.split('.')
ImageName = Temp[0]
OutputImageName = OutputImage_StorePath + '/' + ImageName + '.' + Extension

im = Image.open(ImagePath)
im.save(OutputImageName)
