# Trim

A quick and dirty little command-line programme to trim/replace a
specified prefix off file-names of all files in the directory
the programme has been executed in.

All the changes will be displayed and the user asked to confirm.

Usage:

```
trim [-d] <prefix> [<replacement>]
```

`-d` will execute a dry run, making no changes.

`<prefix>` refers to the text which will be removed.

`<replacement>` refers to the text which will replace the prefix.
If omitted, the prefix is simply removed.
