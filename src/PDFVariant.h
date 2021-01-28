#pragma once

#include <variant>
#include "PDFs/CosinePDF.h"
#include "PDFs/HittablePDF.h"
#include "PDFs/MixturePDF.h"


/**
 * PDFVariant is variant container, that well, contains sublcasses of IPDF.
 *
 * It's indended purpose is to be used to save us from having functions return a pointer
 * to a dynamically allocated probability distributtion function.  In the book's architecture
 * There is quite a bit amount of dynamically allocated memory being created during render time.
 * That's not really good for performance.  Using a typesafe container might possibly take up more
 * memory, but it lets the compiler better predict how that memory is going to be used, and thus
 * optimize more for it.
 *
 * Also, due to rules of forward delcrations and whatnot (and that PDFVariant is used in MixturePDF)
 * We have to put some includes below the `using` statement.
 */
using PDFVariant = std::variant<std::monostate, CosinePDF, HittablePDF, MixturePDF>;
