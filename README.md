# Jk
An [ossia score](https://ossia.io) add-on applying jq-style filters to values.

For a map such as `{"action":"back","state":"s2"}`,
`select(.state == "s2")` emits the map and `select(.state != "s2")` emits nothing.
Each filter result becomes an output value. Input maps are used directly;
strings containing JSON are not implicitly parsed.

See [the evaluator documentation](3rdparty/jk/README.md) for supported syntax,
compatibility limits, and verification commands.
