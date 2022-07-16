def recsumlisti(index, alist):
    if len(alist[index:])==0:
        return 0
    else:
        return alist[-1]+recsumlisti(index, alist[:-1])

def recmemberlisti(index, target, alist):
    if len(alist)==0 or index>=len(alist) or len(alist[index:])==0:
        return False
    else:
        return alist[-1]==target or recmemberlisti(index, target, alist[:-1])

def reccountlisti(index, target, alist):
    if len(alist)==0 or index>=len(alist) or len(alist[index:])==0:
        return 0
    elif alist[-1]==target:
        return 1+ reccountlisti(index, target, alist[:-1])
    else:
        return reccountlisti(index, target, alist[:-1])