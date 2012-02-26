import numpy as np
import doctest

class SubspaceHITS():
    def __init__(self, adj_matrix):
        self._adj = adj_matrix

    def calc(self):
        """ INPUT:  adjacency matrix (numpy.matrix),
            OUTPUT: hub and authority vector

            >>> import SubspaceHITS as SHITS
            >>> import numpy as np
            >>> a = np.array([[0,0,1,1],[0,0,1,1],[1,1,0,0],[0,1,1,0]])
            >>> hits_calculator = SHITS.SubspaceHITS(a)
            >>> (auth,hub) = hits_calculator.calc()
            >>> auth
	    array([ 0.06666667,  0.2       ,  0.46666667,  0.26666667])
            >>> hub
            array([ 0.3       ,  0.3       ,  0.16666667,  0.23333333])
        """
        cit_matrix = np.dot(self._adj.T, self._adj)
        bib_matrix = np.dot(self._adj, self._adj.T)
        e_val,e_vec = np.linalg.eig( cit_matrix )
        auth = sorted( zip( e_val, e_vec.T ), lambda x,y : cmp(y[0], x[0]) )
        subspace_auth = []
        for j in range(len(auth)):
            xj = 0
            for each_auth in auth:
                eignval = each_auth[0]
                eignvec = each_auth[1]
                basis = np.zeros(len(auth))
                basis[j] = 1
                temp = np.dot(eignvec, basis.T)
                xj += eignval * eignval * temp * temp
            subspace_auth.append(xj)
        auth = subspace_auth / sum(subspace_auth)
        e_val,e_vec = np.linalg.eig( bib_matrix )
        hub = sorted( zip( e_val, e_vec.T ), lambda x,y : cmp(y[0], x[0]) )
        subspace_hub = []
        for j in range(len(hub)):
            xj = 0
            for each_hub in hub:
                eignval = each_hub[0]
                eignvec = each_hub[1]
                basis = np.zeros(len(hub))
                basis[j] = 1
                temp = np.dot(eignvec, basis.T)
                xj += eignval * eignval * temp * temp
            subspace_hub.append(xj)
        hub = subspace_hub / sum(subspace_hub)
        return (auth, hub)

if __name__ == "__main__":
    doctest.testmod()
