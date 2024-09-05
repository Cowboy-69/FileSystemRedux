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
                Log("FileSystem.cleo: WARNING - Could not open file! [OPEN_FILE]");

                SetIntParam(ctx, -1);
                return HandlerResult::CONTINUE;
            }

            bIsFileOpen = true;

            break;
        }

        if (!bIsFileOpen) {
            Log("FileSystem.cleo: WARNING - Could not open file! [OPEN_FILE]");

            SetIntParam(ctx, -1);
            return HandlerResult::CONTINUE;
        }

        SetIntParam(ctx, fileID);
        return HandlerResult::CONTINUE;
    }

    static HandlerResult CloseFile(Context ctx) {
        int fileID = GetIntParam(ctx);

        if (fileID < 0 || fileID >= MAX_FILES) {
            Log("FileSystem.cleo: WARNING - This file does not exist! [CLOSE_FILE]");
            return HandlerResult::CONTINUE;
        }

        if (!files[fileID].is_open()) {
            Log("FileSystem.cleo: WARNING - This file does not exist! [CLOSE_FILE]");
            return HandlerResult::CONTINUE;
        }

        files[fileID].close();

        return HandlerResult::CONTINUE;
    }

    static HandlerResult WriteStringToFile(Context ctx) {
        int fileID = GetIntParam(ctx);

        if (fileID < 0 || fileID >= MAX_FILES) {
            Log("FileSystem.cleo: WARNING - This file does not exist! [WRITE_STRING_TO_FILE]");
            return HandlerResult::CONTINUE;
        }

        if (!files[fileID].is_open()) {
            Log("FileSystem.cleo: WARNING - This file does not exist! [WRITE_STRING_TO_FILE]");
            return HandlerResult::CONTINUE;
        }

        char line[STR_MAX_LEN];
        GetStringParam(ctx, line, STR_MAX_LEN);

        files[fileID] << line;

        return HandlerResult::CONTINUE;
    }

} FileSystemPlugin;
