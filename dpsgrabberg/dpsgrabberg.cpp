#include <fstream>
#include <iostream>
#include <Windows.h>
#include <string>
int main(int argc, char* argv[])
{
    /* check path */
    if (!argv[1])
    {
        std::cout << "invalid specified path\n";
        getchar();
        return -1;
    }

    /* Open file */
    std::ifstream file(argv[1], 0);
    if (!file) {
        std::cout << "failed to open file";
        getchar();
        return -1;
    }


    /* DOS Header */
    IMAGE_DOS_HEADER dosHeader;
    file.read((char*)(&dosHeader), sizeof(dosHeader));
    if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
        file.close();
        std::cout << "invalid DOS header";
        getchar();
        return -1;
    }


    /* PE header */
    file.seekg(dosHeader.e_lfanew, 0);
    IMAGE_NT_HEADERS ntHeader;
    file.read((char*)(&ntHeader), sizeof(ntHeader));
    if (ntHeader.Signature != IMAGE_NT_SIGNATURE) {
        file.close();
        std::cout << "invalid PE header";
        getchar();
        return -1;
    }


    /* _IMAGE_NT_HEADERS -> IMAGE_FILE_HEADER -> TimeDateStamp*/
    const auto compiledTime = ntHeader.FileHeader.TimeDateStamp;
    file.close();


    /* convert to DPS format */
    std::time_t timestamp = (std::time_t)(compiledTime);
    const std::tm* time = new tm;
    gmtime_s((tm*)time, &timestamp);

    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y/%m/%d:%H:%M:%S", time);
    std::cout << '!' << buffer << '!';
    getchar();
    return 0;
}

