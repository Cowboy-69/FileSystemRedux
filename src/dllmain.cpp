#include "../include/cleo_redux_sdk.h"

#include <fstream>

#define MAX_FILES 32

std::fstream files[MAX_FILES];

class FileSystemPlugin {
public:
    FileSystemPlugin() {
        RegisterCommand("OPEN_FILE", OpenFile);
        RegisterCommand("CLOSE_FILE", CloseFile);
        RegisterCommand("WRITE_STRING_TO_FILE", WriteStringToFile);
    }

    static HandlerResult OpenFile(Context ctx) {
        char filePathName[STR_MAX_LEN];
        GetStringParam(ctx, filePathName, STR_MAX_LEN);

        bool bIsFileOpen = false;

        int fileID = -1;

        for (fileID = 0; fileID < MAX_FILES; fileID++) {
            if (files[fileID].is_open())
                continue;

            files[fileID].open(filePathName, std::ios_base::app);

            if (!files[fileID].good()) {
                Log("FileSystem.cleo: ERROR - Could not open file!");
                return HandlerResult::ERR;
            }

            bIsFileOpen = true;

            break;
        }

        if (!bIsFileOpen) {
            Log("FileSystem.cleo: ERROR - Could not open file!");
            return HandlerResult::ERR;
        }

        SetIntParam(ctx, fileID);

        return HandlerResult::CONTINUE;
    }

    static HandlerResult CloseFile(Context ctx) {
        int fileID = GetIntParam(ctx);

        if (fileID < 0 || fileID >= MAX_FILES || !files[fileID].is_open()) {
            Log("FileSystem.cleo: ERROR - This file does not exist!");
            return HandlerResult::ERR;
        }

        files[fileID].close();

        return HandlerResult::CONTINUE;
    }

    static HandlerResult WriteStringToFile(Context ctx) {
        int fileID = GetIntParam(ctx);

        if (fileID < 0 || fileID >= MAX_FILES || !files[fileID].is_open()) {
            Log("FileSystem.cleo: ERROR - This file does not exist!");
            return HandlerResult::ERR;
        }

        char line[STR_MAX_LEN];
        GetStringParam(ctx, line, STR_MAX_LEN);

        files[fileID] << line;

        return HandlerResult::CONTINUE;
    }

} FileSystemPlugin;
