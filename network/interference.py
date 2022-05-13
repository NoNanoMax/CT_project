from PIL import Image
import numpy as np
from gru import GRU
import mesh
import torch

def load_images():
    img_h = 240
    img_w = 240
    
    imgs = []
    
    for i in range(2):
        img = Image.open('imgs/img%d.png' % i)
        img = img.resize((img_h, img_w), Image.ANTIALIAS)
        imgs.append([np.array(img).transpose( \
                        (2, 0, 1)).astype(np.float32)])
    ims_np = np.array(imgs).astype(np.float32)
    return torch.from_numpy(ims_np)

if __name__ == "__main__":
    pred_file_name = 'prediction.obj'
    

