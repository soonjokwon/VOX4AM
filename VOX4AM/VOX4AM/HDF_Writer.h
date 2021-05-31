#pragma once


class HDF_Writer
{
public:
	HDF_Writer() {};
	~HDF_Writer() {};

	void Write(string outFilePath, int fileCount, int64_t voxelPerModel, float*& data);
};