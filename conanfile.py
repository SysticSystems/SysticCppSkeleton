from conan import ConanFile
from conan.tools.cmake import cmake_layout

class SysticSkeletonConan(ConanFile):
    name = "systic_skeleton"
    version = "1.0.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def layout(self):
        # This aligns Conan's internal paths with your script's OUT_DIR logic
        # It creates the 'build' folder structure automatically
        cmake_layout(self)

    def requirements(self):
        self.requires("gtest/1.14.0")
        self.requires("benchmark/1.8.3")

    def configure(self):
        # Force the handshake: Ensure everyone is using the same C++ standard
        # Validating our C++23/26 requirement for low-level determinism
        self.settings.compiler.cppstd = "23"
        
        # Hardening: Ensure we use the correct ABI for Clang on Linux
        if self.settings.compiler == "clang":
            self.settings.compiler.libcxx = "libstdc++11"

    def build_requirements(self):
        # REMOVED: self.tool_requires("cmake/...") 
        # We rely on the system CMake (apk add cmake) to avoid musl/glibc conflicts.
        # If you need Ninja specifically from Conan, only add it if apk doesn't have it.
        pass