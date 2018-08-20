import glob

inputDir = "./tmp/"

beta = .8

n = 2
## load gradient
gradient = []
for l in open(inputDir + '/gradient'):
    gradient.append([float(x) for x in l.split('\t') if x.strip('\n\t ')!=''])

m = len(gradient)

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


def get_tangent_point_info(a,b):
    a = a[0]; b = b[0];
    crnt, cont = get_payoff(a,b)
    ap = get_equi_actprof(a,b)
    ret = {'x': [],
           'y': [],
           'firm1': {
               'curr_profit': [],
               'cont_profit': [],
               'action_profile': []
           },
           'firm2': {
               'curr_profit': [],
               'cont_profit': [],
               'action_profile': []
           }
    }

    for j in range(len(crnt)):
        ret['x'].append(crnt[j][0]*(1-beta) + cont[j][0]*beta)
        ret['y'].append(crnt[j][1]*(1-beta) + cont[j][1]*beta)
        
    for i in range(m):
        ret['firm1']['curr_profit'].append(crnt[i][0])
        ret['firm2']['curr_profit'].append(crnt[i][1])
        ret['firm1']['cont_profit'].append(cont[i][0])
        ret['firm2']['cont_profit'].append(cont[i][1])
        ret['firm1']['action_profile'].append(ap[i][0])
        ret['firm2']['action_profile'].append(ap[i][1])

    return ret


## second, load action profile
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
