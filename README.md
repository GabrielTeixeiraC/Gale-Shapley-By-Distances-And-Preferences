# Matching-By-Distances-And-Preferences

This program receives a grid with bikes, visitors and obstacles and finds the optimal matching of bikes and visitors according to the distance of the bikes to the visitors and the visitors preferences. 
To do this, the BFS and Gale-Shapley algorithms were used. The inputs follow the format:

    <Number of visitors and bikes (n)>
    <Number of lines> <Number of columns>
    <Grid, where numbers represent bikes, letters represent visitors, '-' represents obstacles and '*' represents paths>
    <Weights of first visitor's preferences>
    <Weights of second visitor's preferences>
    ...
    <Weights of n-th visitor's preferences>
## Example 1:

### Input: 
    2
    4 4
    a**1
    --**
    *-**
    b*0*
    3 4
    4 5

### Output
    a 1 // <Visitor ’a’ matched with bike 1>
    b 0 // <Visitor ’b’ mathced with bike 0>
    
## Example 2:

### Input: 
    5
    4 5
    c*d3a
    --1*2
    0-*b*
    ***4e
    1 2 4 5 3
    4 3 5 1 2
    5 4 3 2 1
    3 4 2 5 1
    1 4 5 2 3


### Output
    a 3 // <Visitor ’a’ matched with bike 3>
    b 2 // <Visitor ’b’ matched with bike 2>
    c 0 // <Visitor ’c’ matched with bike 0>
    d 1 // <Visitor ’d’ matched with bike 1>
    e 4 // <Visitor ’e’ matched with bike 4>
