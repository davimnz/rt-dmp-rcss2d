#include "csv.h"
#include "core/estimate.h"

#define METHOD ConvexHullEstimate

int main(int argc, char *argv[]) {

    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <input_path> <output_path> <state_update_count> <prediction_window>\n";
        return -1;
    }

    std::string input_path{argv[1]};
    std::string output_path{argv[2]};
    int state_update_count = std::stoi(argv[3]);
    int prediction_window = std::stoi(argv[4]);

    /* Read input file */
    std::vector<int> validCycles;
    GameLog log;
    int numRows = ParseLog(input_path, log, validCycles);
    if (numRows < 0) {
        return -1;
    }

    /* Set output file */
    char output_file[128];
    std::snprintf(output_file, sizeof(output_file), "%s/convexhull_%d_%d.csv", output_path.c_str(), state_update_count, prediction_window);
    std::ofstream file(output_file);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << output_file << std::endl;
        return -1;
    }
    file << "player" << ",";
    file << "cycle" << ",";
    file << "x" << ",";
    file << "y" << "\n";

    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < numRows; ++j) {
            auto convexHullVector = METHOD(log[i][j].second, state_update_count, prediction_window);
            int lastConvexHullSize = convexHullVector[convexHullVector.size() - 1].size();
            for (int k = 0; k < lastConvexHullSize; ++k) {
                file << (i + 1) << ",";
                file << log[i][j].first << ",";
                file << convexHullVector[convexHullVector.size() - 1][k].x << ",";
                file << convexHullVector[convexHullVector.size() - 1][k].y << "\n";
            }
        }
    }
    return 0;
}
