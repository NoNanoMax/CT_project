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

def draw_men_with_dumplings_lol(PATH=None):
    '''
        input: (C, H, W) matrix
    '''
    if PATH is None:
        sub_proc = Popen(["./run"], stdout=PIPE, stderr=PIPE, universal_newlines=True)
    (out, err) = sub_proc.communicate()
    out = out.strip().split()
    if len(err.strip()) != 0:
        return err.strip()
    H, W, C = int(out[0]), int(out[1]), int(out[2])
    picture = np.array(out[3:], dtype=float).reshape((H, W, C)) / 255
    plt.axis(False)
    plt.imshow(picture)
    plt.show()

if __name__ == "__main__":
    PATH = None
    if len(sys.argv) > 1:
        PATH = sys.argv[1].strip()
    draw_men_with_dumplings_lol(PATH)
