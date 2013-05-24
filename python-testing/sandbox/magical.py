# -*- coding: utf-8 -*-
from six import iteritems


class Madoka(object):
    def __init__(self):
        self.__title = "Puella Magi Madoka Magica"
        self.__cast = {
            'Madoka Kaname': {
                'ja': 'Aoi Yūki',
                'en': 'Christine Marie Cabanos',
            },
            'Akemi Homura': {
                'ja': 'Chiwa Saitō',
                'en': 'Cristina Vee',
            },
            'Kyōko Sakura': {
                'ja': 'Ai Nonaka',
                'en': 'Lauren Landa',
            },
        }

    def title(self):
        return self.__title

    def actors(self, lang='ja'):
        return self.__actors_by_lang(lang)

    def __actors_by_lang(self, lang):
        return {name: bylang[lang] for name, bylang in iteritems(self.__cast)}
