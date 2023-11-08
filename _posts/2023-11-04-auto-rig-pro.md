---
title: "Bad open source experience: Auto-Rig Pro is terrible, so is Blender Market (and r/blender too)"
---

{% raw %}

NOTE: Quoted text aren't verbatim, unless said otherwise.

I got into CG animation recently to make a movie. I wanted a solution to quickly rig characters, since I anticipate having many humanoid characters in the movie.

So I purchased the [Auto-Rig Pro](https://blendermarket.com/products/auto-rig-pro) addon (ARP) for $40 just to try it out, according to a recommendation from reddit.
Then I installed it and tried the advertised Smart Rig feature, it crashes my Blender more often than not.

I contacted the ARP dev (Artell) about it, they gave up and said there's no way to fix this problem, and asked if I would like a refund. Didn't even ask me for a screenshot, or a more precise description of the problem.

So just for education, I looked at why it hung and found that it was a memory leak. I isolated the problem down to some specific rendering method and found a way to work around it. Anyway, the technical details doesn't matter. Point being, the bug was quite easily identified and fixed.

While looking through the code, I saw that the code quality was substandard (f.ex in the rendering code, they loop through the vertices, then do a *string search* in the vertex name just to find out what color to draw it in). So I issued a refund request to Blender Market, saying that since the code is bad, and whenever I encounter a problem in the future the developer wouldn't be willing to help me, I don't wanna risk using this tool in my pipeline.

Blender Market was like "I saw that you already talked to the developer and they assisted you in solving the problem". I said they were lying since I fixed the bug myself. They said "No one is lying", and repeated bunch of things, in the fashion of ChatGPT. The gist of it is that I can't get a refund because there was no fault in the product (even though there clearly *would* be, if I hadn't fixed it myself).

So I asked the dev whether they could get a refund, since I no longer wanna use their addon. I got back a schmuck response saying how my solution was a "hack" (as if I was supposed to come up with a correct solution?) and that they would have no proof of me not using the product again so they can't give me the refund. ~~The worst offender was that they accused me of "sounding pleased with myself", as if fixing your janky Python addon is worth boasting about.~~

I got angry and told them <REDACTED>, just keep my money if you want it so much.

Anyway, I just wanna spread the word about this. Don't pay these people, folks!

Update: This same content was uploaded to r/blender subreddit.
It was removed after a few minutes.
I DMed the mods to ask for the reason and they haven't replied to me yet (even though they were so quick to censor my post).
I don't know what kind of censorship is happening here, but I can't say I haven't seen it.

Update 2: This is the verbatim schmuck response from the dev that made my blood boil (NOTE: This is *after* I asked for the refund. I had been completely polite and "diplomatic" up to this point.)

---

> I contacted support and asked for help. They gave up immediately and told me to get a refund

The Blender Market is the merchant platform. In case of problems, the creators of the products must offer support, which is my role here.



I saw that the code was of substandard quality; plus the unwillingness to help from the developer made me not wanna bet my pipeline on this product.

This does not make sense.

You first fix the problem by yourself with a quick hack, then say: It still doesn't looks exactly right, but works for me :)
You sounded pleased.

Then, you suddenly ask for a refund, invoking arrogant reasons of bad code quality, which is a subjective opinion, and even say that I was unwilling to help?
Even though I feel sincerely sorry for the inconvenience -it is always a pain hearing my tool is not working right, I want my customers to be happy- this issue only happens on your Mac system. No other users reported that so far, and yes multiple users reported they were using a Mac too, you are not the first one. Consequently, it could likely be due to your graphic drivers, or an incompatibility with your environment.  The quick hack you made did the trick by chance, but this is not a real fix to me. There is no true reason to change 'TRI_FAN' to 'TRI_STRIP' as drawing method. The Blender drawing API does not provide much help about it, these are built-in GPU functions that cannot be modified when writing addons which is higher level code. This memory leak may even be fixed later with a new Blender update, or graphic driver update.

Sometimes, some users report issues to me, that only happen because they installed a specific unofficial version of Blender, or have a specific environment... this is why I have to be careful when modifying the code because something is not working right only for a single user.

I will stop here the geeky tech debate. But the sudden offensive change in your attitude is questionable. Offering my work and time for free is not my thing, I have no proofs that you won't be using it later, so to me this is not a valid refund request that we have here.

---

Update 3: Verbatim response from Blender Market, saying that I have to find another issue and report that to the dev before I'm allowed to get the refund

---

I see that you've worked with Artell directly and he's provided you some assistance in getting the addon working for your particular set-up.

Please let him know what additional problems you are having, so he has the opportunity to help. If you still would like a refund, please inform Artell directly in DMs that you're seeking a refund.

Please allow them at least 72 hours to respond. If the Creator isn't able to solve the issues then we'll happily issue a refund.

---

Update 4: After rereading the message from Artell, I realized that it wasn't as schmuck as I thought (I thought they were saying I was "pleased with myself" for fixing the bug).
So I sent an apology for the REDACTED part.
However, I still think it's shitty to judge my tone and criticize the edit I made to *your program* when *I don't care* and just want a refund.

{% endraw %}
