#!/usr/bin/env python2.7
import json
import argparse
import codecs

def main(args):
    with open(args.in_lyapas) as in_f, open(args.out_filename, 'w') as out_f:
        data = in_f.read()
        data = json.dumps(data, ensure_ascii=False, encoding='utf-8')
        json_data = '{"file": "' + in_f.name + '",' + ' "source": ' + data +'}'
        out_f.write(json_data)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Getting json from lyapas sourses')
    parser.add_argument("in_lyapas", help='Path in filesystem for input lyapas-file')
    parser.add_argument('-out_filename', default='ex.json.lyapas', help='Name of output file')
    args = parser.parse_args()
    main(args)
