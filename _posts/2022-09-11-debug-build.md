---
title: "Never, ever use '-O0' in clang"
---

{% raw %}

# Introduction

There are many things about compilers that you newbies like me assume, that are just plain wrong! A
great example is the clang optimization flags. Given that `-O` specifies the "optimization level",
you'd think the progression is like so: `-O0` means "no optimization", up to `-O3` meaning
"maximally optimized".

Sadly, that model is *wrong*, at least for `-O0`. At `-O0`, clang does some of the most unimaginable
things to our code. In this post, I will show you why.

# Why do you even need `-O0`? Why not just use the optimized code?

Because most of the development time is spent stepping through my code, either to debug it or to
understand what it's doing. For both of those purposes, I need the generated assembly to correspond
exactly to the code I wrote. And for that, the only option I have at the moment is `-O0`.

# What is wrong with`-O0`?

The thing you have to know about `-O0` is that not only does it *not* optimize your code, it also
*pessimizes* it. Let us see an example ([godbolt link](https://godbolt.org/z/vdv4jr67s)).

```
#include <immintrin.h>

__m256 set1ps(float i)
{
    __m256 f = _mm256_set1_ps(i);
    return f;
}
```

What I want to accomplish is simple: to take a floating point value, and spread it across 8 lanes of
a 256-bit wide register. The code targets specifically AVX2, and I'm not even using some crazy
wrapper classes or operator overloads. I use the actual intrinsics that Intel provides, to tell the
compiler specifically what to do[^intrinsics].

This one line of code, even assuming full debuggability, should just generate three instructions:
one to load `i` from the stack, one to actually execute the instruction for `_mm256_set1_ps`, and
finally write `f` out to the stack:

```
__m256 f = _mm256_set1_ps(i);
```

under `-O0` it would turn into this giant spaghetti bowl:

```
        vmovss  xmm0, dword ptr [rsp + 56]      # xmm0 = mem[0],zero,zero,zero
        vmovss  dword ptr [rsp + 60], xmm0
        vmovss  xmm0, dword ptr [rsp + 60]      # xmm0 = mem[0],zero,zero,zero
        vmovss  dword ptr [rsp + 140], xmm0
        vmovss  dword ptr [rsp + 136], xmm0
        vmovss  dword ptr [rsp + 132], xmm0
        vmovss  dword ptr [rsp + 128], xmm0
        vmovss  dword ptr [rsp + 124], xmm0
        vmovss  dword ptr [rsp + 120], xmm0
        vmovss  dword ptr [rsp + 116], xmm0
        vmovss  dword ptr [rsp + 112], xmm0
        vmovss  xmm1, dword ptr [rsp + 132]     # xmm1 = mem[0],zero,zero,zero
        vmovss  xmm0, dword ptr [rsp + 128]     # xmm0 = mem[0],zero,zero,zero
        vinsertps       xmm0, xmm0, xmm1, 16    # xmm0 = xmm0[0],xmm1[0],xmm0[2,3]
        vmovss  xmm1, dword ptr [rsp + 136]     # xmm1 = mem[0],zero,zero,zero
        vinsertps       xmm0, xmm0, xmm1, 32    # xmm0 = xmm0[0,1],xmm1[0],xmm0[3]
        vmovss  xmm1, dword ptr [rsp + 140]     # xmm1 = mem[0],zero,zero,zero
        vinsertps       xmm1, xmm0, xmm1, 48    # xmm1 = xmm0[0,1,2],xmm1[0]
        vmovss  xmm2, dword ptr [rsp + 116]     # xmm2 = mem[0],zero,zero,zero
        vmovss  xmm0, dword ptr [rsp + 112]     # xmm0 = mem[0],zero,zero,zero
        vinsertps       xmm0, xmm0, xmm2, 16    # xmm0 = xmm0[0],xmm2[0],xmm0[2,3]
        vmovss  xmm2, dword ptr [rsp + 120]     # xmm2 = mem[0],zero,zero,zero
        vinsertps       xmm0, xmm0, xmm2, 32    # xmm0 = xmm0[0,1],xmm2[0],xmm0[3]
        vmovss  xmm2, dword ptr [rsp + 124]     # xmm2 = mem[0],zero,zero,zero
        vinsertps       xmm2, xmm0, xmm2, 48    # xmm2 = xmm0[0,1,2],xmm2[0]
        vmovaps xmm0, xmm2
        vinsertf128     ymm0, ymm0, xmm1, 1
        vmovaps ymmword ptr [rsp + 64], ymm0
        vmovaps ymm0, ymmword ptr [rsp + 64]
        vmovaps ymmword ptr [rsp], ymm0
```

Now what in the world is that? I don't know and don't even want to ask. If I have to guess, this
part is where `xmm0` gets stored to the stack

```
        vmovss  dword ptr [rsp + 268], xmm0
        vmovss  dword ptr [rsp + 264], xmm0
        vmovss  dword ptr [rsp + 260], xmm0
        vmovss  dword ptr [rsp + 256], xmm0
        vmovss  dword ptr [rsp + 252], xmm0
        vmovss  dword ptr [rsp + 248], xmm0
        vmovss  dword ptr [rsp + 244], xmm0
        vmovss  dword ptr [rsp + 240], xmm0
```

That could have been accomplished with one instruction, but regardless: what are the rest of the
instructions doing, seriously? If somebody understands that, please let me know.

So given that `-O0` is completely useless if your code is doing heavyweight computation, what else
do we have?  Nothing. If you jump to `-O1`, clang already assumes you don't wanna debug it. There is
also the `-Og` option, which is *actually intended for debugging* (meaning that the compiler will
generate fully debuggable code, while not being a complete jerk to us as well as the CPU). However,
that option is useless at the moment (clang 15.0.0): it still optimizes away variables, and it still
changes code flow.

Maybe it's just me, but I just can't stand having my variables optimized away, or the statement
cursor jumping up and down while I'm stepping through my code. If somebody from the LLVM team is
reading this: please, please make `-Og` a reality. I already went through such lengths to optimize
my code, so that it is both performant and debuggable, I don't want the compiler to completely wreck
it.

Until then, I guess I'll just keep having to lookout for these landmines.

[^intrinsics]: Intrinsics don't have to correspond to CPU instruction, which is the case
    here. However, the compiler can choose to emit whatever instruction it sees fit, so it doesn't
    matter.

{% endraw %}
