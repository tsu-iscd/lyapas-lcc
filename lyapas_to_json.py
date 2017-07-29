#!/usr/bin/env python2.7
import json
import argparse
import codecs

def main(args):
    if args.in_lyapas is None:
        data = ''
        while True:
            try:
                data += raw_input() + '\n'
            except EOFError:
                break
        data = json.dumps(data, ensure_ascii=False, encoding='utf-8')
        json_data = '{"file": stdin, "source": ' + data +'}'
    else:
        with open(args.in_lyapas) as in_f: 
            data = in_f.read()
            data = json.dumps(data, ensure_ascii=False, encoding='utf-8')
            json_data = '{"file": "' + in_f.name + '",' + ' "source": ' + data +'}' 
    
    with open(args.out_filename, 'w') as out_f:     
        out_f.write(json_data)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Getting json from lyapas sourses')
    parser.add_argument("in_lyapas", help='Path in filesystem for input lyapas-file', default=None, nargs='?')
    parser.add_argument('-out_filename', default='ex.json.lyapas', help='Name of output file', nargs='?')
    args = parser.parse_args()
    main(args)
