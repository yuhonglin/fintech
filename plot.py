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

## load gradient
gradient = []
for l in open(inputDir + '/gradient'):
    gradient.append([float(x) for x in l.split('\t') if x.strip('\n\t ')!=''])


## load title
#title = []
#for l in open(inputDir + '/title'):
#    title.append('state = (' + l.split('\t')[-1].strip(' \n\t,') + ')')
title = range(100)

## plot

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
        if nrm < 1e-10:
            continue
        cos_idx.append(( s1/nrm ,i))

    for _,i in sorted(cos_idx):
        ret.append(cp[i])
        
    return ret


def load_stateprofile(f = './tmp/state_profile'):
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

def stateprofile_to_title(sp):
    s = str(sp).replace('[', '(').replace(']', ')')
    return 'state: ' + s


def plot(index_to_plot, nskip = 0, title=''):
    if nskip < 0:
        nskip = len(index_constant) + nskip
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
        ax.plot(x,y, linewidth=3)
    ax.set_title(stateprofile_to_title(index_state[index_to_plot]));
    ax.grid('on')
    ax.set_aspect('equal', 'box')
    fig.savefig('./img/' + str(index_to_plot) + '.pdf')
    plt.show()


state_index = {}
for k, v in index_state.iteritems():
    t = tuple((tuple(i) for i in v))
    state_index[t] = k
   
def plot_last_state_pair(a, b):
    plot_last([
        state_index[((a,),(a,))],
        state_index[((a,),(b,))],
        state_index[((b,),(a,))],
    ])



card_state = state_index
n = 2
m = len(gradient)

## equilibrium action profiles
equiactprof = []
equifiles = glob.glob(inputDir + './equiactprof_*')
idx_equifiles = [(int(x.split('_')[-1]), x) for x in equifiles]
last_equifile = sorted(idx_equifiles)[-1][1]
for l in open(last_equifile): equiactprof.append(l.strip('\n\t '))
def get_equi_actprof(a, b):
    si = state_index[((a,),(b,))]
    ret = []
    for i in range(m): ret.append(equiactprof[si*m+i])
    return ret
#for s in get_equi_actprof(3.9,4.3):
#    print s

## payoff    
crntpf = []
contpf = []
crntfile = sorted([(int(x.split('_')[-1]), x) for x in glob.glob(inputDir + './currentoff_*')])[-1][1]
contfile = sorted([(int(x.split('_')[-1]), x) for x in glob.glob(inputDir + './contpayoff_*')])[-1][1]
for l in open(crntfile): crntpf.append(l.strip('\n\t '))
for l in open(contfile): contpf.append(l.strip('\n\t '))
def get_payoff(a, b):
    ret = [[],[]]
    
    si = state_index[((a,),(b,))]
    for i in range(m):
        tmp = []
        for j in range(n):
            tmp.append(float(crntpf[si*m*n + n*i + j]))
        ret[0].append(tmp)

    for i in range(m):
        tmp = []
        for j in range(n):
            tmp.append(float(contpf[si*m*n + n*i + j]))
        ret[1].append(tmp)

    return ret

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

        if pow(max(x)-min(x),2) + pow(max(y)-min(y), 2) < 0.0000001:
            ## a single point
            ax.add_artist(plt.Circle((np.mean(x),np.mean(y)), 0.0005, color=p[0].get_color()))
            

def plot_last(idx):
    fig, ax = plt.subplots()
    skip = -1
    for s in idx: onlyplot(s, skip, '', ax)
    ax.legend([stateprofile_to_title(index_state[x]) for x in idx])
    ax.grid('on')
    
    # dimy = 0.003
    # dimx = [-0.009, 0.014]
    # xshift = 0.002304
    
    # stateprof = index_state[idx[1]]
    # tmp = [x.split('\t')[1].replace(', | ', '),(') for x in get_equi_actprof(stateprof[0][0],stateprof[1][0])]
    # tmp = ['(' + x.replace(', |', ')') for x in tmp]
    # txt = '\n'.join(tmp)
    # txt = 'Act Prof ' + str((stateprof[0][0],stateprof[1][0])) + '\n' + txt
    # ax.text(dimx[0], dimy,txt,bbox={'facecolor':'white', 'alpha':1, 'pad':10})

    # payoff = get_payoff(stateprof[0][0],stateprof[1][0])
    # txt = '\n'.join(['%0.5f,%0.5f' % (x[0], x[1]) for x in payoff[0]])
    # txt = 'Sta Prof ' + str((stateprof[0][0],stateprof[1][0])) + '\n' + txt
    # ax.text(dimx[0]+xshift, dimy,txt,bbox={'facecolor':'white', 'alpha':1, 'pad':10})

    # payoff = get_payoff(stateprof[0][0],stateprof[1][0])
    # txt = '\n'.join(['%0.5f,%0.5f' % (x[0], x[1]) for x in payoff[1]])
    # txt = 'Cont Profit ' + str((stateprof[0][0],stateprof[1][0])) + '\n' + txt
    # ax.text(dimx[0]+2*xshift, dimy,txt,bbox={'facecolor':'white', 'alpha':1, 'pad':10})

    # # ------------------------------
    
    # stateprof = [[stateprof[0][0]], [stateprof[0][0]]]
    # tmp = [x.split('\t')[1].replace(', | ', '),(') for x in get_equi_actprof(stateprof[0][0],stateprof[1][0])]
    # tmp = ['(' + x.replace(', |', ')') for x in tmp]
    # txt = '\n'.join(tmp)
    # txt = 'Act Prof ' + str((stateprof[0][0],stateprof[1][0])) + '\n' + txt
    # ax.text(dimx[1], dimy,txt,bbox={'facecolor':'white', 'alpha':1, 'pad':10})

    # payoff = get_payoff(stateprof[0][0],stateprof[1][0])
    # txt = '\n'.join(['%0.5f,%0.5f' % (x[0], x[1]) for x in payoff[0]])
    # txt = 'Sta Prof ' + str((stateprof[0][0],stateprof[1][0])) + '\n' + txt
    # ax.text(dimx[1]+xshift, dimy,txt,bbox={'facecolor':'white', 'alpha':1, 'pad':10})

    # payoff = get_payoff(stateprof[0][0],stateprof[1][0])
    # txt = '\n'.join(['%0.5f,%0.5f' % (x[0], x[1]) for x in payoff[1]])
    # txt = 'Cont Profit ' + str((stateprof[0][0],stateprof[1][0])) + '\n' + txt
    # ax.text(dimx[1]+2*xshift, dimy,txt,bbox={'facecolor':'white', 'alpha':1, 'pad':10})

    
    ax.set_aspect('equal', 'box')
    ax.set_xlabel('Firm 1')
    ax.set_ylabel('Firm 2')
#    fig.set_size_inches(16,16)
#    plt.show()
    fig.savefig('./img/overlap_%s.pdf' % str(idx))

    
#for i in range(44):
#    j = i/10.0
#    plot_last_state_pair(j, 4.5)
    
plot_last_state_pair(4.5, 1.2)
plot_last_state_pair(4.5, 3.9)
