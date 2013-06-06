# -*- coding: utf-8 -*-


def transform(typeinfo, strvals_dict):
    """
    Return typed-values from string-values
    """
    row = {}
    for fieldname in strvals_dict:
        typename = typeinfo.typename(fieldname)

        if typename == 'int':
            if strvals_dict[fieldname] == '':
                row[fieldname] = None
            else:
                row[fieldname] = int(strvals_dict[fieldname])
        elif typename == 'float':
            if strvals_dict[fieldname] == '':
                row[fieldname] = None
            else:
                row[fieldname] = float(strvals_dict[fieldname])
        elif typename == 'bool':
            if strvals_dict[fieldname] == '':
                row[fieldname] = None
            else:
                row[fieldname] = bool(strvals_dict[fieldname])
        else:
            row[fieldname] = strvals_dict[fieldname]

    return row
