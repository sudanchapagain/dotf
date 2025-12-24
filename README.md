# dotf

A small dotfiles manager which symlinks the defined files in
`~/.dotfiles/.mappings`.

## How do I use dotf?

- In `~/.dotfiles/` create a new file called `.mappings`
- It should be in TOML format with everything under `[general]` table.
- The entries should follow `"source" = "destination"` pattern where source
  should have path relative to `~/.dotfiles/` whereas destination should not.

  An example `.mappings` file:

  ```toml
  [general]
  # ghostty/config is ~/.dotfiles/ghostty/config
  "ghostty/config" = "/home/username/.config/ghostty/config"
  # starship/config is ~/.dotfiles/starship/config
  "starship/config" = "~/.config/starship/config"
  ```

- Then, with the cli you can run `dotf link` or `dotf -l` to map everything.
- If you want to remove all links then use `dotf remove` or `dotf -r`.
- If you want to check status of entries on which one is not linked and
  which is use `dotf status` or `dotf -s`.

## What more can I do?

Just run `dotf -h`. You should see something similar to output below:

```txt
dotf - manage dotfiles with symlinks

Usage: dotf [OPTIONS] [COMMAND]

Commands:
  link    Link dotfiles from .dotfiles directory to defined paths
  remove  Remove linked dotfiles
  status  Show the status of dotfiles
  help    Print this message or the help of the given subcommand(s)

Options:
  -f, --force
  -d, --dry-run
  -h, --help     Print help
  -V, --version  Print version
```


> [!IMPORTANT]
> If you are using Git or any other version control remember to ignore `.state`
> file in your `~/.dotfiles/` directory.

## How do i build from source?

`dotf` does not do funny business, you can simply run the following command to
build the program.

```sh
cargo build .
```

## What are my rights to the code?

Everything is licensed under the MIT license. See [LICENSE](./LICENSE) file
for full information.
