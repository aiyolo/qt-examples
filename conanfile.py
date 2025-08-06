from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeToolchain, CMakeDeps


class ExampleRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    default_options = {
        "qt/*:shared": True,
        "qt/*:with_mysql": False,
        "qt/*:with_sqlite3": False,
        "qt/*:svg": True,
    }

    def requirements(self):
        self.requires("opencv/4.11.0")
        self.requires("qt/5.15.16")
        self.requires("fmt/11.2.0")
        self.requires("mdns/1.4.3")


    def layout(self):
        cmake_layout(self)
        self.folders.generators = ".deps"
