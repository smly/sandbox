# -*- coding: utf-8 -*-
"""
Dependency:

* brew install geos
* pip install basemap

* http://maps.stamen.com/m2i/image/20150606/mapstack_boglYE6_9JA
* http://mapstack.stamen.com/edit.html#color:$ffffff;mapbox-water[bright=-60,tint=$141a1f@40];parks;terrain-lines[alpha=40]/13/41.159593802241154/-8.60452651977539

Sat Jun 6, 2015 at 01:04 PM
1,112 x 609 px
41° 9' 34.54" N , 8° 36' 16.3" W→Map Stack
"""
import matplotlib.pyplot as plt
from mpl_toolkits.basemap import Basemap
import numpy as np

from geomap import GeoMap


map_fac = 1.5

width = 1112
height = 609

zoom_level = 13
LAT = 41.159593802241154
LON = -8.60452651977539

longs = [LON]
lats = [LAT]

gm = GeoMap(LAT, LON, zoom_level, width, height)
xpix, ypix = gm.to_point(longs, lats)

im = plt.imread("./mapstack_v2.png")
plt.figure(figsize=(map_fac * 9.98, map_fac * 6.7))
plt.imshow(np.flipud(im))
plt.plot(xpix, ypix, 'ro', markersize=5, markeredgecolor='none')
plt.xlim([0, width])
plt.ylim([0, height])
plt.axis('off')
plt.title('taxis running in the city of Porto, in Porugal')
plt.tight_layout()
plt.savefig('test.png')
