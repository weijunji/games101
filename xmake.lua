-- define project
set_project("games101")
set_version("0.1.0", {build = "%Y%m%d%H%M"})

-- set common flags
set_languages("cxx17")

-- add build modes
add_rules("mode.release", "mode.debug")

-- includes sub-projects
includes("pa*")
