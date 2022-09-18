---
title: "Learning experience with Handmade Hero"
---

I have been following along with the [Handmade Hero](https://handmadehero.org/)
project: the show where the host, Casey Muratori, makes a complete game engine
from scratch without using even the C runtime library. It started mainly as a
passion self-study project at first, but now getting more "serieous" as now I'm
planning to switch into game programming for real. It occupied most of my free
time besides my dayjob at Hatch Entertainment.

# Why did I start doing this?

The reason why I started was because I'm fascinated by what Casey is able to do
on the show. With only one hour a night, he's able to accomplish more than what
I can do in days or even weeks (after learning how the code is supposed to
work), while explaining in painstaking details everything he does. My job as a
cloud engineer also involves a lot of programming and this exercise has greatly
improved my work.

The second reason was that Hatch (the company I worked for) is a game
company. We don't make games, and my job didn't necessarily involve any game
technology knowledge. However, I still think it's beneficial to gain a basic
understanding of how games work in general, to help in playtesting our product.

The third reason was because I feel it's important to know how to make fast,
reliable software in general. (UPDATE: There used to be a pointless rant here,
but I've removed it for brevity).

# Cool demos from the game

Unfortunately, I can't publicize any of the game code due to copyright reasons
(which is *not cool* since I spent so much time writing them myself). However, I
can show you some stuff we have in the game so far that I think is cool:

## General description

The game is a top-down adventure game (like e.g the original Zelda game). I'm
still unclear about what would be in the game, since right now I'm still working
on the engine (which is 50% done now).

## Infinitely large, procedurally-generated world

The world is where most of the work goes into. I've barely touched the
procedural generation part, but the infrastructure to handle scale is already
there.

<iframe src="https://drive.google.com/file/d/1RbuPJzrlCBVwvVeoFHcDH3Vuaa3IpR_p/view?usp=sharing" frameborder="0" allowfullscreen></iframe>

The scale of the world introduces precision problems if you only a single set of
floating-point coordinates. To avoid this problem, we break the world into a
bunch of *tile chunks*. The tile chunks are identified with 32-bit xyz
coordinates, so in theory we can have `2^32 * 2^32 * 2^32` total tile
chunks. Entity positions are then stored using a two-tiered system: a tile chunk
and the floating-point offset within that chunk.

The tile chunks are generated on-the-fly, and stored sparsely in a hash
table. This is a must, given the huge number of possible tile chunks.

Only a small portion of the world will be simulated at any given time. That
portion is currently the reason that viewable by the camera, however in the
future we will have multiple threads to simulate neighboring parts of the world.

## Going up/down stair

We have a 3D coordinate system, going up/down stair is handled consistently (or
at least *planned* to be). Perspective is also handled correctly too so we can
interact with multiple floor levels at the same time.

<iframe src="https://drive.google.com/file/d/1cR0P4lZxFHOUswBtDQuF5mpYLiurV_2v/view?usp=sharing" frameborder="0" allowfullscreen></iframe>

## Software rendering with AVX2

The game is currently rendered entirely in software for education purpose (which
is why it's only running at 960 x 540). Only doing the operations wide with AVX2
already provides really good performance. I have also implemented dynamic
lighting but it's not optimized yet.

Right now I'm working on parallelizing the renderer.

# What did I learn?

The most important thing was how to actually learn effectively. I started out by
thinking "Well I'll just copy what Casey does and by the end: Bam!  I'll have
30-years worth of game dev experience, excluding the time it takes to figure all
these stuff out on my own". Casey seems to also encourage the same thing since
he tells his viewers at the beginning of every video to "just download the
source code and follow along at home". I found after a dozen videos that I could
not have been more wrong, for reasons include but not limited to:

- The guy is really, really fast. This fact alone already narrows down the group
of people who could possibly "follow along" to include those who can type at
least 100 wpm. Either that or you have to have to alt-tab every 5s to pause the
video (good luck if you're watching the stream on Twitch).

- Coding isn't painting. In order to follow someone's step, you gotta follow the
whole way: use the exact same naming convention, same macros, same style, same
variables, same functions, same formatting, etc. Your functions must be at the
same locations in the file and for that matter, you need to have the exact same
files. That leaves two options. Option 1: Do what you like and translate Casey's
code to your own coding style on-the-fly (remember you must do that while typing
100 wpm). Option 2: subject to the constraints specified above, which slows you
down tremendously (think 6x-10x, maybe even more).

- A portion of what he types in simply does not make sense. Although Casey is
very good, he attempts to solve new problems on the stream, and sometimes he get
things completely wrong. If you have read code any time in your life, you know
how hard it is to understand even working programs that someone else wrote. Now
imagine how hard it is to understand *non-working* programs (frankly, it's
impossible). Therefore, if you're "following along", 100% chance is you're just
training to become a stenographer by parroting what's already on the screen and
not actually thinking.

- Debugging: I guess by now you already got the point. If debugging is like
a puzzle game, then watching somebody else's debug code is like not
playing the game at all.

So what I opted to do was this: I listen to Casey explaining what he plans
to do at the start, optionally watch him implement it a little bit (just
casually watching, not even paying too much attention). Then, after I
already know what to do, I implement a solution as I understand it in my
version of the code. Doing that forces me to come up with my own solution,
make my own mistakes, debug my own bugs, encounter my own problems, develop
my own style of programming. After obtaining a working program, I watch
back how Casey does it and compare our solutions. Very often I see
inefficiencies in my code so I clean them up. Sometimes I even gets things
completely wrong, but it's still fun to go back and fix the code on my
own. I find that this is a uniquely great way to learn since not very often
do you get to see someone else doing exactly what you've already done in a
cleaner and better way.

Right now I'm 125 episodes in, still a loooong way to go (at the moment of
writing the series has 661 episodes, and it's still ongoing!) But I'll
definitely finish it some day, 2 years, 5 years, even 10 years from
now. I'll post updates of my progress on this blog.
