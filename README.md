# Visual Analytics to Support Illustrative Visualization

Art-inspired illustrative visualization for volume rendering, where optimal renderings and user-steerable real-time changes are incorporated. 

### Useful for which scenario: 
1. Inspection of outcomes
2. Generate lots of data for designing style transferring solutions, learn aesthetics, …

### How to (step-by-step): 

1) Read the following papers: 
    - https://onlinelibrary.wiley.com/doi/full/10.1111/cgf.13452
    - https://onlinelibrary.wiley.com/doi/epdf/10.1111/cgf.13322
    - https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=8807288

2) Pick 2-3 illustrative visualization approaches (they should be view-dependent or illumination-dependent, at least!), examples: 
    - Silhouettes
    - Laplacian lines 
    - Suggestive contours
    - Ridges
        
##### The idea here is that these lines change with the view or with the illumination scheme. Start with one approach, then the rest should be easy. I use as an example below suggestive contours (also, wherever you find github implementations use them!)

3) Compute the suggestive contours for a high number of views and different illumination schemes 
4) Select a number of features (see section 3.1 paper [1])
5) Implement a viewer for the features and for the scene (we use some medical data, e.g., a brain would be nice)
6) Check if we need to implement a dimensionality reduction/clustering approach on top of that to find significant patterns in the data
7) Propose an optimal or three top renderings to the user automatically based on the views above
8) User can alter the rendering to their taste (user-steerable rendering: real-time rendering of the outcomes)
9) Capture the alterations in a provenance graph (see paper [3])
10) Use this information as input to the next investigation (preferences integrated to the next task)
