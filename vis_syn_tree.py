import json
import argparse
from graphviz import Digraph

tree = Digraph(comment='lyapas_im0_syntax_tree', format='png')
num_of_nodes = 0

# This function generates node's name of syntax tree
# Including all fields from json-object
def gen_node_name(json):
    global num_of_nodes
    s = ''
    for key in json:
        s = s + key + '->'+ str(json[key]) +'\n'
    num_of_nodes +=1
    return s + str(num_of_nodes)

# Recursive procedure to generate dot-object for syntax tree
def build_tree(name_node, json_childs):
    if len(json_childs) >= 1:
        for child in json_childs:
            name_child = gen_node_name(child[0])
            tree.node(name_child)
            tree.edge(name_node, name_child)
            build_tree(name_child, child[1:])

def main(args):
    json_im0 = json.load(args.in_json)
    name_node = gen_node_name(json_im0[0])
    tree.node(name_node)
    build_tree(name_node, json_im0[1:])
    tree.render(args.out_filename)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generation of pretty syntax tree')
    parser.add_argument('in_json', type=argparse.FileType('r'),help='Path in filesystem for input json-file')
    parser.add_argument('-out_filename', default='syntree', help='Name of output file')
    args = parser.parse_args()
    main(args)
