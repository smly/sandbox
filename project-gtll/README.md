# gtll -- graph-based transductive learning library

(not completed yet)

Several semi-supervised learning methods have been developed in recent years.

gtll particularly provieds graph-based transductive methods
included in Gaussian Field and Harmonic Function (GFHF)
and Local and Global Consistency(LGC) for semi-supervised setting.

In addition, this library also provides graph sparsify methods and similarity measures.

# data format

    <class label> [feature_id:weight]
    ...

An unlabeled data is indicated by 1 charactor (not digit).

'-1' is ok to indicate an unlabeled data.

feature_id must be 1-indexed number.

# sample data

    1 1:0.1 4:0.1 6:0.12
    1 1:0.1 5:0.4 6:0.2
    . 1:0.1 4:0.12 6:0.1
    2 2:0.1 5:0.1 6:0.1
    . 2:0.1 4:0.1 5:0.1

or

    1 1:0.1 4:0.1 6:0.12
    1 1:0.1 5:0.4 6:0.2
    -1 1:0.1 4:0.12 6:0.1
    2 2:0.1 5:0.1 6:0.1
    -1 2:0.1 4:0.1 5:0.1

In the example above is described that:

Data x_1 and x_2 are labeled by [1].

Data x_4 is labeled by [2].

Data x_3 and x_5 are unlabeled.

# acknowledgments

This project includes following softwares:

- waf: http://code.google.com/p/waf/
- Yet Another Waf Unittest http://github.com/tanakh/waf-unittest
