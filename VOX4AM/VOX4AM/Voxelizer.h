#pragma once


class Voxelizer
{
public:
	Voxelizer() {};
	~Voxelizer() {};

	void Voxelize(string folderPath, int64_t fileSize, double dim, double voxSize, float numInside, int& fileCount, float*& data);
	bool CheckDimension(double dim, double bb_x_max, double bb_x_min, double bb_y_max, double bb_y_min, double bb_z_max, double bb_z_min);
};