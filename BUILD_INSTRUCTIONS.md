# Building and Testing - System Configuration Issue

## Issue Found

The system's C++ compiler cannot find standard library headers like `<cmath>`. This is a system configuration issue with Xcode Command Line Tools.

## Solutions

### Option 1: Update Xcode Command Line Tools (Recommended)
```bash
xcode-select --install
# Or if already installed, try:
sudo xcode-select --switch /Library/Developer/CommandLineTools
```

### Option 2: Install g++-14 via Homebrew (As specified in CMakePresets)
```bash
brew install gcc@14
# Then use: CMAKE_CXX_COMPILER=g++-14 cmake ...
```

### Option 3: Manual Build (if headers work manually)
Once the compiler issue is resolved:

```bash
# Configure
mkdir -p build && cd build
cmake .. -DCMAKE_CXX_STANDARD=20 -DCMAKE_CXX_STANDARD_REQUIRED=ON

# Build tests
cmake --build . --target utcommon

# Run tests
./utcommon
# Or using CTest:
ctest --output-on-failure
```

## Test Files Created

The following test files were created:
- `utcommon/test_vector.cpp` - Vector operations tests
- `utcommon/test_material.cpp` - Material tests  
- `utcommon/test_sphere.cpp` - Sphere intersection tests
- `utcommon/test_scene.cpp` - Scene parser tests
- `utcommon/test_config.cpp` - Config parser tests

Once the compiler issue is resolved, all tests should compile and run successfully.
