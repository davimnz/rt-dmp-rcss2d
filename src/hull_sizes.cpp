#include <chrono>
#include <random>
#include <fstream>
#include <iostream>
#include "constants.h"
#include "core/estimate.h"

#define PLAYERS 1
#define METHOD  ConvexHullEstimate

int main(int argc, char *argv[]) {

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <num_runs> <prediction_window> <state_update_count>\n";
        return 1;
    }

    int num_runs = std::stoi(argv[1]);
    int prediction_window = std::stoi(argv[2]);
    int state_update_count = std::stoi(argv[3]);

    char output_file[64];
    std::snprintf(output_file, sizeof(output_file), "./hull_sizes_%s_%s_%s.csv", argv[1], argv[2], argv[3]);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> positionXDistribution(-FIELD_HALF_WIDTH, FIELD_HALF_WIDTH);
    std::uniform_real_distribution<double> positionYDistribution(-FIELD_HALF_LENGTH, FIELD_HALF_LENGTH);
    std::uniform_real_distribution<double> velocityNormDistribution(0, MAX_VELOCITY);
    std::uniform_real_distribution<double> velocityAngleDistribution(MIN_ALLOWED_ANGLE, MAX_ALLOWED_ANGLE);
    std::uniform_real_distribution<double> bodyAngleDistribution(MIN_ALLOWED_ANGLE, MAX_ALLOWED_ANGLE);
    std::vector<Point> points;
    std::vector<std::vector<double>> positions;
    std::vector<std::vector<double>> velocities;
    for (int i = 0; i < PLAYERS * num_runs; ++i) {

        double px = positionXDistribution(gen);
        double py = positionYDistribution(gen);

        double vnorm = velocityNormDistribution(gen);
        double vangle = velocityAngleDistribution(gen);

        double vx = vnorm * cos(vangle * M_PI / 180.0);
        double vy = vnorm * sin(vangle * M_PI / 180.0);

        double body = bodyAngleDistribution(gen);

        points.emplace_back(Point{
                px,
                py,
                vx,
                vy,
                body
        });
    }

    ConvexHullVector chVec;

    std::fstream file;
    file.open(output_file, std::ios::out);
    file << "num_runs=" << num_runs << "\n";
    file << "prediction_window=" << prediction_window << "\n";
    file << "state_update_count=" << state_update_count << "\n";
    file << "hull_sizes" << "\n";
    for (int i = 0; i < num_runs; ++i) {
        for (int j = 0; j < PLAYERS; ++j) {
            chVec = METHOD(points[j + i * PLAYERS], state_update_count, prediction_window);
        }
        for (size_t j = 0; j < chVec.size(); ++j) {
            file << chVec[j].size() << " ";
        }
        file << "\n";
    }
    file.close();
    return 0;
}
