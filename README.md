shared-hash-tables
==================

New efficient hashing methodology allowing hash tables to be shared by multiple dictionaries.

Current Makefile builds a test binary that inserts 1 million keys and then fetches them back out.
On my machine this test completes in 0.25 seconds. Note that this library is not meant for having
millions of items in a single hash; this is just a demo to prove it can. It peforms similarly up
to 3.5 million keys or so currently.
