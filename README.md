# Voxelizer for Additive Manufacutring (VOX4AM)
- Voxelizer for Additive Manufacutring (VOX4AM) was developed to generate voxelization data for training 3D convolutional neural network (CNN) models that estimate build time for AM. VOX4AM gets more than one mesh files as OBJ format (.obj) and produces a Hierarchical Data Format (HDF) file (.h5). Unlike other conventional voxelizers, VOX4AM takes the build space into consideration because the volume ratio between a mesh model and a build space is a factor that affcts the build time. The software was written in C++.
## Prerequisites
- Boost C++ Libraries 1.74 or higher
- The Computational Geometry Algorithms Library (CGAL) 5.1 or higher for processing mesh files
  - CGAL requires Boost C++ Libraries to be installed first
  - libgmp-10.dll is required to run the software
- HDF5 Library 1.12  or higher for writing HDF files
## Build the software
- Install the prerequisites listed above.
- Unzip 'HDF5Lib.zip' to the same folder.
- Open 'VOX4AM.sln' and check the additional include and library directories. CGAL_PATH, BOOST_PATH, HDF_PATH must be adjusted to your local environment.
  - Additional include directories
    - CGAL_PATH\include
    - CGAL_PATH\auxiliary\gmp\include
    - BOOST_PATH\
    - HDF_PATH\src
    - HDF_PATH\build
      - You must build HDF5 using CMake in this directory
  - Additional library directories
    - CGAL_PATH\auxiliary\gmp\lib
      - libmpfr-4.lib, libgmp-10.lib
    - HDF_PATH\build\Debug
      - libhdf5_D.lib
    - HDF_PATH\build\Release
      - libhdf5.lib
## Usage
- VOX4AM is a command line software that requires a set of parameters as follows
  - VOX4AM.exe [folder_path] [obj_size] [dimension_size] [voxel_size] [num_inside]
    - folder_path: The path where OBJ files are located. Note that obj file names must be numbers.
      - e.g. For 500 objs, the names must be 1.obj,2.obj,..,500.obj.
    - obj_size: The number of obj files at the path.
    - dimension_size: The size of dimension along one side of the build space.
    - voxel_size: The size of a unit voxel.
    - num_inside: The number representing the complete inside of the model.
      - The numbers representing outside and boundary are 0 and 1.
## Contact Information
- Soonjo Kwon, soonjo.kwon.1@gmail.com
## Version
- 1.0
## Reference
- Y. Oh et al., 2021, Neural network-based build time estimation for additive manufacturing: a performance comparison, Journal of Computational Design and Engineering, To be published.
