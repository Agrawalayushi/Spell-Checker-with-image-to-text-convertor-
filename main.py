import pytesseract
from PIL import Image

pytesseract.pytesseract.tesseract_cmd = r"C:\Program Files\Tesseract-OCR\tesseract.exe"
img = Image.open('newest.jpg')
text = pytesseract.image_to_string(img)

print(text)

f = open("testimage.txt","wt")
f.write(text)
f.close()
