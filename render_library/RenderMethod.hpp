#pragma once


/**
 * Books 1 & 2 share the same method of rendering, but book 3 has a different model (e.g. the use of PDFs), which
 * makes rendering, well, different.  To be able to support both ways, we need an enumeration so materials can know
 * if they should be using book 1/2 's model, or book 3's.
 */
enum class RenderMethod
{
    Books1And2 = 0,
    Book3
};
