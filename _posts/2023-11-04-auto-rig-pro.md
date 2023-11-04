---
title: "Auto-Rig Pro is terrible, so is Blender Market"
---

{% raw %}

So I purchased the [Auto-Rig Pro](https://blendermarket.com/products/auto-rig-pro) addon for $40 just to try it out, according to a recommendations.

Then I tried the Smart feature, it crashes my computer more often than not.

I contacted the dev about it, they gave up and said there's no way to fix this problem, and asked if I would like a refund. Didn't even ask me for a screenshot, or a more precise description of the problem.

So just for education, I looked at why it hung and found that it was a memory leak. I isolated the problem down to some specific rendering method and fixed it. Anyway, the technical details doesn't matter. Point being, the bug was quite easily identified and fixed.

While looking through the code, I saw that the code quality was substandard (f.ex in the rendering code, they loop through the vertices, then do a *string search* in the vertex name just to find out what color to draw it in). So I issued a refund request to Blender Market, saying that since the code is bad, and whenever I encounter a problem in the future the developer wouldn't be willing to help me, I don't wanna risk using this tool in my pipeline.

Blender Market was like "I saw that you already talked to the developer and they assisted you in solving the problem". I said they were lying since I fixed the bug myself. They said "No one is lying", and repeated bunch of things, in the fashion of ChatGPT. The gist of it is that I can't get a refund because there was no fault in the product (even though there *would* be, if I hadn't fixed it myself).

So I asked the dev whether they could get a refund, since I no longer wanna use their addon. I got back a schmuck response saying how my solution was a "hack" (as if I was supposed to come up with a correct solution?) and that they would have no proof of me not using the product again so they can't give me the refund. The worst offender was that they accused me of "sounding pleased with myself", as if fixing your janky Python addon is worth boasting about.

I got angry and told them <REDACTED>, just keep my money if you want it so much.

Anyway, I just wanna spread the word about this. Don't pay these people, folks!

{% endraw %}
