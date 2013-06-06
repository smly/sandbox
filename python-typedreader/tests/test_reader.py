# -*- coding: utf-8 -*-
import unittest
from tests.helper import tempinput
import typedreader
from typedreader.exception import (FieldnameMissmatch, TypenameNotDefined)


class TestReader(unittest.TestCase):
    def test_reader_no_typename(self):
        csv_filegen = tempinput("""name,age,gender,score
Akari,9,f,
Kyoko,10,f,0.06
Yui,10,f,0.08
Chinatsu,9,f,0.04
""")
        type_filegen = tempinput("""[setting]
optional = score
""")
        with csv_filegen as csv_filename, type_filegen as type_filename:
            with open(csv_filename, 'r') as csv_fh:
                self.assertRaises(TypenameNotDefined,
                                  typedreader.reader,
                                  *[csv_fh, type_filename])

    def test_reader_no_fieldname(self):
        csv_filegen = tempinput("""Akari,9,f,
Kyoko,10,f,0.06
Yui,10,f,0.08
Chinatsu,9,f,0.04
""")
        type_filegen = tempinput("""[typename]
name = string
age = int
gender = string
score = float

[setting]
optional = score
""")
        with csv_filegen as csv_filename, type_filegen as type_filename:
            with open(csv_filename, 'r') as csv_fh:
                self.assertRaises(FieldnameMissmatch,
                                  typedreader.reader,
                                  *[csv_fh, type_filename])

    def test_reader_typeinfo_invalid_fieldname(self):
        csv_filegen = tempinput("""name,age,gender,score
Akari,9,f,
Kyoko,10,f,0.06
Yui,10,f,0.08
Chinatsu,9,f,0.04
""")
        type_filegen = tempinput("""[typename]
name = string
age = int
gender = string
score = float

[setting]
optional = score
""")
        with csv_filegen as csv_filename, type_filegen as type_filename:
            with open(csv_filename, 'r') as csv_fh:
                typeinfo, reader = typedreader.reader(csv_fh, type_filename)

        assert typeinfo.required('\(^o^)/') is None

    def test_reader(self):
        csv_filegen = tempinput("""name,age,gender,score
Akari,9,f,
Kyoko,10,f,0.06
Yui,10,f,0.08
Chinatsu,9,f,0.04
""")
        type_filegen = tempinput("""[typename]
name = string
age = int
gender = string
score = float

[setting]
optional = score
""")
        with csv_filegen as csv_filename, type_filegen as type_filename:
            with open(csv_filename, 'r') as csv_fh:
                typeinfo, reader = typedreader.reader(csv_fh, type_filename)
                for row in reader:
                    assert type(row['name']) is str
                    assert type(row['age']) is int
                    assert type(row['gender']) is str
                    assert type(row['score']) is float or row['score'] is None

                assert typeinfo.required('name') is True
                assert typeinfo.required('age') is True
                assert typeinfo.required('score') is False
