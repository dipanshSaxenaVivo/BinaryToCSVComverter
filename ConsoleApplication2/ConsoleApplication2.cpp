#include <iostream>
#include <fstream>
#include <iomanip>

#pragma pack(push, 1)
struct RadarLogEntryExtended {
    int32_t radarID;
    int32_t frameNumber;
    uint32_t timestamp;
    int32_t trackID;
    float posX, posY, posX_STD, posY_STD;
    float relVelX, relVelY, relVelX_STD, relVelY_STD;
    float velX, velY;
    float accelX, accelY, accelX_STD, accelY_STD;
    float orientation, orientation_STD;
    float length, width, height;
    int32_t classification;
    float classification_confidence;
    int32_t motionStatus;
    float existProb, obstacleProb;
    uint16_t age;
    uint8_t quality, isObstacle, observerStatus, lane;
    float rcs;
    uint8_t overrideable, underdrivable;
    float overrideableProb, underdrivableProb;
    uint8_t reserved[6];
};
#pragma pack(pop)

int main(int argc, char* argv[]) {
   /* if (argc < 3) {
        std::cout << "Usage: binary_to_csv_extended <input.bin> <output.csv>" << std::endl;
        return 1;
    }*/

    /*std::ifstream inFile(argv[1], std::ios::binary);
    std::ofstream outFile(argv[2]);*/

    std::ifstream inFile("C:\\Users\\dipan\\UDP\\UDP_Data_Extraction_Documents\\UDP_Data_Extraction_Documents\\ReceivedCanfdNet-VS2022\\x64\\debug\\radar_data_ingress.bin", std::ios::binary);
    std::ofstream outFile("C:\\Users\\dipan\\UDP\\UDP_Data_Extraction_Documents\\UDP_Data_Extraction_Documents\\ReceivedCanfdNet-VS2022\\x64\\Release\\radar_data_ingress1.csv");

    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Failed to open files!" << std::endl;
        return 1;
    }

    // Read header
    uint32_t magic, version, structSize;
    inFile.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    inFile.read(reinterpret_cast<char*>(&version), sizeof(version));
    inFile.read(reinterpret_cast<char*>(&structSize), sizeof(structSize));

    if (magic != 0x52445232) {
        std::cerr << "Invalid file format! Magic: 0x" << std::hex << magic << std::endl;
        return 1;
    }

    std::cout << "Version: " << version << ", Entry size: " << structSize << " bytes" << std::endl;

    // Write comprehensive CSV header
    outFile << "RadarID,Frame,Timestamp,TrackID,"
        << "PosX,PosY,PosX_STD,PosY_STD,"
        << "RelVelX,RelVelY,RelVelX_STD,RelVelY_STD,"
        << "VelX,VelY,"
        << "AccelX,AccelY,AccelX_STD,AccelY_STD,"
        << "Orientation,Orientation_STD,"
        << "Length,Width,Height,"
        << "Classification,ClassConf,MotionStatus,"
        << "ExistProb,ObstacleProb,Age,Quality,IsObstacle,ObserverStatus,Lane,"
        << "RCS,"
        << "Overrideable,Underdrivable,OverrideableProb,UnderdrivableProb\n";

    outFile << std::fixed << std::setprecision(4);

    // Convert entries
    RadarLogEntryExtended entry;
    size_t count = 0;

    while (inFile.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
        outFile << entry.radarID << "," << entry.frameNumber << ","
            << entry.timestamp << "," << entry.trackID << ","
            << entry.posX << "," << entry.posY << ","
            << entry.posX_STD << "," << entry.posY_STD << ","
            << entry.relVelX << "," << entry.relVelY << ","
            << entry.relVelX_STD << "," << entry.relVelY_STD << ","
            << entry.velX << "," << entry.velY << ","
            << entry.accelX << "," << entry.accelY << ","
            << entry.accelX_STD << "," << entry.accelY_STD << ","
            << entry.orientation << "," << entry.orientation_STD << ","
            << entry.length << "," << entry.width << "," << entry.height << ","
            << entry.classification << "," << entry.classification_confidence << ","
            << entry.motionStatus << ","
            << entry.existProb << "," << entry.obstacleProb << ","
            << entry.age << "," << (int)entry.quality << ","
            << (int)entry.isObstacle << "," << (int)entry.observerStatus << ","
            << (int)entry.lane << ","
            << entry.rcs << ","
            << (int)entry.overrideable << "," << (int)entry.underdrivable << ","
            << entry.overrideableProb << "," << entry.underdrivableProb << "\n";

        count++;
        if (count % 10000 == 0) {
            std::cout << "Processed " << count << " entries..." << std::endl;
        }
    }

    std::cout << "\nConversion complete! Total entries: " << count << std::endl;
    return 0;
}