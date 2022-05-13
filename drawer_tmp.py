from cmath import pi
from typing import Union, Any

import numpy as np
from matplotlib import pyplot as plt
import sys
from subprocess import Popen, PIPE

def useless_func():
    a = np.random.randn(256, 256, 3)
    a = np.abs(a)
    a[a > 1] = 0
    plt.imshow(a)
    plt.axis(False)
    plt.show()

def brightness_normalise(arr, out, H, W):
    norm = 1
    if np.percentile(np.array(out[3:], dtype='float'),  99.95) != 0:
        norm = np.percentile(np.array(out[3:], dtype='float'),  99.95)
    arr /= norm
    for i in range(0, H):
        for j in range(0, W):
            arr[i][j][0] = min(arr[i][j][0], 1)
            arr[i][j][1] = min(arr[i][j][1], 1)
            arr[i][j][2] = min(arr[i][j][2], 1)
    return arr


def draw_men_with_dumplings_lol(PATH=None):
    '''
        input: (C, H, W) matrix
    '''
    if PATH is None:
        sub_proc = Popen(["./run"], stdout=PIPE, stderr=PIPE, universal_newlines=True)
    (out, err) = sub_proc.communicate()

    if err is not None:
        print("azaa")
        sub_proc.kill()
        print(err.strip())

    if out is None:
        return 

    out = out.strip().split()
    
    H, W, C = int(out[0]), int(out[1]), int(out[2])

    picture = np.array(out[3:], dtype='float').reshape(H, W, C)

    picture = brightness_normalise(picture, out, H, W)


    plt.axis(False)
    plt.imshow(picture)
    plt.savefig('scene.png', dpi = 300)

if __name__ == "__main__":
    PATH = None
    if len(sys.argv) > 1:
        PATH = sys.argv[1].strip()
    draw_men_with_dumplings_lol(PATH)