import os
import glob
for f in glob.glob('tmp/*'):
    a = f.split('.')[0].split('_')[-1]
    try:
        a = int(a)
    except:
        continue
    if a > 84:
        print f
        os.system('rm '+f)
