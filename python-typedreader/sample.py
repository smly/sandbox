#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import typedreader


def parse_args():
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('-c', '--csvfile',
                        help='')
    parser.add_argument('-t', '--typefile',
                        help='')
    return parser.parse_args()


if __name__ == '__main__':
    args = parse_args()
    with open(args.csvfile, 'r') as csvfh:
        typeinfo, reader = typedreader.reader(csvfh, args.typefile)
        for row in reader:
            print(row)
