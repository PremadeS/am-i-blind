#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

#define HIGHLIGHT_START_FILE "\033[1;31m"
#define HIGHLIGHT_START_DIR "\033[1;32m"
#define HIGHLIGHT_END "\033[0m"
#define BLIND_MSG "yes you are"
#define NOT_BLIND_MSG "no."

// TODO: change the blind_msg_printed to blind_count, make it efficient and find out why error
// yes i'm using snake_case and camelCase together and the earth is still the same as it was before, shock

struct Config {
  string name;
  string path;
  bool blind_msg_printed;
  bool only_file;
  bool only_dir;
  bool recursive;     // find it recusrively?
  bool match_exactly; // find substring or da name must match exactly?
  Config()
      : recursive(false), match_exactly(false), only_file(false), only_dir(false), path(fs::current_path()),
        blind_msg_printed(false) {} // default constructor
};

void handleMatch(const fs::directory_entry &entry, Config &config) {
  try {
    string name = entry.path().filename();
    string pathStr = entry.path().string();

    if (config.match_exactly) {
      if (name == config.name) {
        if ((!config.only_file && !config.only_dir) || (config.only_dir && fs::is_directory(entry)) ||
            (config.only_file && !fs::is_directory(entry))) {

          if (!config.blind_msg_printed) {
            cout << BLIND_MSG << endl;
            config.blind_msg_printed = true;
          }

          cout << pathStr.substr(0, pathStr.length() - name.length());
          cout << (fs::is_directory(entry) ? HIGHLIGHT_START_DIR : HIGHLIGHT_START_FILE) << name << HIGHLIGHT_END;
        }
      }

    } else {
      size_t pos = name.find(config.name);
      if (pos != string::npos) {
        pos = pathStr.rfind(config.name);
        if ((!config.only_file && !config.only_dir) || (config.only_dir && fs::is_directory(entry)) ||
            (config.only_file && !fs::is_directory(entry))) {
          {

            if (!config.blind_msg_printed) {
              cout << BLIND_MSG << endl;
              config.blind_msg_printed = true;
            }

            cout << pathStr.substr(0, pos);
            cout << (fs::is_directory(entry) ? HIGHLIGHT_START_DIR : HIGHLIGHT_START_FILE) << config.name
                 << HIGHLIGHT_END;
            cout << pathStr.substr(pos + config.name.length()) << endl;
          }
        }
      }
    }
  } catch (const fs::filesystem_error &e) {
    cerr << "Error accessing " << entry.path() << ": " << e.what() << endl;
  } catch (const exception &e) {
    cerr << "Unexpected error: " << e.what() << endl;
  }
}

// find the garbage you specified
void findThisGarbage(Config &config) {
  bool blind = false;
  for (const auto &entry : fs::directory_iterator(config.path)) {
    handleMatch(entry, config);
  }
}

// recursively find the garbage you specified
void findThisGarbageRecursively(Config &config) {
  try {
    for (const auto &entry : fs::recursive_directory_iterator(config.path)) {
      try {
        handleMatch(entry, config);
      } catch (const fs::filesystem_error &e) {
        continue; // skip
      }
    }
  } catch (const fs::filesystem_error &e) {
    cerr << "Error starting recursive directory iteration: " << e.what() << endl;
  }
}

// handle ~
void handleDaFuqingSquigglyLine(Config &config) {
  if (!config.path.empty() && config.path[0] == '~') {
    const char *homeDir = getenv("HOME");
    if (homeDir) {
      config.path = string(homeDir) + config.path.substr(1);
    }
  }
}

void printHelp() {
  cout << "Usage: am-i-blind [OPTIONS] <name> [path]" << endl << endl;
  cout << "-r, --recursive      Search Recursively" << endl;
  cout << "-e, --exact          Match the name exactly" << endl;
  cout << "-f, --file           Only search for files" << endl;
  cout << "-d, --dir            Only search for directories" << endl;
  exit(0);
}

Config parseDaFuqingArgs(const int &argc, char *argv[]) {

  Config config;
  bool isNameSet = false;
  bool isPathSet = false;
  bool isFile = false;
  bool isDir = false;
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "-h" || arg == "--help") {
      printHelp();
    } else if (arg == "-e" || arg == "--exact") {
      config.match_exactly = true;
    } else if (arg == "-r" || arg == "--recursive") {
      config.recursive = true;
    } else if ((arg == "-f" || arg == "--file") && !isFile && !isDir) {
      config.only_file = true;
      isFile = true;
    } else if ((arg == "-d" || arg == "--dir") && !isFile && !isDir) {
      config.only_dir = true;
      isDir = true;
    } else if (!isNameSet) {
      config.name = arg;
      isNameSet = true;
    } else if (!isPathSet) {
      config.path = arg;
      isPathSet = true;
    } else {
      throw "r u domb, use -h";
    }
  }

  if (!isNameSet) {
    throw "u defo dumb, use -h";
  }

  return config;
}

int main(int argc, char *argv[]) {

  Config config;
  try {
    config = parseDaFuqingArgs(argc, argv);
  } catch (const char *e) {
    cerr << e << endl;
    return 1;
  }

  if (!fs::exists(config.path)) {
    cerr << "Path don't exist: " << config.path << endl;
    return 1;
  }

  handleDaFuqingSquigglyLine(config);
  if (config.recursive)
    findThisGarbageRecursively(config);
  else
    findThisGarbage(config);

  if (!config.blind_msg_printed)
    cout << NOT_BLIND_MSG << endl;

  return 0;
}