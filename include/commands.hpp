#pragma once

#include "include/config.hpp"

void
sync(
    bool is_dry_run_enabled,
    bool is_force_enabled,
    bool is_skip_conflicts_enabled,
    Mappings ns,
    Mappings os
);

void
remove(Mappings os);

void
status(Mappings ns, Mappings os);
