from itertools import *

def makeTopology(openedSets):
    l = []
    for a in openedSets:
        for b in openedSets:
            l.append(a & b)
    res = set(l)
    while True:
        changed = False
        for a in l:
            for b in l:
                if a | b not in res:
                    res.add(a | b)
                    changed = True
        l = list(res)
        if not changed:
            break
    return res

def theSet(t):
    result = ""
    for x in t:
        if len(x) > len(result):
            result = x
    return result

def closure(x, t):
    V = theSet(t)
    result = V
    for s in t:
        complement = V - s
        if x <= complement and len(complement) < len(result):
            result = complement
    return frozenset(result)

def interior(x, t):
    V = set()
    result = V
    for s in t:
        if s <= x and len(s) > len(result):
            result = s
    return frozenset(result)

def niceLook(x):
    r = "".join(sorted([c for c in x]))
    return r

def subsets(ss):
    r = chain(*map(lambda x: combinations(ss, x), range(0, len(ss)+1)))
    return list(map(frozenset, r))

def printSolution(A, topology):
    print("A: %s" % niceLook(A))
    cl = closure(A, topology)
    ind = interior(A, topology)
    print("Cl A: %s" % niceLook(cl))
    print("Int A: %s" % niceLook(ind))
    cl_ind = closure(ind, topology)
    ind_cl = interior(cl, topology)
    print("Int Cl A: %s" % niceLook(ind_cl))
    print("Cl Int A: %s" % niceLook(cl_ind))
    cl_ind_cl = closure(ind_cl, topology)
    ind_cl_ind = interior(cl_ind, topology)
    print("Cl Int Cl A: %s" % niceLook(cl_ind_cl))
    print("Int Cl Int A: %s" % niceLook(ind_cl_ind))
    xxx = len(set([A, cl, ind, cl_ind, ind_cl, cl_ind_cl, ind_cl_ind]))
    print("Overall: %d" % xxx)
    
def countMax(topology):
    V = theSet(topology)
    mx = -1
    r = set()
    #print(", ".join(map(niceLook, subsets(V))))
    for A in subsets(V):
        cl = closure(A, topology)
        ind = interior(A, topology)
        cl_ind = closure(ind, topology)
        ind_cl = interior(cl, topology)
        cl_ind_cl = closure(ind_cl, topology)
        ind_cl_ind = interior(cl_ind, topology)
        xxx = len(set([A, cl, ind, cl_ind, ind_cl, cl_ind_cl, ind_cl_ind]))
        if xxx > mx:
            mx = xxx
            r = A
    return r

print("Write all opened sets you know:")
topology = makeTopology(list(map(frozenset, input().split())))
#print("Topology:", ", ".join(map(niceLook, topology)))
# print("Write set A:")
# A = frozenset(input())

A = countMax(topology)
printSolution(A, topology)
