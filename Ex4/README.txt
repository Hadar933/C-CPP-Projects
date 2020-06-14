#PROJECT 4 (C++) - Neural networks, classes, operators overloading...
-------------------------------------------------------------------------------

###created by:
Hadar Sharvit

-------------------------------------------------------------------------------

###Institution:
the Hebrew university of Jerusalem

-------------------------------------------------------------------------------

###Description:

in this project i've built a neural network that takes as inputs a
28 * 28 binary image representing some number between 0 to 9,
and outputs a "guess" for the number shown in the image.

the network guesses correctly 96% of the time.

the project is loosely base on this video by 3Blue1Brown - check it out
(https://www.youtube.com/watch?v=aircAruvnKk&t=107s)

-------------------------------------------------------------------------------

###Try it yourself:

####using an interpreter(ex. Clion):
  in edit configurations, add the following input: w1 w2 w3 w4 b1 b2 b3 b4
  and run.
  *w_i represents a weight matrix and b_i represents a bias vector

####from command line prompt:
  comline the program using the supplied make file by typing to shell:
  $ make mlpnetwork
  run the code using:
  $ ./mlpnetwork w1 w2 w3 w4 b1 b2 b3 b4

####while the program runs:
  you will be asked to provide a binary photo
  we have provided several options (im0,im1,...im9). so just type im the name of
  the photo you wish to run on.
  press q to finish
  
  -------------------------------------------------------------------------------
###example:
  
Please insert image path:
im2
Image processed:
                                                        
                                                        
                                                        
                                                        
                                                        
                                        ******          
        ****                            ******          
        ****                            ******          
        ****                          ******            
        ****                          ******            
      ******                          ******            
      ******                          ******            
      ******                        ******              
      ******                    **********              
      ******      ************************              
      ****************************  ****                
        ****************          ******                
                                  ******                
                                  ******                
                                  ******                
                                  ******                
                                  ******                
                                  ******                
                                  ******                
                                  ******                
                                                        
                                                        
                                                        

Mlp result: 4 at probability: 0.996058
