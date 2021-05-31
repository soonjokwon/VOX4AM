# 3D Voxelizer for Additive Manufacutring (VOX4AM)
- Voxelizer for Additive Manufacutring (VOX4AM) was developed to generate voxelization data for training 3D convolutional neural network (CNN) models that estimate build time for AM or 3D printing. VOX4AM gets more than one mesh files as OBJ format (.obj) and produces a single Hierarchical Data Format (HDF) file (.h5) that consists of an 1D array of 0 and 1 values. The array should be resized when imported in your application.
- Unlike other conventional 3D model voxelizers, VOX4AM takes the build space into consideration because the volume ratio between a mesh model and a build space is a factor that affcts the build time. 
- The software was written in C++.
![Fig 4](https://user-images.githubusercontent.com/67339153/120209624-dcff3700-c269-11eb-933a-4064a32ef2f8.jpg)
## Prerequisites
- [Boost C++ Libraries](https://www.boost.org/) 1.74 or higher
- [The Computational Geometry Algorithms Library (CGAL)](https://www.cgal.org/download.html) 5.1 or higher for processing mesh files
  - CGAL requires Boost C++ Libraries to be installed first
  - libgmp-10.dll is required to run the software
- [HDF5 Library](https://www.hdfgroup.org/downloads/hdf5/source-code/) 1.12  or higher for writing HDF files
## Build the software
- Install the prerequisites listed above.
- Open 'VOX4AM.sln' and check the additional include and library directories (CGAL_PATH, BOOST_PATH, HDF_PATH must be adjusted to your local environment)
  - Additional include directories
    - CGAL_PATH\include
    - CGAL_PATH\auxiliary\gmp\include
    - BOOST_PATH\
    - HDF_PATH\src
    - HDF_PATH\build
      - You must build HDF5 first using CMake in this directory
  - Additional library directories
    - CGAL_PATH\auxiliary\gmp\lib
      - libmpfr-4.lib, libgmp-10.lib
    - HDF_PATH\build\Debug
      - libhdf5_D.lib
    - HDF_PATH\build\Release
      - libhdf5.lib
## Usage
- VOX4AM is a command line software that requires a set of arguments as follows
  ```
  VOX4AM.exe [folder_path] [obj_size] [dimension_size] [voxel_size] [num_inside]
  ```
    - folder_path: The path where OBJ files are located
    - obj_size: The number of obj files at the given path
      - Note that obj file names must be numbers
        - If there are 500 obj files, the obj file names **MUST** be 1.obj,2.obj,..,500.obj
    - dimension_size: The size of dimension along one side of the build space
      - This value will be equally set to width, length, height
    - voxel_size: The size of a unit voxel
      - If voxel_size is 3 and dimension_size is 300, the number of voxels along one side will be 100
    - num_inside: The number representing the complete inside of the 3D model (usually 1 but can be changed)
      - The numbers representing outside and boundary of the 3D model are 0 and 1
## Contact Information
- Soonjo Kwon, soonjo.kwon@kumoh.ac.kr
- Yosep Oh, yosep.oh@kyonggi.ac.kr
## Version
- 1.0
## Reference
- Oh et al., 2021, **Neural network-based build time estimation for additive manufacturing: a performance comparison**, *Journal of Computational Design and Engineering*, To be published.
