import os
import random

from matplotlib import cm
from matplotlib.image import imsave

import numpy as np

viridis = cm.viridis.colors
num_viridis = len(viridis)

dir_path = os.path.abspath(os.path.dirname(__file__))
dir_path = os.path.join(dir_path, 'pics')

for index in range(0, 100):
    pick = viridis[random.randrange(num_viridis - 1)]
    array = np.tile(pick, (50, 50, 1))
    imsave('{}/{}.png'.format(dir_path, index), array)
