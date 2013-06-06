# -*- coding: utf-8 -*-
import typedreader
from tests.helper import tempinput


def test_transform():
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

        strvals_dict = {'name': 'Yui', 'age': '10', 'gender': 'f', 'score': ''}
        with csv_filegen as csv_filename, type_filegen as type_filename:
            with open(csv_filename, 'r') as csv_fh:
                typeinfo = typedreader.typeinfo(csv_fh, type_filename)

        typedvals_dict = typedreader.transform(typeinfo, strvals_dict)

        assert type(typedvals_dict['name']) is str
        assert type(typedvals_dict['age']) is int
        assert type(typedvals_dict['gender']) is str
        assert typedvals_dict['score'] is None

        strvals_dict.update({'score': '0.0'})
        typedvals_dict = typedreader.transform(typeinfo, strvals_dict)

        assert type(typedvals_dict['score']) is float
        assert typedvals_dict['name'] == 'Yui'
        assert typedvals_dict['gender'] == 'f'
        assert typedvals_dict['age'] == 10
        assert typedvals_dict['score'] == 0.0
