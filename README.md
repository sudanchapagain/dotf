# dotf

A small dotfiles manager which symlinks the defined files in
`~/.dotfiles/.mappings.toml`.

## How do i use `dotf`?

In `~/.dotfiles/`, create a new file called `.mappings.toml`. It should be in TOML
format with everything under `[general]` table. The entries should follow
`"source" = "destination"` pattern where source should have path relative to
`~/.dotfiles/` whereas destination should not.

An example `.mappings.toml` file:

```toml
[general]
# ghostty/config is ~/.dotfiles/ghostty/config
"ghostty/config" = "/home/username/.config/ghostty/config"
# starship/config is ~/.dotfiles/starship/config
"starship/config" = "~/.config/starship/config"
 ```

Then, with the CLI you can run `dotf link` to map everything. If you want to
remove all links then use `dotf remove`. If you want to check status of
entries on which one is not linked and which is use `dotf status`.

## What more can i do?

Just run `dotf -h`. You should see something similar to output below:

```console
$ dotf -h

Usage: dotf [--help] [--version] {remove,status,sync}

dotf - manage dotfiles with symlinks

Optional arguments:
  -h, --help     shows help message and exits
  -v, --version  prints version information and exits

Subcommands:
  remove        Remove linked dotfiles.
  status        Show the status of dotfiles.
  sync          Sync everything to defined paths.
```

The sync command specifically has few flags:

```console
$ dotf sync -h

Usage: dotf sync [--help] [--version] [[--force]|[--skip-conflicts]] [--dry-run]

Sync everything to defined paths.

Optional arguments:
  -h, --help            shows help message and exits
  -v, --version         prints version information and exits
  -f, --force           Forcefully link by renaming existing files when conflicts arise.
  -s, --skip-conflicts  Skip linking for all conflicts that arise.
  -d, --dry-run         Skip destructive actions while appearing to perform said action.
```


> [!TIP]
> If you are using Git or any other version control remember to ignore `.state`
> file in your `~/.dotfiles/` directory.

## How do i build from source?

`dotf` does not do funny business, you can simply run the following command to
build the program.

```nushell
meson setup --reconfigure build
meson compile -C build
```

## What are my rights to the code?

Everything is licensed under the MIT license. See [LICENSE](./LICENSE) file
for full information.
