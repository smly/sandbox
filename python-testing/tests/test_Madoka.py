# -*- coding: utf-8 -*-
import unittest
from sandbox import Madoka


class TestMadoka(unittest.TestCase):
    def setUp(self):
        pass

    def test_title(self):
        m = Madoka()
        assert m.title() == 'Puella Magi Madoka Magica'

    def test_actors(self):
        m = Madoka()
        assert len(m.actors().keys()) == 3


class TestPatchMadoka(unittest.TestCase):
    def setUp(self):
        pass

    def test_patch_title(self):
        m = Madoka()
        m.title = lambda: 'MadoMagi'
        assert m.title() == 'MadoMagi'

        m = Madoka()
        m._Madoka__title = 'MadoMagi'
        assert m.title() == 'MadoMagi'

    def test_patch_actors(self):
        m = Madoka()
        assert m.actors()['Akemi Homura'] == 'Chiwa Saitō'

        m = Madoka()
        m._Madoka__actors_by_lang = lambda x: {'Akemi Homura': 'Cristina Vee'}
        assert m.actors()['Akemi Homura'] == 'Cristina Vee'

    def test_patch_actors_by_lang(self):
        m = Madoka()
        assert m._Madoka__actors_by_lang('ja')['Akemi Homura'] == 'Chiwa Saitō'

        m._Madoka__cast = {'Akemi Homura': {'ja': 'Onaka Suita'}}
        assert m._Madoka__actors_by_lang('ja')['Akemi Homura'] == 'Onaka Suita'
