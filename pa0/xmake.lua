add_requires("eigen3")

target("Transformation")
    set_kind("binary")
    add_files("*.cpp")
    add_packages("eigen3")
