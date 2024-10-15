// Τύποι που χρησιμοποιούνται σε πολλά modules
#pragma once
#include <stdbool.h> 

// Pointer προς ένα αντικείμενο οποιουδήποτε τύπου.
typedef void* Pointer;

// Δείκτης σε συνάρτηση που συγκρίνει 2 στοιχεία a και b και επιστρέφει:
// < 0  αν a < b
//   0  αν a και b είναι ισοδύναμα (_όχι_ αναγναστικά ίσα)
// > 0  αν a > b
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Δείκτης σε συνάρτηση που καταστρέφει ένα στοιχείο value
typedef void (*DestroyFunc)(Pointer value);