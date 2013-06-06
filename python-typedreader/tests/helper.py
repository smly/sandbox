# -*- coding: utf-8 -*-
from six import b
import os
import tempfile
from contextlib import contextmanager


@contextmanager
def tempinput(data):
    temp = tempfile.NamedTemporaryFile(delete=False)
    temp.write(b(data))
    temp.close()
    yield temp.name
    os.unlink(temp.name)
