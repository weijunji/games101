add_requires("eigen3")
add_requires("opencv")

target("Rasterizer")
    set_kind("binary")
    add_files("*.cpp")
    add_packages("opencv", "eigen3")
