#include "FileWatcher.h"
#include <Logger.h>
#include <unistd.h>
#include <poll.h>

FileWatcher::FileWatcher() {
    fileDescriptor = inotify_init();
    if (fileDescriptor < 0) {
        Logger::logError("Couldn't start inotify");
    } else {
        Logger::logInfo("FileWatcher started. Ready for adding file watch subscriptions");
    }
}

void FileWatcher::addWatch(std::string fileName, std::function<void()> callback) {
    Logger::logInfo("Adding file watch subscription to: " + filename);
    int watchDescriptor = inotify_add_watch(fileDescriptor, filename.c_str(), IN_CLOSE_WRITE);
    if (watchDescriptor < 0) {
        Logger::logError("Failed to subscribe file watch to file: " + filename);
        return;
    }

    numEvents++;
    watchCallbacks.insert(std::pair<WatchID, std::function<void()>>(watchDescriptor, callback));
}

void FileWatcher::update() {
    struct pollfd pfd = {fileDescriptor, POLLIN, POLLIN};
    int timeout = 1;
    int eventsAvailable = poll(&pfd, 1, timeout);

    if(eventsAvailable > 0) {
        char readBuffer[EVENT_BUF_LEN];
        ssize_t length = read(fileDescriptor, readBuffer, EVENT_BUF_LEN);

        int i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &readBuffer[i];
            if (event->mask & IN_CLOSE_WRITE) {
                auto value = watchCallbacks.find(event->wd);
                if(value != watchCallbacks.end()) {
                    value->second();
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }
}
