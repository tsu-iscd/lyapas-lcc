#!/usr/bin/env python2.7
import json
import argparse
import codecs
import sys

def main(args):
    data = args.in_lyapas.read()
    data = json.dumps(data, ensure_ascii=False, encoding='utf-8')
    json_data = '{"file": "' + args.in_lyapas.name + '",' + ' "source": ' + data +'}' 
    args.out_filename.write(json_data)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Getting json from lyapas sourses')
    parser.add_argument('in_lyapas', help='Path in filesystem for input lyapas-file', nargs='?', type=argparse.FileType('r'), default=sys.stdin)
    parser.add_argument('-out_filename', help='Name of output file', nargs='?', type=argparse.FileType('w'), default=sys.stdout)
    args = parser.parse_args()
    main(args)
