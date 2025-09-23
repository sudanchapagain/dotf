dotf
====

a small dotfiles manager which maps your declared files to destination with
symlinks.

usage
-----

- in `~/.dotfiles/` create a new file called `.mappings`
- it should be in TOML format with everything under `[general]` table.
- the entries should be `"source" = "destination"` where source should have path
  relative to `~/.dotfiles/` whereas destination should not.

  example `.mappings`

  ```toml
  [general]
  # ghostty/config is ~/.dotfiles/ghostty/config
  "ghostty/config" = "/home/username/.config/ghostty/config"

  # starship/config is ~/.dotfiles/starship/config
  "starship/config" = "~/.config/starship/config"
  ```

- then, start using the CLI with `dotf link` or `dotf -l` to map everything.
- if you want to remove all links then use `dotf remove` or `dotf -r`.
- if you want to check status of entries on which one is not linked and
  which is use `dotf status` or `dotf -s`.

**IF YOU ARE USING GIT OR ANY OTHER VERSION CONTROL REMEMBER TO IGNORE `.state`
FILE IN YOUR `~/.dotfiles/` DIRECTORY.**

building
--------

```
cargo build .
```

license
-------

Everything is licensed under MIT. See [LICENSE](./LICENSE)
