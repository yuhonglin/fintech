

tmp = sorted(index_constant)[-1]
W = tmp[1][460]

X = [x/1000. for x in range(-100, 100)]

fig, ax = plt.subplots()

for i, g in enumerate(gradient):

    #if i % 3 !=0:
    #continue
    
    cnst = W[i]
    print g[0], '*x + ', g[1], '*y <=', cnst, ','
    if g[0] == 0:
        ax.plot([-1, 1], [cnst/g[1], cnst/g[1]])
    elif g[1] == 0:
        ax.plot([cnst/g[0], cnst/g[0]], [-1, 1])
    else:
        Y = []
        for x in X:
            Y.append((cnst - g[0]*x)/g[1])
        ax.plot(X,Y)

plt.show()

