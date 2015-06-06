from mpl_toolkits.basemap import Basemap
import numpy as np


class GeoMap(object):
    def __init__(self, lat, lon, zoom, width, height):
        self.lat = lat
        self.lon = lon
        self.zoom = zoom
        self.width = width
        self.height = height

        lon_s, lat_s, lon_e, lat_e = latlon2box(lat, lon, zoom, width, height)
        self.m = Basemap(llcrnrlon=lon_s, llcrnrlat=lat_s,
            urcrnrlon=lon_e, urcrnrlat=lat_e,
            resolution='h', projection='merc')
        xs, ys = self.m(lon_s, lat_s)
        xe, ye = self.m(lon_e, lat_e)
        self.xs = xs
        self.ys = ys
        self.xe = xe
        self.ye = ye

    def to_point(self, longs, lats):
        x1, y1 = self.m(longs, lats)
        xpix = self.width * (np.array(x1) - self.xs)/(self.xe - self.xs)
        ypix = self.height - self.height * (np.array(y1) - self.ys)/(self.ye - self.ys)
        return xpix, ypix


def get_tile_number(lat, lon, zoom):
    xtile = (lon + 180) / 360 * 2 ** zoom
    ytile = (1 - np.log(np.tan(np.radians(lat)) + 1/np.cos(np.radians(lat))) / np.pi) / 2 * 2 ** zoom
    return xtile, ytile

def get_lon_lat(xtile, ytile, zoom):
    n = 2 ** zoom
    lon_deg = xtile / n * 360.0 - 180.0
    lat_deg = np.degrees(np.arctan(np.sinh(np.pi * (1 - 2 * ytile / n))))
    return lon_deg, lat_deg

def latlon2box(lat, lon, zoom, width, height):
    tile_size = 256.0

    xtile, ytile = get_tile_number(lat, lon, zoom)
    xtile_s = (xtile * tile_size - width / 2) / tile_size
    ytile_s = (ytile * tile_size - height / 2) / tile_size
    xtile_e = (xtile * tile_size + width / 2) / tile_size
    ytile_e = (ytile * tile_size + height / 2) / tile_size

    lon_s, lat_s = get_lon_lat(xtile_s, ytile_s, zoom)
    lon_e, lat_e = get_lon_lat(xtile_e, ytile_e, zoom)
    return lon_s, lat_s, lon_e, lat_e


