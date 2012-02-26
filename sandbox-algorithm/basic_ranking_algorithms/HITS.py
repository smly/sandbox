import numpy as np
import doctest

class HITS():
    def __init__(self, adj_matrix):
        self._adj = adj_matrix

    def calc(self):
        """ INPUT:  adjacency matrix (numpy.matrix),
            OUTPUT: hub and authority vector

            >>> import HITS
            >>> import numpy as np
            >>> a = np.array([[0,0,1,1],[0,0,1,1],[1,1,0,0],[0,1,1,0]])
            >>> hits_calculator = HITS.HITS(a)
            >>> (auth,hub) = hits_calculator.calc()
            >>> auth
            array([ 0.04659837,  0.17770786,  0.45340163,  0.32229214])
            >>> hub
            array([ 0.32229214,  0.32229214,  0.09319675,  0.26221898])
        """
        cit_matrix = np.dot(self._adj.T, self._adj)
        bib_matrix = np.dot(self._adj, self._adj.T)
        e_val,e_vec = np.linalg.eig( cit_matrix )
        auth = sorted( zip( e_val, e_vec.T ), lambda x,y : cmp(y[0], x[0]) )
        e_val,e_vec = np.linalg.eig( bib_matrix )
        hub = sorted( zip( e_val, e_vec.T ), lambda x,y : cmp(y[0], x[0]) )
        return (auth[0][1] / sum(auth[0][1]),
                hub[0][1] / sum(hub[0][1]))

if __name__ == "__main__":
    doctest.testmod()
