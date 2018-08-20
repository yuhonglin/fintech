import matplotlib.pyplot as plt
import glob
import copy
import numpy as np

inputDir = "./tmp/"

## load data
index_constant = []
for f in glob.glob(inputDir + '/' + 'data_*'):
    index = int(f.split('/')[-1].split('_')[-1])
    constant = []
    for firstline in open(f):
        constant.append([float(x) for x in firstline.split('\t') if x.strip('\n\t ')!=''])
    index_constant.append((index, constant))

gradient = []
for l in open(inputDir + '/gradient'):
    gradient.append([float(x) for x in l.split('\t') if x.strip('\n\t ')!=''])


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


## plotting routines
def get_cor(g, c):
    den = g[0]*g[0] + g[1]*g[1]
    return [g[0]*c/den, g[1]*c/den]

def get_cross(g1, w1, g2, w2, eps=1e-10):
    a = g1[0]; b = g1[1];
    c = g2[0]; d = g2[1];
    den = a*d - b*c
    if abs(den) < eps:
        return None
    return [ (d*w1-b*w2)/den, (-c*w1+a*w2)/den ];

def get_all_cross(grads, ws, eps=1e-10):
    all = []
    for i in range(0, len(grads)):
        for j in range(i+1, len(grads)):
            cross = get_cross(grads[i], ws[i], grads[j], ws[j])
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

def stateprofile_to_title(sp):
    s = str(sp).replace('[', '(').replace(']', ')')
    return 'state: ' + s

    
def onlyplot(index_to_plot, nskip = 0, title='', ax=None):
    if nskip < 0:
        nskip = len(index_constant) + nskip
    if ax == None:
        fig, ax = plt.subplots()
    for i, (idx, constant) in enumerate(sorted(index_constant)):
        if i < nskip:
            continue
        const = constant[index_to_plot]
        cross = get_all_cross(gradient, const)
        if len(cross)==0:
            continue
        x, y  = zip(*sort_cross(cross))
        x = list(x)
        y = list(y)
        x.append(x[0])
        y.append(y[0])

        p = ax.plot(x,y, linewidth=3)


def plot_last(idx):
    fig, ax = plt.subplots()
    skip = -1
    for s in idx: onlyplot(s, skip, '', ax)
    ax.legend([stateprofile_to_title(index_state[x]) for x in idx])
    ax.grid('on')
    
    dimy = 0.003
    dimx = [-0.009, 0.014]
    xshift = 0.002304
    
    stateprof = index_state[idx[1]]
    tmp = [x.split('\t')[1].replace(', | ', '),(') for x in get_equi_actprof(stateprof[0][0],stateprof[1][0])]
    tmp = [str(i) + '\t(' + x.replace(', |', ')') for i,x in enumerate(tmp)]
    txt = '\n'.join(tmp)
    fn = 'Action Profile ' + str((stateprof[0][0],stateprof[1][0]))
    open('doc/txt/' + fn + '.txt', 'w').write(txt)

    payoff = get_payoff(stateprof[0][0],stateprof[1][0])
    txt = '\n'.join(['%d\t%0.5f,%0.5f' % (i, x[0], x[1]) for i, x in enumerate(payoff[0])])
    fn = 'Stage Profit ' + str((stateprof[0][0],stateprof[1][0]))
    open('doc/txt/' + fn + '.txt', 'w').write(txt)

    payoff = get_payoff(stateprof[0][0],stateprof[1][0])
    txt = '\n'.join(['%d\t%0.5f,%0.5f' % (i, x[0], x[1]) for i, x in enumerate(payoff[1])])
    fn = 'Continuation Profit ' + str((stateprof[0][0],stateprof[1][0]))
    open('doc/txt/' + fn + '.txt', 'w').write(txt)

    # ------------------------------
    
    stateprof = [[stateprof[0][0]], [stateprof[0][0]]]
    tmp = [x.split('\t')[1].replace(', | ', '),(') for x in get_equi_actprof(stateprof[0][0],stateprof[1][0])]
    tmp = [str(i) + '\t(' + x.replace(', |', ')') for i, x in enumerate(tmp)]
    txt = '\n'.join(tmp)
    fn = 'Action Profit ' + str((stateprof[0][0],stateprof[1][0]))
    open('doc/txt/' + fn + '.txt', 'w').write(txt)

    payoff = get_payoff(stateprof[0][0],stateprof[1][0])
    txt = '\n'.join(['%d\t%0.5f,%0.5f' % (i, x[0], x[1]) for i, x in enumerate(payoff[0])])
    fn = 'Stage Profit ' + str((stateprof[0][0],stateprof[1][0]))
    open('doc/txt/' + fn + '.txt', 'w').write(txt)

    payoff = get_payoff(stateprof[0][0],stateprof[1][0])
    txt = '\n'.join(['%d\t%0.5f,%0.5f' % (i, x[0], x[1]) for i, x in enumerate(payoff[1])])
    fn = 'Continuation Profit ' + str((stateprof[0][0],stateprof[1][0]))
    open('doc/txt/' + fn + '.txt', 'w').write(txt)
    
    ax.set_aspect('equal', 'box')
    ax.set_xlabel('Firm 1')
    ax.set_ylabel('Firm 2')
    plt.show()
    fig.savefig('./doc/img/overlap_%s.pdf' % '_'.join((str(x) for x in idx)) )




def plot_last_state_pair(a, b):
    plot_last([
        state_index[((a,),(a,))],
        state_index[((a,),(b,))],
        state_index[((b,),(a,))],
    ])
    
