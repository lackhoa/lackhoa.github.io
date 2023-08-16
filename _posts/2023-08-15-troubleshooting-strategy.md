---
title: "Optimal Troubleshooting Strategy"
---

{% raw %}

# Introduction

As an IT person, I've had to troubleshoot a lot of issues throughout the years.
The issues often looks like this: "the client computer can't connect to a server, and we need to figure out what's wrong".

The question is: What's the best way to do that?
Even though I've done it a lot, I hadn't formulated my strategy clearly until someone asked me during an interview.
But now I have, and I'll describe it here in this post.

# Framing the problem

In a computing system, every moving part must be in perfect order for the whole thing to work.
Let's say that in our case, we can break down the system into multiple parts like so (for simplicity, suppose we're only concerned about the client side).

```
PART: Application | OS | Network | Firewall
```

But how can we break things down in this manner?
Well there is no general answer, you have to know how the system works.
In this case, I know the client is an program which must send the request to the server.
After formulating the request, it must make a system call to the operating system using some library.
The OS then sends the request packet down the network.
And when it gets to the other end, the packet must traverse a firewall on a router in the same subnet.
Any failure (or misconfiguration) in the application, the OS, the network, or the firewall will bring the commucation down.

Things can get arbitrarily complicated, but how complicated should you get?
Well you can imagine when the packet traverse the network, it must go through a bunch of switches/routers.
While it's true that any of those switches/routers could fail, it would be crazy to start examining all the network appliances right from the get-go without determining whether the entire communication link works or not.
Therefore, I believe it's the best idea to break the problem down to as few parts as we can.
Then, when we know the problem, we will break that problem down into smaller parts and continue the process (recursively).

Another assumption throughout this story is that these separate parts can be examined individually.
For example, suppose you know there's something wrong with the application, you wouldn't need to still examine whether the network works or not.
The network link doesn't discriminate, it will send the packet no matter which application sent it.
This assumption might not hold, for example, if you don't have [net neutrality](https://en.wikipedia.org/wiki/Net_neutrality).
You could imagine an ISP blocking BitTorrent traffic, and the client in this picture happens to be communicating to a BitTorrent server.
In that case, we need to add another part to the equation, namely "ISP filter".

Generally, the more complicated a system is, the more moving parts you have to check.
Regardless of the politics, we know for a fact that net neutrality makes networking problems harder to debug, because it makes things more complicated.
The same thing applies to DRM: anything that adds extra hurdles in the functioning of a system will only make it more likely to fail.

Suppose that there's only one thing wrong in the entire system (if there are more failures, you'll still have to fix them all anyway).
We can assign to each part the chance that it might fail.
If we don't know anything about the nature of the bug, we can start by assigning equal chances.

```
PART:   Application | OS  | Network | Firewall
CHANCE: 25%         | 25% | 25%     | 25%
```

Having worked through the problem before, I've figured out that it's more convenient if we replace *chance* with *value*.

```
PART:  Application | OS | Network | Firewall
VALUE: 1           | 1  | 1       | 1
```

The nature of the problem doesn't change if you use either value or chance.
Using value, to retrieve the chance that a component might fail, you take its value and divide by the total value, which is 1/4 = 25% (same as before).
You can already imagine why I like value more: suppose we've examined the firewall and note that it is working perfectly, the new problem description would then be.

```
PART:  Application | OS | Network |
VALUE: 1           | 1  | 1       |
```

Note that the values are still 1. Whereas if you use chance, they would have to be updated

```
PART:   Application | OS  | Network |
CHANCE: 33%         | 33% | 33%     |
```

Equal chances are rare. In real life, you'd generally know what breaks more often.
- Obviously, the application would break more often than the OS, since it's updated more frequently (and by smaller team, could even be your team).
- Next comes the network: someone might have tripped on a wire, or turned off a router somewhere.
- Next comes the firewall, someone could have easily updated a firewall rule to disallow Internet access for our client computer (kinda specific but it could happen).
- The most stable thing in this case is the OS. Of course the failure chance goes up if you've updated the OS recently recently, or you might have changed the driver.

Give that, our final value assignment is as follows

```
PART:  Application | OS | Network | Firewall
VALUE: 8           | 1  | 4       | 2
```

Given only this info, the best strategy is trivial to see: just check the most likely thing first.
If it doesn't work, we're done troubleshooting (we still have to fix it).
If that part is in perfect working order, go on to the second-hardest part, and so on.
In our case, the troubleshooting order is like this

```
Application -> Firewall -> Network
```

Note that there's no need to check the OS: if everything else works, the OS is definitely at fault.

Why is this the best strategy?
Because if you've found that there's a bug in one of the parts, you wouldn't need to examined the rest.
Therefore, if you check the part that has a higher chance of failure, you'd be more likely to succeed, thus save time.

In reality, however, there is one more complication: it requires a different amount of time to examine each part.
Even though the application might fail often, it takes more time to check what's wrong: you have to grab the up-to-date source code, read through it and understand what it does, for example.
Whereas the OS may be very easy to check, even though it doesn't fail that often.

So let us add another dimension to the problem: the cost (normally it is time, but in some cases it could also be money. Usually you can just convert time into money anyway.)
- The OS is super easy to check, just do a "ping" or "netcat" to some other host on the same network.
- The firewall is also very easy to check: you just need to check the rules on your router.
- Checking the network requires you to have another host plugged in to the same subnet. And from that host you make a request to the same server. Of course you need to be sure that this host is in perfect working order otherwise (or else you'd need to troubleshoot it too :>).
- The application part turned out to be the toughest (in our case): you have to know the programming language, and which file to check. That is, assuming you even have the source code.

Here is the final problem specification

```
PART:  Application | OS | Network | Firewall |
VALUE: 8           | 1  | 4       | 2        |
COST:  32          | 1  | 8       | 4        |
```

As you might have noticed, it seems suicidal if you dive head-first into reading the application source code.
Even though it might be the most brittle part.
Whereas the OS might not break that often, checking it is a piece of cake, so it doesn't hurt to make a quick ping before doing anything else.

But the problem is: is there any way that we can justify our intuition?

# The formula for "benefit"

So currently there are two variables which seem to contradict each other:
- Value: The more likely it is to fail, the earlier you should check it.
- Cost: The harder something is to check, the more we should delay checking it.

In order to make a decision on which part to check first, let's assign a *benefit* to each part.
What this benefit is, we don't know exactly.
But we do know it has to look something like this

```
benefit = value / cost
```

What does this formula say?
Benefit varies directly proportional to value, and inversely proportional to the cost.
Intuitively this makes complete sense: If somebody sells you an apple for $4 and somebody else sells you the exact same apple for $2; you'd say the it's more benficial to buy the second apple.
Another scenario: if I sell one apple for $4, and you sell three apples for $4; you'd be offering a deal three times as beneficial.

Maybe there could be a better word than "benefit", I just don't know it.

Now I will propose a strategy for troubleshooting: just check the thing that has the most benefit first.
This strategy sounds simple and intuitive, but we have to verify that it is the best strategy.
To do that, we need to know what "best" means.

# What does it mean for a strategy to be the best?

There is generally one very good answer: the best strategy takes the least time (on average).
You can imagine other metrics, but most of the time we'd just stick to this one.
We call this metrics the "expected cost".

Let's work through some examples to demonstrate this.
We start with a system with two parts, which can only have one strategy:

```
PART:  Application | OS |
VALUE: 8           | 1  |
COST:  32          | 1  |

E(Application) = 32
E(OS)          = 1
``` 

So if you check the OS with a simple "ping", you'd save 31 seconds.

That was pretty easy. Things get more interesting, however, when there are three or more parts:

```
PART:  Application | OS | Network |
VALUE: 8           | 1  | 4       |
COST:  32          | 1  | 8       |

E(Application -> Network) = (8/13 * 32) + 5/13 * (32 + 8) = 35
E(OS -> Network)          = (1/13 * 1) + 12/13*(1 + 8)    = 8.3
```

This is a big time save: if you go with the second strategy, you'd save 26.7 units on average!
For those who aren't familiar with these calculations, it goes like this: suppose you choose to troubleshoot the application first:
- 8 out of 13 times you find a bug in the code. Troubleshooting costs 32 units.
- 5 out of 13 times you find the code to be flawless, then you have to check both the application and the network. Troubleshooting costs 40 untis.

Probabilistically speaking, since the majority of the time  we only spend 32 units, it makes perfect sense that the average cost (35 units) tilts towards the lower end.

# Empirical observation

Without any reasoning at all, we can find out what actually is the best strategy by writing a brute-force program to compute the average cost of all possible strategies, and pick the best one.

```python
#!/usr/local/bin/python3

from dataclasses import dataclass
from collections.abc import Iterable
from common import *
from tabulate import tabulate

@dataclass
class Part:
    name: str
    value: float
    cost: float
    def __repr__(self):
        return f"({self.value}, {self.cost})"

@dataclass
class Strategy:
    sequence: tuple[Part,...]
    cost: float

PARTS = (Part('Application', 8, 32), Part("OS", 1, 1), Part("Network", 4, 8), Part("Firewall", 2, 4))

null_strategy = Strategy(tuple(), 0.0)

def optimize_(parts: Iterable[Part], total_value: float) -> Strategy:
    best_strategy: Strategy = null_strategy
    if len(parts) == 1:
        my_assert(approximately_equal(total_value, parts[0].value))
        best_strategy = Strategy((), 0)
    else:
        for part in parts:
            new_parts = tuple(j for j in parts if j is not part)
            recurse = optimize_(new_parts, total_value - part.value)
            chance = part.value / total_value
            new_cost = chance * part.cost + (1 - chance) * (part.cost + recurse.cost)
            if (best_strategy is null_strategy) or (new_cost < best_strategy.cost):
                new_sequence = (part,) + recurse.sequence
                best_strategy = Strategy(new_sequence, new_cost)
    return best_strategy

def optimize(parts):
    return optimize_(parts, sum((part.value for part in parts)))

result = optimize(PARTS)
print(tabulate(([part.name, part.value / part.cost] for part in result.sequence),
               headers=["name", "benefit"]))
print()
print(f"Total cost: {result.cost}")
```

I won't explain the code above, but I'll just point out that the average strategy cost I mentioned is in the following part.

```
            chance = part.value / total_value
            new_cost = chance * part.cost + (1 - chance) * (part.cost + recurse.cost)
```

Anyway, this program gives the result

```
name        benefit
--------  -------
OS            1
Network       0.5
Firewall      0.5

Total cost: 11.133333333333333
```

Note that the outputted strategy contains decreasing benefit, which supports our hypothesis that benefit is the right thing to focus on.

Oh, and let's do one last sanity check. How would I troubleshoot the issues?
- I would totally try to do a quick ping first.
- Then check that the network/firewall is okay.
- If all else fails, I'll read the source to figure out what the app is doing wrong and fix it.

So yes, I also agree with this strategy.

# Proof that prioritizing benefit is optimal

To prove the legitimacy of our strategy, we use a cool little trick: slice up the problems into equal parts (in cost), like so

```
PART:  OS | Firewall (a) | Firewall (b) | Firewall (c) | Firewall (d) | Network (a) | ...
VALUE: 1  | .5           | .5           | .5           | .5           | .5          | ...
COST:  1  | 1            | 1            | 1            | 1            | 1           | ...
```

The way to interpret this is that we've relaxed the problem. Now, instead of having to troubleshoot the entire firewall, you can troubleshoot one part of the it, then move on to troubleshoot the OS, then switch back to the firewall, etc.

In this new problem, now that the cost of all parts are 1, the optimal strategy is trivial: we attempt to verify the parts in order of decreasing value, for example:

```
OS -> Firewall (a) -> Firewall (b) -> Firewall (c) -> Firewall (d) -> Network (a) -> ... -> Network (h)
```

So we have the optimal strategy for the new problem.
But since the new problem is easier than or equally as hard as the original problem, its optimal strategy has to also be as expensive or cheaper than the original's.
In this case, the new optimal strategy has the same expected cost as our old strategy.
Therefore, our "greedy" strategy of prioritizing benefit is indeed optimal.

# Conclusion

You may find the solution obvious, but I have heard of stories about "IT supporters" who carries around a piece of paper detailing how to solve common problems.
And every time there's a problem, they would pull out the piece of paper and try random things until something works.

Now you know for a fact that it's not the right thing to do, is it?

As a final note, this strategy is only good for troubleshooting computer systems and other nerdy things such as eletronic circuits.
It isn't applicable for finding lost items, or hunting for treasures in your backyard.
The reason is because finding things in reality require you to scan the entire area.
You generally don't have the ability to just "know" whether or not an item is residing within a large area, without exerting a corresponding amount of effort.

{% endraw %}
