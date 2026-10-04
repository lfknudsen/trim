# Trim

A quick and dirty little command-line programme to trim/replace a
specified prefix or postfix off file-names of all files in the directory
the programme has been executed in.

Postfix trimming ignores the extension.

All the changes will be displayed and the user asked to confirm.

Usage:

```
trim [<options>...] <prefix> [<replacement>]
```

Options:
`-d` or `--dry-run` will execute a dry run, making no changes.

`-q` or `--quiet` will avoid printing the list of changes.

`-a` or `--autoconfirm` will not ask the user for confirmation.

`-r` or `--recursive` will look in sub-directories as well.

`--right` will trim a postfix (ignoring the extension) instead of prefix.

`--both` will trim both prefix and postfix.

The short-form arguments can be combined, e.g. `-qa`.

`<prefix>` refers to the text which will be removed.

`<replacement>` refers to the text which will replace the prefix.
If omitted, the prefix is simply removed.
