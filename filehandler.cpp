#include "filehandler.h"
#include "yard.h"

namespace fs = std::filesystem;

FileHandler::FileHandler() {
    if (!fs::is_directory("output") || !fs::exists("output")) { // Check if src folder exists
        fs::create_directory("output"); // create src folder
    }
}

void FileHandler::Write(Ferry& f, Yard& y) {
    std::ofstream ofile("output/test.txt");
    ofile << f.length << " " << f.width << " " << f.queues[0].total_size << " " << f.queues[0].width << " " << f.queues.size();
    ofile << "\n\n";
    ofile << "type reg_nr arrvival_time queue_nr queue_pos x y length width mass\n";
    std::string debug_buf = "debug";
    for (int i = 0; i < f.queues.size(); i++) {
        auto& q = f.queues[i];
        for (int j = 0; j < q.vehicles.size(); j++) {
            auto& v = q.vehicles[j];
            ofile << v.type << " " << v.plate_nr << " " << v.arrival_time << " " << std::to_string(i) << " " << std::to_string(j) << " " << v.x << " " << v.y << " " << v.length << " " << v.width << " " << v.mass << "\n";
        }
    }
    std::vector<std::tuple<Vehicle*, size_t, size_t>> veh_ptr;
    for (size_t i = 0; i < y.queues.size(); i++) {
        auto& q = y.queues[i];
        for (size_t j = 0; j < q.vehicles.size(); j++) {
            auto& v = q.vehicles[j];
            veh_ptr.push_back({&v, i, j});
        }
    }
    std::sort(veh_ptr.begin(), veh_ptr.end(), [](const auto& v1, const auto& v2){ return std::get<0>(v1)->arrival_time < std::get<0>(v2)->arrival_time; });
    ofile << "\n";
    ofile << y.queues[0].total_size << " " << y.queues[0].width << " " << y.queues.size();
    ofile << "\n\n";
    for (auto v_ptr : veh_ptr) {
        auto& v = *std::get<0>(v_ptr);
        ofile << v.type << " " << v.plate_nr << " " << v.arrival_time << " " << std::to_string(std::get<1>(v_ptr)) << " " << std::to_string(std::get<2>(v_ptr)) << " " << v.x << " " << v.y << " " << v.length << " " << v.width << " " << v.mass << "\n";
    }
}