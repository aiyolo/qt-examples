from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeToolchain, CMakeDeps


class ExampleRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    default_options = {
        "qt/*:shared": True,
        "qt/*:qtcharts": True,
        "qt/*:svg": True,
        "qt/*:with_mysql": False
    }

    def requirements(self):
        self.requires("qt/5.15.16")

    def build_requirements(self):
        self.build_requires("ninja/1.12.1")

    def layout(self):
        cmake_layout(self)
        self.folders.generators = "conan/generators"

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()
