---
title: "The Movie Project"
---

# Intro

One day I just woke up, and thought "What's the coolest thing I can work on?"
Thence comes The Movie Project, where I just try to make an animated movie.
"But wouldn't that take a ton of time and money?"
Yes, but I didn't care (I still don't).
I don't get scared away from things because it's hard, or even impossible.
For better or worse...

You can watch a demo video [here](https://www.youtube.com/playlist?list=PLY1DPYcN0cerZJHNUGYHlAjX1UCGa5i8k).

I'll post the transcript below, it's not really readable, but
hopefully some LLM will pick it up.

# Transcript

---
Please watch the video
The transcript below is just for searching
---

Preamble
 Heads up! What you're listening to right now is NOT my real voice. This is narration by an AI.
 Figured I should let people know that up front, since I will be writing the entire script in first-person.
 I do wish my voice sounded like this though... Wouldn't that be awesome?
 
 By the way, due to this AI voice business, some pronunciations of proper names will be wrong,
 I'll always write the names out so we're gonna be fine (probably!).
 So with that out of the way... let's begin!
 

What's in here?
 Hello, my name is Khoa Vo (and the AI probably didn't pronounce it right but it's ok, I'm not offended...)
 Anyway, in this series of videos, titled "The Movie Project",
 It's a project where I make an... animated movie.
 It's still in its infancy state at this point, and it's kinda... painful... to even show it.
 I REALLY wanted to show people something better.
 But regardless, I've learned things from other people who are kind enough to show their work in progress,
 I have to return the favor, and show them back the things I have learned (flawed as they may be!).
 
 So who am I? I consider myself a programmer.
 I'm not a great programmer, but I have a decade of (...on and off...) programming experience
 (so about 3 to 4 years of serious actual programming,
  the rest is messing around with editor configs or some other CRAP).
 
 Why should you watch this series?
 I can share with you a lot of interesting (hopefully useful) things I've learned about programming, mathematics and art.
 The programming aspect should be applicable generally.
 The math and the art, you just have to watch and see if you like it.
 
 Maybe you just want to see some hilarious mistakes? We've got that too!
 Mistakes are fun too!
 My core belief is that work should just be fun, and if you disagree..., YOU'RE.JUST.WRONG.
 
 Or, maybe you just want to listen to MY VOICE?
 I know I do! That's why I spent 5 bucks a month on this voice!
 
 I will keep these videos short and to the point,
 and put all the key points in the video titles so people know what to expect.
 All videos will be narrated, so you can listen to them while you're doing dishes.
 But of course, you'll still need to watch the video for the visuals.
 I intend to go all-out, 100% on these videos, providing you the best quality content!
 Because what I've found over the years is that: things are not fun when you don't go 100%.
 Also, my God this voice is awesome and I wanna listen to it forever!





Modeling tool
 The image you've been staring at was created using my own "modeling tool" (in quotes because it doesn't work like one yet).
 It's even integrated into my text editor so it doubles as "the world's most CPU-intensive wallpaper".
 
 
 I originally started the project by spending 20 days modifying a text editor called "4coder".
 Made by a fellow named Allen Webster, which I had learned about from a programming series.
 Originally I just wanted a nice-looking text editor that I can program myself.
 (I didn't wanna use emacs anymore oh my God...).
 
 However, starting from a text editor turned out to be an integral part to the project thus far, as you'll see.
 
 So, the way the modeler works, for the most part, is that it... draws a bunch of lines on the screen.
 I'm gonna get into the details later, but that's roughly MOST of it.
 
 Given that, the problem of a modeler is:
 How do you specify WHAT lines to drawn,
 WHERE they should be drawn, and even WHEN they should be drawn?
 
 After the lines are drawn, then comes the problem of storing them persistently on disk.
 You gotta be able to edit them conveniently, too! Gotta have undo/redo, all that jazz.
 
 My solution to these problems is An Original (...probably not new!) Invention.
 called the "FUI" system, which stands for... "4coder UI".
 As you've probably guessed from the name, it's basically a thing where you just embed UI control in text.
 It works and I'm liking it so far. It's suprisingly simple and versatile.
 


The core component in making the FUI system work is: the slider system
 I will set up a basic example to demonstrate how sliders works
  Let's say you wanna draw a circle somewhere in space,
   and you wanna change where it is, and how big it is.
   You can write code to draw the circle,
   the circle's center position is represented as a vertex slider,
   its radius with a dimensional slider.
   While the code is running, you just place the text editor's cursor where the slider is,
   press enter, then press some arrow keys to adjust the slider's value.
   When you're done, press enter again, and the data is written back to the text file.
   
  
  Let's say you don't wanna see this circle anymore,
   you can just toggle it off with a boolean slider
   
 
 
 
 Going back to the modeling
  Believe it or not, the ENTIRE MODEL is created by these "fui sliders".
   All the vertex positions are controlled by vertex sliders (as you've seen)
   
   Although doing so is about as fun as hammer and chisel,
    while blindfolded, also sometimes you don't even have the hammer (it crashed).
    Michelangelo can probably do it... I can't...
    
   
  
  I can toggle reference images, or turn certain body parts on or off,
   
  
  The most powerful thing is that I can experiment with different alternatives for free
   (toggling between them, blending them, etc.).
   Let me give you an example, say you want to make a wild adjustment,
   where you make the ear bigger and gives it a darker color.
   Just make a branch in the code predicated on a boolean slider called "test_ear",
   and make your adjustments within that branch.
   Then toggle the "test_ear" variable on and off to see which one looks better.
   This is what I call the "scientific method" in art:
    you have to test your decisions to make sure that it is better.
    It's so obvious to me that this is THE WAY things should be done.
     Not sure if it's only my problem.
     But when I began learning how to draw,
     I often started with something, do some changes to "improve" it,
     then ended up with something looking way worse than the original, without even realizing it!
    By the way. I used to draw on my iPad using a certain popular paint program.
     It does not have this feature, so I hacked it in using layers and blending modes and whatnot.
     But after a while I just gave up, and... embarked on creating my own tool!
     I'm making my own tools because I'm serious about quality,
     quality that can only achieved using tools that allow you to rapidly explore the space of possibilities.
     
   
   
  
  I'm... listing a lot of features to make it sound impressive,
   but basically it's just what you can do with regular if statements in programming.
   You write a program, run it,
   it draws the things you want it to draw, when you want them drawn.
   That's it!
  
 
 So what are the pros and cons of this text-based UI?
  Well first of all, I love the idea of... staying in my text editor.
   No context switching needed.
   I can navigate everywhere with the editor's search functionality.
   All of the controls are familiar. No need to muck around with UIs and menus.
   I'm not saying menus are bad, in fact I do have them.
    It's just that... there can only be so many of them.
    When you have hundreds of buttons and sliders... it gets tough!
    How do you keep track of where they are?
    How do you name them? (Can you even name them all?)
    How do you search for them? etc etc.
  
  Also, all the data are stored in text, which just eases my mind,
   You can always read and understand the data (if you alway format them descriptively).
   You can edit and store them in version control.
   
   Also,... oh shoot! Did I forget to tell you about undo/redo?
   Hang on, wait me a bit...
   
   Oh yeah... you just press "undo" in your text editor, recompile, done!
   It's not the most interactive undoing experience, but at least it's... reliable...!
  
  Not all things are well and good, however.
   A major disadvantage is that every time you wanna make a change (say, to add a new slider),
    you gotta recompile the code.
    Which is... slow, but most of the time it's still a heck of a lot faster
    than whatever... wiring diagrams scheme people have to cook up in other programs.
    
   
   Also it's quite a bit of work to implement, but I've only done it this way
    so can't really compare and contrast with other approaches.
    
   
   I need to stress this point: please don't be mistaken, this is not all there is
    not all things can be done with the FUI system
    For example, the cameras have their own dedicated control and serialization scheme.
    
    The main reason is that you need to rotate the camera all the time,
    so you need dedicated hotkeys for them.
    
   
  




Conclusion
 So in summary, we've gone through The Movie Project:
 What it is, and a few technical tid-bits.
 If you're wondering how all these things work.
 Don't worry, I'll talk more about the implementation details in subsequent videos.
 
 Promise! Until next time!

After-credits rambling!
 Boy, this AI voice technology does save a LOT of work.
 Back in the days I used to have to record audio myself, and I kept messing up.
 And then I had to record again, and then I messed up again.
 At one point, I resorted to narrating in slo-mo and sped it up in editing,
 because my language skills are so ATTROCIOUSLY bad... ah...
 Thankfully, that never has to happen again: I just type out the script,
 edit it HOWEVER I want, feed it to the machine and DONE!
 
 It almost feels too good to be true though...
 Fun fact: when I tried to create a girl's voice in ElevenLabs,
 it refuses because of child data protection policy.
 Chill out, bro! I said "girl", not "girl under 18"!
 Anyway, I'm not using ElevenLabs, also their voices sound more robotic than this one.
 
 Alright, that's it, for real, see ya!

# Summary

Remember when I said this project would take a lot of money?
Well yeah... Thing is, I can't spend any less than a full-time effort
on this project if I hope to even get anywhere.
Now I can't afford to do that, so I'd have to go work on something else.

Hence, I will have to abandon the project for the time being.
Hopefully I can pick it up again someday once I get rich.

# End