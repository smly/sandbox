# -*- coding: utf-8 -*-
from flyrc import client, util, handler
from six.moves import configparser
import subprocess
import re

CONFIG_FILE = "settings.ini"


class Bell(object):
    def __init__(self, config):
        self.player_path = config.get('sound', 'media_player')
        self.conf = config

    def make_proc(self, ogg_file):
        return subprocess.Popen(
            [self.player_path, ogg_file],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT)

    def hit(self, bell_mode):
        proc = self.make_proc(self.conf.get('sound', bell_mode))
        proc.communicate()


class AlertHandler(object):
    DEPENDENCIES = [handler.MessageProcessor]

    def __init__(self, conf):
        self.bell = Bell(conf)
        self.conf = conf

    def irc_channel_message(self, client, source, target, text):
        for option in self.conf.options('match'):
            pattern_raw = self.conf.get('match', option)
            bell_mode = pattern_raw.split(",")[0]
            pattern = ",".join(pattern_raw.split(",")[1:])
            match = re.match(pattern, text)
            if match:
                print("channel: {}, text: {}".format(target, text))
                self.bell.hit(bell_mode)


config = configparser.ConfigParser()
config.readfp(open(CONFIG_FILE))
cli = client.SimpleClient(
    config.get('irc', 'loginname'),
    config.get('irc', 'nickname'),
    config.get('irc', 'desc'),
    config.get('irc', 'server'),
    int(config.get('irc', 'port')),
    ssl=False)
cli.add_handler(AlertHandler(config))
util.run_client(cli)
