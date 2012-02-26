# huffman tree

view code table for (blocked) huffman encode

## compile

    g++ main.cpp huffman_tree.cpp

## how to run

    $ ./a.out < huffman_sample
    -----------------------
    B       0.2     00
    AAB     0.028   0100
    AAAB    0.11    0101
    AB      0.15    011
    AAAA    0.512   1
    [Ave] 0.900333
    -----------------------
    ...
    [Ave] 0.742033
    -----------------------
    ...
    [Ave] 0.683868


    $ ./a.out < huffman_sample2
    -----------------------
    B       0.1     0
    A       0.9     1
    [Ave] 1
    -----------------------
    AB      0.09    00
    BB      0.01    010
    BA      0.09    011
    AA      0.81    1
    [Ave] 0.645
    -----------------------
    BBB     0.001   00000
    BAB     0.009   00001
    ABB     0.009   00010
    BBA     0.009   00011
    ABA     0.081   001
    AAB     0.081   010
    BAA     0.081   011
    AAA     0.729   1
    [Ave] 0.532667
