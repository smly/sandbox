import numpy as np
import doctest

class PageRank():
    def __init__(self, adj_matrix):
        self._EPS = 1.0e-10
        self._cnt = 0
        self._err = 0
        self._dim = adj_matrix[0].size
        self._P = np.zeros( (self._dim, self._dim), np.float64 )
        for i in range(self._dim):
            self._P[i,:] = adj_matrix[i] / (sum(adj_matrix[i]) * 1.0)

    def calc(self, curr, alpha):
        """ INPUT:  adjacency matrix,
            OUTPUT: object-ranking vector

            >>> import PageRank as PR
            >>> import numpy as np
            >>> a = np.array([[0,0,1,1],[0,0,1,1],[1,1,0,0],[0,1,1,0]])
            >>> init = np.array([1/4., 1/4., 1/4., 1/4.], np.float64)
            >>> pr_calculator = PR.PageRank(a)
            >>> pr_calculator.calc(init, 1.0)
            array([ 0.16666667,  0.27777778,  0.33333333,  0.22222222])
        """
        self._cnt = 0
        while 1:
            self._cnt += 1
            prev = curr
            curr = alpha * np.dot((self._P).T, prev)
            curr += (1. - alpha) / (self._dim * 1.)
            self._err = self.diff(prev, curr)
            if (self._err < self._EPS): break
        return curr

    def diff(self, prev, curr):
        err = 0
        for i in range(self._dim):
            err += np.absolute(prev[i] - curr[i])
        return err

if __name__ == "__main__":
    doctest.testmod()
