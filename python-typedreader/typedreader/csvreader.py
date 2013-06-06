# -*- coding: utf-8 -*-
import typedreader.exception as exc
from six.moves import configparser
import csv


class TypeInfo(object):
    def __init__(self, config, fieldset):
        self.__config = config
        self.__fields = fieldset
        self.__setup_optional()

    def __setup_optional(self):
        val = self.__config.get('setting', 'optional')
        fieldnames = val.split(',')
        self.__optional = set(fieldnames)

    def typename(self, fieldname):
        if not self.__config.has_option('typename', fieldname):
            return None

        return self.__config.get('typename', fieldname)

    def required(self, fieldname):
        if not fieldname in self.__fields:
            return None

        return not fieldname in self.__optional


def _generator(dictreader, config):
    for row in dictreader:
        for fieldname in dictreader.fieldnames:
            typename = config.get('typename', fieldname)

            if typename == 'int':
                if row[fieldname] == '':
                    row[fieldname] = None
                else:
                    row[fieldname] = int(row[fieldname])
            elif typename == 'float':
                if row[fieldname] == '':
                    row[fieldname] = None
                else:
                    row[fieldname] = float(row[fieldname])
            elif typename == 'bool':
                if row[fieldname] == '':
                    row[fieldname] = None
                else:
                    row[fieldname] = bool(row[fieldname])
        yield row


def typeinfo(csv_fh, type_filename, **kwargs):
    """
    Return TypeInfo obj
    """
    config = configparser.ConfigParser()
    config.read(type_filename)
    dictreader = csv.DictReader(csv_fh, **kwargs)

    if not config.has_section('typename'):
        raise exc.TypenameNotDefined()

    fieldset = set(dictreader.fieldnames)
    if fieldset != set(config.options('typename')):
        raise exc.FieldnameMissmatch()

    return TypeInfo(config, fieldset)


def reader(csv_fh, type_filename, **kwargs):
    """
    Return TypeInfo obj and generator
    """
    config = configparser.ConfigParser()
    config.read(type_filename)
    dictreader = csv.DictReader(csv_fh, **kwargs)

    if not config.has_section('typename'):
        raise exc.TypenameNotDefined()

    fieldset = set(dictreader.fieldnames)
    if fieldset != set(config.options('typename')):
        raise exc.FieldnameMissmatch()

    return TypeInfo(config, fieldset), _generator(dictreader, config)
