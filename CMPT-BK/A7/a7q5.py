def recsumlist(alist):
    if len(alist)==0:
        return 0
    else:
        return alist[-1]+recsumlist(alist[:-1])

def recmemberlist(target, alist):
    if len(alist)==0:
        return False
    else:
        return target==alist[-1] or recmemberlist(target,alist[:-1])

def reccountlist(target, alist) :
    if len(alist)==0:
        return 0
    elif target==alist[-1]:
        return 1+reccountlist(target, alist[:-1])
    else:
        return reccountlist(target, alist[:-1])

def subst_str(t,r,s):
    if len(s)==0:
        return ""
    elif s[-1] == t:
        return subst_str(t,r,s[:-1])+r
    else:
        return subst_str(t,r,s[:-1])+s[-1]