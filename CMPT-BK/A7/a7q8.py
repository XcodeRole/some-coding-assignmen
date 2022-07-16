from node import node
def to_string(node_chain:node)->str:
    if node_chain is None:
        return ''
    elif node_chain.get_next()==None:
        return '[ '+str(node_chain.get_data())+ ' | / ]'+to_string(node_chain.get_next())
    else:
        return '[ '+str(node_chain.get_data())+ ' | * -]-->'+to_string(node_chain.get_next())

def check_chains(chain1:node, chain2:node)->str:
    if (chain1 is None and chain2 is not None) or (chain1 is not None and chain2 is None):
        return False
    elif chain1 is None and chain2 is None:
        return True
    else:
        return chain1.get_data()==chain2.get_data() and check_chains(chain1.get_next(),chain2.get_next())

def copy(node_chain:node):
    if node_chain is None:
        return None
    else:
        new_node=node(node_chain.get_data())
        new_node.set_next(copy(node_chain.get_next()))
        return new_node

def replace(node_chain:node, target, replacement):
    if node_chain is None:
        return None
    elif node_chain.get_data()==target:
        node_chain.set_data(replacement)
        node_chain.set_next(replace(node_chain.get_next(),target,replacement))
        return node_chain
    else:
        node_chain.set_next(replace(node_chain.get_next(),target,replacement))
        return node_chain