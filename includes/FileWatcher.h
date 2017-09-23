#pragma once

#include <sys/inotify.h>
#include <string>
#include <map>
#include <functional>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN ( 1024 * ( EVENT_SIZE + 16 ))
typedef int WatchID;

/**
 * @brief Calls callbacks when disks on file change
 *
 * This class is used for executing callbacks when a file changes on disk.
 * This might be useful if you change resources on disk and quickly want to
 * reload them in the engine. Should be used for debugging purposes only.
 *
 * \code
 * FileWatcher fileWatcher();
 * fileWatcher.addWatch(local/filename.txt, [](){printf("Callback called");});
 * fileWatcher.update();
 * \endcode
 */
class FileWatcher {
public:
    FileWatcher();

    /**
    * @brief Adds a file watch that calls the callback whenever the file is modified
    *
    * This is only used for debugging purposes to quickly reload resources.
    *
    * @param fileName The name of the mesh file.
    */
    void addWatch(std::string fileName, std::function<void()> callback);

    /**
    * @brief File watches is poll based and the update method needs to be called regularily.
    *
    * The file watches fill up a buffer in the kernel that is read and parsed in the update method.
    * If a file change matches your subscription the associated callback is called.
    *
    */
    void update();
private:
    int fileDescriptor;
    int numEvents = 0;
    std::map<WatchID, std::function<void()>> watchCallbacks;
};