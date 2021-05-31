// OBJ2HDF5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include "Voxelizer.h"
#include "HDF_Writer.h"

bool CheckInputParams(int64_t fileSize, double dimension, double voxSize, float numInside)
{
    if (fileSize <= 0)
    {
        cout << "obj_count must be greater than 0." << endl;
        return false;
    }

    if (dimension <= 0.0)
    {
        cout << "dimension_size must be greater than 0." << endl;
        return false;
    }

    if (voxSize <= 0.0 || voxSize > dimension)
    {
        cout << "voxel_size must be greater than 0 and less than dimension_size." << endl;
        return false;
    }

    if (numInside < 0.0 || numInside > 1.0)
    {
        cout << "num_inside must be between 0 and 1." << endl;
        return false;
    }

    return true;
}

int main(int argc, char* argv[])
{
    if (argc != 6)
    {
        cout << "[Usage] " << argv[0] << " [folder_path] [obj_size] [dimension_size] [voxel_size] [num_inside]" << endl;
        return -1;
    }

    // Input parameters
    string folderPath(argv[1]);
    int64_t fileSize = atoi(argv[2]);
    double dimension = atof(argv[3]);   // Dimension of the build space
    double voxSize = atof(argv[4]);     // Size of a voxel
    float numInside = stof(argv[5]);    // The number representing the complete inside of a model

    // Check input parameters
    if (!CheckInputParams(fileSize, dimension, voxSize, numInside))
        return -1;

    int voxelPerSide = (int)ceil(dimension / voxSize);                  // Number of voxels along one axis  
    int64_t voxelPerModel = voxelPerSide * voxelPerSide * voxelPerSide; // Number of voxels in a model
    int64_t voxelInFiles = fileSize * voxelPerModel;                    // Number of voxels in all files

    // Array for storing all values
    float* data = new float[voxelInFiles];

    // Number of obj files successfully processed
    int fileCount = 0;
    clock_t startTime = clock();

    cout << "Reading and voxelizing OBJ files.." << endl;
    Voxelizer v;
    v.Voxelize(folderPath, fileSize, dimension, voxSize, numInside, fileCount, data);

    // Exit if there's no file to read
    if (fileCount == 0)
    {
        cout << "There is no OBJ file to read." << endl;
        return -1;
    }

    string outFilePath = folderPath + "\\voxelizationData(" + to_string(fileSize) + ")" + "_("
        + to_string((int)dimension) + "," + to_string((int)voxSize) + ").h5";

    cout << "Writing an integrated HDF5 file.." << endl;
    HDF_Writer hw;
    hw.Write(outFilePath, fileCount, voxelPerModel, data);

    // Delete the data array
    delete[] data;

    clock_t endTime = clock();
    clock_t time = endTime - startTime;
    double timeInSec_dbl = (double)time / (double)CLOCKS_PER_SEC;

    // Report time elapsed
    cout << "OBJ(s) to HDF5 completed!" << endl;
    cout << "Time elapsed: " << timeInSec_dbl << " s" << endl;

    return 0;
}
