#!/usr/bin/env python2.7
import json
import argparse
import sys

def process_args(datum):
    if "args" in datum:
        args = datum["args"]
        processed_args = []
        for arg in args:
            if isinstance(arg, int) or isinstance(arg, long):
                processed_args.append(str(arg))
            elif isinstance(arg, basestring):
                processed_args.append(arg)
            else:
                raise TypeError('Unexpected type "' + type(arg).__name__ + '"')
        return processed_args
    return []

def get_label_name(datum):
    if "number" in datum:
        return str(datum["number"])
    elif "name" in datum:
        return datum["name"]
    elif "args" in datum and isinstance(datum["args"], (list,)) and len(datum["args"]) == 1:
        return str(datum["args"][0])

def convert(datum):
    datum_type = datum["type"]
    if datum_type == "cmd":
        return datum["cmd"] + " " + ", ".join(process_args(datum)) + "\n"
    elif datum_type == "label":
        return get_label_name(datum) + ":\n"
    elif datum_type in ["definition", "call"]:
        return datum_type + " " + ", ".join(process_args(datum)) + "\n"
    else:
        raise NameError('Unknown type "' + datum_type + '"')

def main(args):
    json_data = json.load(args.in_file)
    for datum in json_data:
        args.out_file.write(convert(datum))
                
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Converting json to human readable format")
    parser.add_argument("in_file", help="Path for input json file", nargs="?", type=argparse.FileType("r"), default=sys.stdin)
    parser.add_argument("out_file", help="Path for output file", nargs="?", type=argparse.FileType("w"), default=sys.stdout)
    args = parser.parse_args()
    main(args)
