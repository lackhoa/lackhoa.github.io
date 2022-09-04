---
title: "Bitmap mermoy: How to store complexity in data structures"
---

{% raw %}

# What's in this post?

I'm gonna talk about how storing complexity in your data structures will almost always be a win,
through examples of drawing bitmaps. The code shown below are just to demonstrate a point, so they
might be wrong.

I didn't invent any of this stuff. I only learned these through watching Handmade Hero. I don't know
if Casey invented any of this techniques himself (he probably did not).

# Introduction

I'm current making a game, and in this game I often have to deal with a lot of bitmaps. Bitmaps are,
to put it simply, a bunch of pixels[^bitmap]. Technically, they are arrays of 32-bit values, each of
which represents four 8-bit fields: red, green, blue, and alpha. The structure is as follows

```c
struct Bitmap_simple
{
    void *memory;
    v2 dim_px;
};

#define Bitmap_Bytes_Per_Pixel 4
```

Here's how you draw a bitmap to a corner of another bitmap (bitmap-ception :>). The destination
bitmap could be the game backbuffer, or a cached buffer to create a **composite bitmap**.

```c
void
drawBitmapSimple(Bitmap_simple *dst_buffer, Bitmap_simple *src_bitmap)
{
    // MARK: set start locations
    u8 *src_row = ((u8 *)src_bitmap->memory);
    u8 *dst_row = ((u8 *)dst_buffer->memory);

    for (int y = 0;
         y < (s32)src_bitmap->dim_px.y;
         ++y)
    {
        u32* src = (u32 *)src_row;
        u32* dst = (u32 *)dst_row;
        for (int x_ = 0;
             x_ < (s32)src_bitmap->dim_px.x;
             ++x_)
        {
            *dst++ = *src++;
        }
        src_row += (s32)src_bitmap->dim_px.x * Bitmap_Bytes_Per_Pixel;
        dst_row += (s32)dst_buffer->dim_px.x * Bitmap_Bytes_Per_Pixel;
    }
}
```

# I wish it was that simple!

Sad news: sometimes the source bitmap can be flipped upside-down compared to the destination (I
don't know why). To handle this, we can just have a boolean in the bitmap struct telling whether it
is flipped ("flipped" compared to some particular direction we prefer [^direction]).

```
struct Bitmap_bool
{
    void *memory;
    v2 dim_px;
    b32 flipped;
};
```

then we can draw it out as follows

```c
void
drawBitmapComplicated(Bitmap_bool *dst_buffer, Bitmap_bool *src_bitmap)
{
    // MARK: set start locations
    u8 *src_row;
    if (src_bitmap->flipped)
    {
        src_row = ((u8 *)src_bitmap->memory +
                   (s32)src_bitmap->dim_px.y * (s32)src_bitmap->dim_px.x * Bitmap_Bytes_Per_Pixel);
    }
    else
    {
        src_row = ((u8 *)src_bitmap->memory);
    }
    u8 *dst_row = ((u8 *)dst_buffer->memory);

    for (int y = 0;
         y < (s32)src_bitmap->dim_px.y;
         ++y)
    {
        u32* src = (u32 *)src_row;
        u32* dst = (u32 *)dst_row;
        for (int x_ = 0;
             x_ < (s32)src_bitmap->dim_px.x;
             ++x_)
        {
            *dst++ = *src++;
        }
        if (src_bitmap->flipped)
        {
            src_row -= (s32)src_bitmap->dim_px.x * Bitmap_Bytes_Per_Pixel;
        }
        else
        {
            src_row += (s32)src_bitmap->dim_px.x * Bitmap_Bytes_Per_Pixel;
        }
        dst_row += (s32)dst_buffer->dim_px.x * Bitmap_Bytes_Per_Pixel;
    }
}
```

As you can see, the code gets a bit more complex, even though our task is conceptually still
simple. This complexity comes from the fact that when the bitmap is flipped, we have to both
pre-step to the end of the bitmap memory and go backward from there. We can always assume that the
destination is in the correct orientation (since that's what "correct orientation" means).

# Enhancement number one: Add a pitch

We see the expression `bitmap->dim_px.x * Bitmap_Bytes_Per_Pixel` appearing in several places, so
let's encode this value in the bitmap itself. This addition has a surprising benefit

```c
struct Bitmap_pitch
{
    void *memory;
    v2 dim_px;
    s32 pitch;
};

void
drawBitmapEnhanced1(Bitmap_pitch *dst_buffer, Bitmap_pitch *src_bitmap)
{
    // MARK: set start locations
    u8 *src_row;
    if (src_bitmap->pitch < 0)
    {
        src_row = ((u8 *)src_bitmap->memory + (s32)src_bitmap->dim_px.y * src_bitmap->pitch);
    }
    else
    {
        src_row = ((u8 *)src_bitmap->memory);
    }
    u8 *dst_row = ((u8 *)dst_buffer->memory);

    for (int y = 0;
         y < (s32)src_bitmap->dim_px.y;
         ++y)
    {
        u32* src = (u32 *)src_row;
        u32* dst = (u32 *)dst_row;
        for (int x_ = 0;
             x_ < (s32)src_bitmap->dim_px.x;
             ++x_)
        {
            *dst++ = *src++;
        }
        src_row += src_bitmap->pitch;
        dst_row += dst_buffer->pitch;
    }
}
```

The benefit is that now we can remove the `flipped` boolean, since if the pitch is negative then it
signitifes that the bitmap is supposed to be read from the end. Note that the value of this `pitch`
field can be purely implied from the other fields in the bitmap struct, which sounds repulsive to me
at the start. But after a while I just thought "Eh, why not?". We only set up the bitmap struct
once, but we use it many times. In fact in the above draw call you already see it used twice: when
we increment the `src_row` and `dst_row` pointer. We also draw many other things, such as
rectangles, to the bitmap struct.

Adding complexity in once place to reduce complexity in multiple other places is an overall win.

# Enhancement number two: Changing where the memory is

While `drawBitmapEnhanced1` is great, it still has to pre-step the `src_row` pointer to the end of
the bitmap memory to begin reading. What if we could eliminating also that step? In fact it's pretty
easily done by setting the memory pointer to either the first row (if the orientation is correct) or
the last row (if it's flipped upside-down). Since the semantics has changed, we can rename the
`memroy` field to `starting_row`.

```c
struct Bitmap_starting_row
{
    void *starting_row;
    v2 dim_px;
    s32 pitch;
};

void
drawBitmapEnhanced2(Bitmap_starting_row *dst_buffer, Bitmap_starting_row *src_bitmap)
{
    // MARK: set start locations
    u8 *src_row = ((u8 *)src_bitmap->starting_row);
    u8 *dst_row = ((u8 *)dst_buffer->starting_row);

    for (int y = 0;
         y < (s32)src_bitmap->dim_px.y;
         ++y)
    {
        u32* src = (u32 *)src_row;
        u32* dst = (u32 *)dst_row;
        for (int x_ = 0;
             x_ < (s32)src_bitmap->dim_px.x;
             ++x_)
        {
            *dst++ = *src++;
        }
        src_row += src_bitmap->pitch;
        dst_row += dst_buffer->pitch;
    }
}
```

And we're now back to the good-old `drawBitmapSimple`!

Again, this adds some complexity to the bitmap loading code (which I didn't show), but still an
overall win.

# Aside: Handmade Hero update

I'm up to episode 111 now (which is pretty darn fast). Lighting is now behind us. I remember dealing
with perspective projection and multiple cameras. I remember not liking very much how the renderer
has to care about the monitor size and camera distance (and it doesn't have to, which I'll write
about in the future).

We'll be optimizing the renderer soon, I sooo look forward to it!

[^bitmap]: They would only be pixels when you blit them straight out to the screen. However, most of
    the time you don't really do that. Usually you have to perform some sort of scaling and
    transformations before blitting, so one 32-bit value in a bitmap (a.k.a one "texel") might be
    used to draw many pixels, and vice-versa.

[^direction]: For example, we prefer bottom-up bitmaps because y-up is the convention in math.

{% endraw %}
