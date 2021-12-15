Owen Sullivan

My project works very similarily to how lab 7 does. I used a lot of boilerplate
code from lab 7 because it had all the aspects we needed shading, textures, lighting, view transforms etc.
The most relevant changes to the code are in Application, Models, and Materials. In Application, I wrote a system to 
only create the model once, then edit it and re draw it as many times onto the canvas. 
This was useful for the Cube that I used as the lemon bars, Napkin, and upholder on the table. 
I also used this aspect for the lemons to draw multiple spheres. I textured my mug, the tea/coffee in it, 
and the wood grain table. I altered the lighting and camera positions to create a studio lit zoomed in effect. 
This is done in Lighting and Viewing. I re-used most of my texture and shader code from previous labs for the 
shading and texturing. 

In application, I load all the models and then apply different transformations on the models multiple times
to create the image we were tasked to re-create. More implementation details can be found in sources.

I did not implement any command line arguments or animations due to many other final projects I have been working on. 
I did some research into some cool shadow and fabric and steam stuff, but I didnt get to finish any of the implemtnations.

Note: I had some segfault issues in the middle of my development, since then I havent had any. I am not sure what fixed it... 
but if it seg faults, re run it a few times. It usually succesfully runs about 75% of the time (when I was having these issues). Now, it has been 100% working.