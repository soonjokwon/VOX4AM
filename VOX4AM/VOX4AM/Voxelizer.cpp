#include "stdafx.h"
#include "Voxelizer.h"


void Voxelizer::Voxelize(string folderPath, int64_t fileSize, double dim, double voxSize, float numInside, int& fileCount, float*& data)
{
    int voxelPerSide = (int)ceil(dim / voxSize);
    int64_t voxelPerModel = voxelPerSide * voxelPerSide * voxelPerSide; // Number of voxels in a model
    int64_t voxelInFiles = fileSize * voxelPerModel;                    // Number of voxels in all files

    int64_t count_val = 0;

    FILE* log = nullptr;

    string logPath = folderPath + "\\log(" + to_string(fileSize) + ")" + "_("
        + to_string((int)dim) + "," + to_string((int)voxSize) + ").csv";

    fopen_s(&log, logPath.c_str(), "w");

    fprintf_s(log, "folder_path,%s\n", folderPath.c_str());
    fprintf_s(log, "obj_size,%I64d\n", fileSize);
    fprintf_s(log, "dimension_size,%.1f\n", dim);
    fprintf_s(log, "voxel_size,%.1f\n", voxSize);
    fprintf_s(log, "num_inside,%.1f\n", numInside);

    fprintf_s(log, "START\n");

    for (int i = 1; i <= fileSize; ++i)
    {
        if (count_val == voxelInFiles)
            break;

        string filePath = folderPath + "\\" + to_string(i) + ".obj";

        // Check if the file exists
        if (!fs::is_regular_file(filePath))
            continue;

        fprintf_s(log, "%d,", i);

        ifstream input(filePath.c_str());
        vector<Point> points;
        vector<vector<size_t>> polygons;

        // Read an OBJ file
        if (!CGAL::read_OBJ(input, points, polygons))
        {
            cout << "Error parsing the OBJ file: " << to_string(i) << ".obj" << endl;
            fprintf_s(log, "FAIL(IMPORT)\n");

            continue;
        }

        // Polygonal mesh data
        Polyhedron poly;
        CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, poly);

        // Bounding box of the input model
        Tree tree(faces(poly).first, faces(poly).second, poly);
        CGAL::Bbox_3 bb = tree.bbox();

        double bb_x_min = bb.xmin();
        double bb_y_min = bb.ymin();
        double bb_z_min = bb.zmin();

        double bb_x_max = bb.xmax();
        double bb_y_max = bb.ymax();
        double bb_z_max = bb.zmax();

        // Skip if the bounding box is smaller than the input dimension
        if (!CheckDimension(dim, bb_x_max, bb_x_min, bb_y_max, bb_y_min, bb_z_max, bb_z_min))
        {
            fprintf_s(log, "FAIL(DIMENSION)\n");

            continue;
        }

        // Place the model as follows
        // 1) at the center of the build space along x,y axes
        // 2) at the bottom of the build space along z axis

        double bb_x_mid = (bb_x_min + bb_x_max) / 2;
        double bb_y_mid = (bb_y_min + bb_y_max) / 2;
        double bb_z_mid = (bb_z_min + bb_z_max) / 2;

        const double halfDimension = dim / 2;
        const double halfVoxSize = voxSize / 2;

        double x_min = bb_x_mid - halfDimension;
        double x_max = bb_x_mid + halfDimension;

        double y_min = bb_y_mid - halfDimension;
        double y_max = bb_y_mid + halfDimension;

        double z_min = bb_z_min;
        double z_max = z_min + dim;

        Inside inside(poly);

        // Voxel count along Z direction
        int count_z = 0;

        // Flag to check if the model has an active voxel (1.0)
        bool voxelizeFlag = false;

        // Traverse along z, y, x
        for (double z = z_min; z < z_max; z += voxSize)
        {
            count_z++;

            if (count_z > voxelPerSide)
                break;

            // Voxel count along Y direction
            int count_y = 0;

            for (double y = y_min; y < y_max; y += voxSize)
            {
                count_y++;

                if (count_y > voxelPerSide)
                    break;

                // Voxel count along X direction
                int count_x = 0;

                for (double x = x_min; x < x_max; x += voxSize)
                {
                    count_x++;

                    if (count_x > voxelPerSide)
                        break;

                    // Create a voxel
                    CGAL::Bbox_3 voxel(x, y, z, x + voxSize, y + voxSize, z + voxSize);

                    // Check if the voxel intersects with the model
                    if (tree.do_intersect(voxel))
                    {
                        data[count_val] = 1.0f;
                    }
                    else
                    {
                        Point voxelCenter(x + halfVoxSize, y + halfVoxSize, z + halfVoxSize);

                        // Check if the model includes the center point of the voxel
                        CGAL::Bounded_side side = inside(voxelCenter);

                        if (side == CGAL::ON_BOUNDED_SIDE) // Inside
                        {
                            data[count_val] = numInside;
                        }
                        else // Outside
                        {
                            data[count_val] = 0.0f;
                        }
                    }

                    // Set the flag TRUE if there's an active voxel
                    if (!voxelizeFlag
                        && data[count_val] != 0.0f)
                        voxelizeFlag = true;

                    count_val++;
                }
            }
        }

        if (voxelizeFlag)
        {
            fprintf_s(log, "SUCCESS\n");
            cout << i << endl;
            fileCount++;
        }
        else
        {
            fprintf_s(log, "FAIL(NULL)\n");
            count_val = count_val - voxelPerModel;
        }
    }

    fprintf_s(log, "END\n");
    fclose(log);
}

bool Voxelizer::CheckDimension(double dim, double bb_x_max, double bb_x_min, double bb_y_max, double bb_y_min, double bb_z_max, double bb_z_min)
{
    double bb_x_gap = bb_x_max - bb_x_min;
    double bb_y_gap = bb_y_max - bb_y_min;
    double bb_z_gap = bb_z_max - bb_z_min;

    // Skip if the bounding box is smaller than the input dimension
    if (!((dim >= bb_x_gap)
        && (dim >= bb_y_gap)
        && (dim >= bb_z_gap)))
    {
        double minDim = 0.0;

        if ((bb_x_gap >= bb_y_gap) && (bb_x_gap >= bb_z_gap))
            minDim = bb_x_gap;
        else if ((bb_y_gap >= bb_x_gap) && (bb_y_gap >= bb_z_gap))
            minDim = bb_y_gap;
        else if ((bb_z_gap >= bb_x_gap) && (bb_z_gap >= bb_y_gap))
            minDim = bb_z_gap;

        stringstream ss_minDim;
        ss_minDim << minDim;

        cout << "Dimension size must be greater than " << ss_minDim.str() << "." << endl;

        return false;
    }
    
    return true;
}
