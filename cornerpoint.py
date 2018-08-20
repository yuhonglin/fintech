# base codes are copied from generate.py

## generate corner points and tangent points

import matplotlib.pyplot as plt
import glob
import copy
import numpy as np

inputDir = "./tmp/"


######################################## load data
index_constant = []
for f in glob.glob(inputDir + '/' + 'data_*'):
    index = int(f.split('/')[-1].split('_')[-1])
    constant = []
    for firstline in open(f):
        constant.append([float(x) for x in firstline.split('\t') if x.strip('\n\t ')!=''])
    index_constant.append((index, constant))

## gradient
gradient = []
for l in open(inputDir + '/gradient'):
    gradient.append([float(x) for x in l.split('\t') if x.strip('\n\t ')!=''])

## index and states
def load_stateprofile(f = inputDir + '/state_profile'):
    ret = {}
    for l in open(f):
        ri, rs = l.split('\t')
        i = ri.strip('()\n\t ')
        s = []
        for x in rs.split('|'):
            tx = x.strip('\n\t, ')
            if tx == '':
                continue
            s.append([float(x) for x in tx.split(',')])
        ret[int(i)] = s
    return ret

index_state = load_stateprofile()
state_index = {}
for k, v in index_state.iteritems():
    t = tuple((tuple(i) for i in v))
    state_index[t] = k


####################################### corner points
def get_cross(g1, w1, g2, w2, eps=1e-10):
    """
    Return the cross points of two lines.
    Will return NULL if the two points are too close (dist<eps).
    """
    a = g1[0]; b = g1[1];
    c = g2[0]; d = g2[1];
    den = a*d - b*c
    if abs(den) < eps:
        return None
    return [ (d*w1-b*w2)/den, (-c*w1+a*w2)/den ];

def get_all_cross(grads, ws, eps=1e-10): ## used to be 1e-8
    all = []
    for i in range(0, len(grads)):
        for j in range(i+1, len(grads)):
            cross = get_cross(grads[i], ws[i], grads[j], ws[j], eps)
            if cross != None:
                all.append(cross)
    # filter out invalid cross
    ret = []
    for p in all:
        valid = True
        for g, w in zip(grads, ws):
            if (g[0]*p[0] + g[1]*p[1] >= w+eps):
                valid = False
                break;
        if valid:
            ret.append(p)
    return ret

def sort_cross(cross):
    """
    Sort the cross anti-clock order.
    """
    ret = []
    cp = copy.deepcopy(cross)
    # find the one with lowest y value
    tmp = cp[0][1]
    lyi = 0
    for i in range(len(cp)):
        if cp[i][1] < tmp:
            lyi = i
            tmp = cp[i][1]
    ret.append(cp.pop(lyi))
    # compute the cos angle of all the other points
    cos_idx = []
    r1 = ret[0][0]; r2 = ret[0][1]
    for i, p in enumerate(cp):
        s1 = p[0]-r1; s2 = p[1]-r2
        nrm = pow(s1*s1+s2*s2,.5)
        if nrm < 1e-8:
            continue
        cos_idx.append(( s1/nrm ,i))
    for _,i in sorted(cos_idx):
        ret.append(cp[i])
    return ret
    
    
def get_corner_xy(stateprof):
    a, b = stateprof
    idx_to_plot = state_index[((a,),(b,))]
    _, constant = sorted(index_constant)[-1]
    const = constant[idx_to_plot]
    cross = get_all_cross(gradient, const)
    eps = 1e-10
    while len(cross)==0:
        eps = eps * 10
        cross = get_all_cross(gradient, const, eps)
        
    x, y  = zip(*sort_cross(cross))
    x = list(x)
    y = list(y)
    x.append(x[0])
    y.append(y[0])
    
    return (x,y)


## dump stateProfileIndex_cornerPoints (commented and moved to run.py)
# stateProfileIndex_cornerPoints = {}
# for i, v in index_state.iteritems():
#     a, b = v
#     x, y = get_corner_xy((a[0],b[0]))
#     stateProfileIndex_cornerPoints[i] = {'x': x, 'y': y}
# print str(stateProfileIndex_cornerPoints)    


