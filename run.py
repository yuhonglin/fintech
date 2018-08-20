
## corner points
exec(open('./cornerpoint.py'))
## tangent points
exec(open('./tangentPoint.py'))



#######################################
## direct_traceData
## stateProfileIndex_cornerPoints
stateProfileIndex_cornerPoints = {}
for i, v in index_state.iteritems():
    a, b = v
    x, y = get_corner_xy((a[0],b[0]))
    stateProfileIndex_cornerPoints[i] = {'x': x, 'y': y}

##
stateProfileIndex_tangentPoints = {}
for i, v in index_state.iteritems():
    a, b = v
    stateProfileIndex_tangentPoints[i] = get_tangent_point_info(a,b)


## stateProfileIndex_direction_nextStateProfileIndex
stateProfileIndex_direction_nextStateProfileIndex = {}
for i, v in index_state.iteritems():
    a, b = v
    actprof = get_equi_actprof(a[0],b[0])
    stateProfileIndex_direction_nextStateProfileIndex[i] = {}
    for direct in range(m):
        ap = tuple((tuple(x) for x in actprof[direct]))
        nextStateIndex = stateIndexActionprof_nextStateIndex[(i,ap)]
        stateProfileIndex_direction_nextStateProfileIndex[i][direct] = nextStateIndex

################
txt = open('./traceviewer.html').read()
txt = txt.replace('###', str(index_state))
txt = txt.replace('!!!', str(len(gradient)))
txt = txt.replace('&&&', str(beta))
txt = txt.replace('@@@', str({k:str(v).replace('[','').replace(']','') for k,v in index_state.iteritems()}))
txt = txt.replace('%%%', str(stateProfileIndex_direction_nextStateProfileIndex))
txt = txt.replace('^^^', str(stateProfileIndex_tangentPoints))
txt = txt.replace('___', str(stateProfileIndex_cornerPoints))

## code
import re
import html
def comment_remover(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'):
            return " " # note: a space and not an empty string
        else:
            return s
    pattern = re.compile(
        r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE
    )
    return re.sub(pattern, replacer, text)

def remove_emptyline(txt):
    tmp = txt.split('\n')
    ret = ''
    for l in tmp:
        if l.strip('\n\t ') == '':
            continue
        else:
            ret += '\n' + l
    return ret

from pygments import highlight
from pygments.lexers import CppLexer
from pygments.formatters import HtmlFormatter

txt = txt.replace('~~~', remove_emptyline(highlight(comment_remover(open('./FinTech.cpp').read()), CppLexer(), HtmlFormatter()).encode('utf-8')).replace('\n','\\n').replace('\"', '\''))
txt = txt.replace('===', remove_emptyline(highlight(comment_remover(open('./main.cpp').read()), CppLexer(), HtmlFormatter()).encode('utf-8')).replace('\n', '\\n').replace('\"', '\''))

txt = txt.replace('***', open('./friendly.css').read().replace('\n', '\\n'))

open('./a.html', 'w').write(txt)
