import pickle
import os
import sys
import glob
import copy
## Two dictionaries of data are needed
## 1. state_profile                 : [tangent points]
## 2. state_profile, action_profile : next_state_profile
## For a targent point, profit_profile and action_profile is needed
## And the tangent points should be ordered according to the degrees of
## the tangent lines from 0 degree.

inputDir = "./tmp/"

n = 2

beta = 0.8

accuracy = 5

####### load stateProfile_tangentPoint

class TangentPoint(object):
    action_profile = None
    profit_profile = None
    cont_profit    = None
    index          = None
    def set_action(self, a):
        self.action_profile = a
        return self
    def set_profit(self, p):
        self.profit_profile = p
        return self
    def set_contprofit(self, p):
        self.cont_profit = p
        return self
    def set_index(self, i):
        self.index = i
        return self
    def __repr__(self):
        return 'Index: %s; Action Profile: %s; Profit Profile: %s; Cont Profit: %s' % (str(self.index), str(self.action_profile), str(self.profit_profile), str(self.cont_profit))
    def __eq__(self, other):
        if isinstance(other, TangentPoint):
            return self.action_profile==other.action_profile and self.profit_profile==other.profit_profile and self.cont_profit==other.cont_profit and self.index==other.index
        else:
            return False
    def __ne__(self, other):
        return (not self.__eq__(other))
    def __hash__(self):
        return hash(self.__repr__())
    

def is_profit_action_equivalent(tp1, tp2):
    return tp1.action_profile==tp2.action_profile and tp1.profit_profile==tp2.profit_profile and tp1.cont_profit==tp2.cont_profit

    
###### Some helper functions to figure out the tangent points

## load gradient
gradient = []
for l in open(inputDir + '/gradient'):
    gradient.append([round(float(x), accuracy) + 0 for x in l.split('\t') if x.strip('\n\t ')!=''])

m = len(gradient)
    
## load the W matrices
index_constant = []
for f in glob.glob(inputDir + '/' + 'data_*'):
    index = int(f.split('/')[-1].split('_')[-1])
    constant = []
    for firstline in open(f):
        constant.append([round(float(x), accuracy) + 0 for x in firstline.split('\t') if x.strip('\n\t ')!=''])
    index_constant.append((index, constant))

## load state profile
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
            s.append([round(float(x),accuracy) + 0 for x in tx.split(',')])
        ret[int(i)] = s
    return ret

index_state = load_stateprofile()

state_index = {}
for k, v in index_state.iteritems():
    t = tuple((tuple(i) for i in v))
    state_index[t] = k


## load equilibrium action profiles of last output
equiactprof = []
equifiles = glob.glob(inputDir + './equiactprof_*')
idx_equifiles = [(int(x.split('_')[-1]), x) for x in equifiles]
last_equifile = sorted(idx_equifiles)[-1][1]
for l in open(last_equifile): equiactprof.append(l.strip('\n\t '))
def get_equi_actprof(a, b):
    si = state_index[((a,),(b,))]
    tmp = []
    for i in range(m): tmp.append(equiactprof[si*m+i])
    tmp = [x.split('\t')[1].replace(', | ', ',') for x in tmp]
    tmp = [x.replace(', |', '').split(',') for i,x in enumerate(tmp)]

    nAct = len(tmp[0])/2
    ret = []
    for i in range(m):
        ret.append([[],[]])
        for j in range(len(tmp[0])):
            if j < nAct:
            ## first firm
                ret[-1][0].append(int(tmp[i][j]))
            else: 
                ret[-1][1].append(int(tmp[i][j]))
    return ret

## load profits
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
            tmp.append(round(float(crntpf[si*m*n + n*i + j]),accuracy) + 0)
        ret[0].append(tmp)

    for i in range(m):
        tmp = []
        for j in range(n):
            tmp.append(round(float(contpf[si*m*n + n*i + j]),accuracy) + 0)
        ret[1].append(tmp)

    return ret


### now figure out stateIndex_tangentPoint
stateIndex_tangentPoint = {}
for si, s in index_state.iteritems():
    stateIndex_tangentPoint[si] = []
    
    statepayoffs, contpayoffs = get_payoff(s[0][0],s[1][0])
    actprofs = get_equi_actprof(s[0][0],s[1][0])
    
    for i in range(len(statepayoffs)):
        tp = TangentPoint()
        tp.set_profit((statepayoffs[i][0],statepayoffs[i][1]))
        tp.set_action(actprofs[i])
        tp.set_contprofit((contpayoffs[i][0],contpayoffs[i][1]))
        tp.set_index(i)
        stateIndex_tangentPoint[si].append(tp)
        
        
### now load dynamics: (stateIndex, actionIndex) : next_state_index
stateIndexActionIndex_nextStateIndex = {}
stateIndexActionprof_nextStateIndex = {}
for l in open(inputDir + '/dynamic'):
    tmp = l.strip('\n\t ').split('@')
    stateIndex = int(tmp[0].split(')')[0].strip(' \t('))
    actionIndex = int(tmp[1].split(')')[0].strip(' \t('))
    nextStateIndex = int(tmp[2].split(')')[0].strip(' \t('))    
    stateIndexActionIndex_nextStateIndex[(stateIndex, actionIndex)] = nextStateIndex

    tmp = tmp[1].strip('|\n\t ').split('\t')[1].split('|')
    ap1 = tuple([int(x) for x in tmp[0].strip(', ').split(',')])
    ap2 = tuple([int(x) for x in tmp[1].strip(', ').split(',')])
    stateIndexActionprof_nextStateIndex[(stateIndex,(ap1,ap2))] = nextStateIndex

################ trace it
# TODO: Need another data structure: 
def trace_one(stateprof, directIndex, eps=1e-8):
    a,b = stateprof
    stateIndex = state_index[((a,),(b,))]
    tangentPoint = stateIndex_tangentPoint[stateIndex][directIndex]
    tmp = tuple([tuple(x) for x in tangentPoint.action_profile])
    nextStateIndex = stateIndexActionprof_nextStateIndex[(stateIndex,tmp)]
    x = tangentPoint.cont_profit[0]
    y = tangentPoint.cont_profit[1]
    ret = []
    for tp in stateIndex_tangentPoint[nextStateIndex]:
        x1 = tp.profit_profile[0]*(1-beta) + tp.cont_profit[0]*beta
        y1 = tp.profit_profile[1]*(1-beta) + tp.cont_profit[1]*beta
        if pow(x-x1,2) + pow(y-y1,2) < eps:
            ret.append(tp)
        #ret.append((pow(x-x1,2) + pow(y-y1,2), tp))
    if len(ret) == 0:
        ret = copy.deepcopy(stateIndex_tangentPoint[nextStateIndex])
    return (nextStateIndex,ret)




def trace_n_profit(stateprof, directIndex, n, eps=1e-8):
    ret = []
    def _trace(profit_history, stateIndex_history, maxLevel):
        if len(profit_history) == maxLevel:
            ret.append(tuple(profit_history))
            return
        sp = tuple(x[0] for x in index_state[stateIndex_history[-1]])
        nextStateIndex, tangentPoints = trace_one(sp, directIndex, eps)
        for i in range(len(tangentPoints)): tangentPoints[i].index = None
        tangentPoints = set(tangentPoints)
        for tp in tangentPoints:
            ph = copy.deepcopy(profit_history)
            ph.append((tp.profit_profile, tp.cont_profit))
            sih = copy.deepcopy(stateIndex_history)
            sih.append(stateIndexActionprof_nextStateIndex[(stateIndex_history[-1], tuple(tuple(x) for x in tp.action_profile))])
            _trace(ph, sih, maxLevel)
    a,b = stateprof
    tmp = get_payoff(a,b)
    _trace([(tuple(tmp[0][directIndex]), tuple(tmp[1][directIndex]))], [state_index[((a,),(b,))]], n)
    return ret


def trace_n_all(stateprof, directIndex, n, eps=1e-8):
    ret = []
    def _trace(history, stateIndex_history, maxLevel):
        if len(history) == maxLevel:
            ret.append((history, stateIndex_history))
            return
        sp = tuple(x[0] for x in index_state[stateIndex_history[-1]])
        nextStateIndex, tangentPoints = trace_one(sp, directIndex, eps)
        for i in range(len(tangentPoints)): tangentPoints[i].index = None
        tangentPoints = set(tangentPoints)
        for tp in tangentPoints:
            ph = copy.deepcopy(history)
            ph.append(tp)
            sih = copy.deepcopy(stateIndex_history)
            sih.append(stateIndexActionprof_nextStateIndex[(stateIndex_history[-1], tuple(tuple(x) for x in tp.action_profile))])
            _trace(ph, sih, maxLevel)
    a,b = stateprof
    _trace([stateIndex_tangentPoint[state_index[((a,),(b,))]][directIndex]], [state_index[((a,),(b,))]], n)
    return ret


def generate_trace_data(stateprof, m, n, eps=1e-8):
    ret = {}
    for directIndex in range(m):
        ret[directIndex] = {'firm1': {}, 'firm2': {}}
        tps = trace_n_all(stateprof, directIndex, n, eps)
        ret[directIndex]['firm1']['clientcapital'] = [[y.action_profile[0][0] for y in x[0]] for x in tps]
        ret[directIndex]['firm1']['quality'] = [[y.action_profile[0][1] for y in x[0]] for x in tps]
        ret[directIndex]['firm1']['price'] = [[y.action_profile[0][2] for y in x[0]] for x in tps]
        ret[directIndex]['firm2']['clientcapital'] = [[y.action_profile[1][0] for y in x[0]] for x in tps]
        ret[directIndex]['firm2']['quality'] = [[y.action_profile[1][1] for y in x[0]] for x in tps]
        ret[directIndex]['firm2']['price'] = [[y.action_profile[1][2] for y in x[0]] for x in tps]
        
        ret[directIndex]['firm1']['stateprofit'] = [[y.profit_profile[0] for y in x[0]] for x in tps]
        ret[directIndex]['firm2']['stateprofit'] = [[y.profit_profile[1] for y in x[0]] for x in tps]
        
        ret[directIndex]['firm1']['contprofit'] = [[y.cont_profit[0] for y in x[0]] for x in tps]
        ret[directIndex]['firm2']['contprofit'] = [[y.cont_profit[1] for y in x[0]] for x in tps]
        ret[directIndex]['stateindex'] = [x[1] for x in tps]
    return ret


