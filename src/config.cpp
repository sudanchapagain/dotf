#include "include/config.hpp"
#include "include/path.hpp"
#include "include/helpers.hpp"

Mappings
load_mappings(toml::node_view<toml::node> table) {
    Mappings result;

    if (!table || !table.is_table()) {
        return result;
    }

    for (auto&& [key, value] : *table.as_table()) {
        if (!value.is_string()) {
            // TODO:
            continue;
        }
        Mapping m;
        m.source = std::string(key.str());
        auto val = value.value<std::string_view>();
        if (!val) { continue; }
        m.destination = std::string(*val);
        result.push_back(std::move(m));
    }

    return result;
}

Mappings
resolve_links_to_absolute(Mappings map) {
    auto dotfiles_path = get_dotfiles_path();
    auto home = get_home_directory();

    // source:
    // the paths are relative to the .dotfiles directory.
    // 
    // destination:
    // the destination paths can be either absolute or relative from home. i.e.
    // either
    //     - ~/.config/some/file.txt
    //     - /home/username/.config/some/file.txt

    for (auto& m : map) {
        auto s = m.source;
        m.source = (std::string) (dotfiles_path / s);

        auto d = m.destination;
        m.destination = starts_with(d, "~")
            ? (std::string) home + d.substr(1)
            : d;
    }

    return map;
}
