#!/usr/bin/env python2.7
import json
import argparse

def process_args(datum):
    if "args" in datum:
        args = datum["args"]
        processed_args = []
        for arg in args:
            if isinstance(arg, int):
                processed_args.append(str(arg))
            elif isinstance(arg, basestring):
                processed_args.append(arg)
            else:
                raise TypeError('Unexpected type "' + type(arg).__name__ + '"')
        return processed_args
    return [] 

def convert(datum):
    datum_type = datum["type"]
    if datum_type == "cmd":
        return datum["cmd"] + " " + ", ".join(process_args(datum)) + "\n"
    elif datum_type == "label":
        return "label " + str(datum["number"]) + "\n"
    elif datum_type == "definition":
        return "definition" + " " + ", ".join(process_args(datum)) + "\n"
    else:
        raise NameError('Unknown type "' + datum_type + '"')

def main(args):
    with open(args.in_filename, "r") as in_file, open(args.out_filename, "w") as out_file:
        json_string = in_file.read()
        json_data = json.loads(json_string)
        for datum in json_data:
            out_file.write(convert(datum))
                
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Converting json to human readable format")
    parser.add_argument("in_filename", help="Path for input json file")
    parser.add_argument("out_filename", help="Path for output file")
    args = parser.parse_args()
    main(args)
