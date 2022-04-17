# Result

Provides a C++ result class in a single header.

A result class can be used instead of exceptions to signal some function's
failure to the caller. Objects of the result type contain either one of two
types.  One of these types represents a success, while the other represents a
failure.  Both can optionally contain a value: the function return
value for a success or some error object to convey the cause of a failure.

Currently __very much a work in progress__. This is mainly on here to share the
code with myself, but feel free to do with it what you want.

## Why use results instead of exceptions?

In short: exceptions obfuscate execution flow.

When a function throws an exception, it can be caught by the immediate caller,
or by the caller's caller, or by the caller's caller's caller, and so on... (or
not at all). This can be _exceptionally_ annoying to debug. When a function
returns a result instead, the caller has to choose to handle failures locally
or to propagate them. "But you can also do that with exceptions!" Well yes, you
can, but you don't have to. It's easy enough to forget (intentionally or
unintentionally) to handle all errors some function might throw. When using a
result return type, since it's part of the function signature, the caller is
forced to make a deliberate decision to handle or ignore potential failures.
That way they can at least feel bad about theirself they explicitly ignore
errors. Additionally, results have the added benefit that they are more
expressive, when compared to returning plain error codes, as is common with C
APIs.

## TODO list
* Convenient conversion into a different type of result that has either the same
  Success or Error type.
* Probably finish this README or something
* ...
