from conan import ConanFile

class SysticSkeletonConan(ConanFile):
    name = "systic_skeleton"
    version = "1.0.0"
    
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    
    # We will generate CMake Toolchain and Dependencies files
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        # Specific dependency requirements for our skeleton
        self.requires("gtest/1.14.0")
        self.requires("benchmark/1.8.3")

    def build_requirements(self):
        # Toolchain dependencies so Conan provisions our exact build system
        self.tool_requires("cmake/[>=3.25]")
        self.tool_requires("ninja/[>=1.11]")
