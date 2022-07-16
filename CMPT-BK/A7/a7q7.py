from node import node
def sumnc_rec(chain:node):
    if chain is None:
        return 0
    else:
        return chain.get_data()+sumnc_rec(chain.get_next())
def membernc_rec(target, chain:node):
    if chain is None:
        return False
    else:
        return chain.get_data()==target or membernc_rec(target, chain.get_next())
def countnc_rec(target, chain:node):
    if chain is None:
        return 0
    elif chain.get_data()==target:
        return 1+countnc_rec(target, chain.get_next())
    else:
        return countnc_rec(target, chain.get_next())
