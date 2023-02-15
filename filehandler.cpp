#include "filehandler.h"

namespace fs = std::filesystem;

FileHandler::FileHandler() {
    if (!fs::is_directory("output") || !fs::exists("output")) { // Check if src folder exists
        fs::create_directory("output"); // create src folder
    }
}

void FileHandler::Write(Ferry& f) {
    std::ofstream ofile("output/test.txt");
    ofile << f.length << " " << f.width << " " << f.queues[0].total_size << " " << f.queues[0].width << " " << f.queues.size();
    ofile << "\n\n";
    ofile << "type reg_nr arrvival_time queue_nr queue_pos x y length width mass\n";
    std::string debug_buf = "debug";
    for (int i = 0; i < f.queues.size(); i++) {
        auto& q = f.queues[i];
        for (int j = 0; j < q.vehicles.size(); j++) {
            auto& v = q.vehicles[j];
            ofile << v.type << " " << v.plate_nr << " " << v.arrival_time << " " << std::to_string(i) << " " << std::to_string(j) << " " << v.x << " " << v.y << " " << v.length << " " << v.width << " " << v.mass;
        }
    }
}