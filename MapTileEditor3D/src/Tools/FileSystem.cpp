#include "Tools/FileSystem.h"

#include <fstream>
#include <filesystem>
#include <stack>
#include <Windows.h>

#include "Tools/System/Logger.h"

#include "Tools/System/Profiler.h"
#include "SDL.h"

#include "ExternalTools/mmgr/mmgr.h"

namespace fs = std::filesystem;

Folder* FileSystem::root = nullptr;
Folder* FileSystem::appdata = nullptr;
std::string FileSystem::sAppdata;

nlohmann::json FileSystem::OpenJSONFile(const char* path)
{
    PROFILE_FUNCTION();
	std::ifstream f(path);
	if (f.good()) {
		nlohmann::json j;
		f >> j;
		f.close();
		return j;
	}
    LOGE("Cannot open json with path %s", path);

	return nlohmann::json();
}

void FileSystem::SaveJSONFile(const char* path, const nlohmann::json& file)
{
    PROFILE_FUNCTION();
    std::ofstream o(path);
    o << std::setw(4) << file << std::endl;
}

std::string FileSystem::OpenTextFile(const char* path)
{
    PROFILE_FUNCTION();
    try {
        std::ifstream in(path, std::ios::in | std::ios::binary);
        if (in)
        {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize((const unsigned int)in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return contents;
        }
        throw path;
    }
    catch (...) {
        LOGE("Cannot open file with path %s", path);
        throw path;
    }
}

void FileSystem::SaveTextFile(const char* path, const char* file)
{
    std::ofstream f;
    f.open(path);
    f << file;
    f.close();
}

bool FileSystem::Exists(const char* path)
{
    return fs::exists(path);
}

uint64_t FileSystem::LastTimeWrite(const char* path)
{
    struct stat time;
    if (stat(path, &time) == 0) {
        return time.st_mtime;
    }
    return 0ULL;
}

std::string FileSystem::NormalizePath(const char* path)
{
    std::string p(path);
    std::string ret;
    auto i = p.begin();
    while (i != p.end()) {
        if (*i == '\\' /*|| *i == '\/'*/) {
            ret.push_back('/');
        }
        else {
            ret.push_back(*i);
        }

        ++i;
    }
    return ret;
}

bool FileSystem::CreateFolder(const char* path)
{
    return fs::create_directory(path);
}

bool FileSystem::fDeleteFile(const char* path)
{
    std::error_code err;
    if (!fs::remove(path, err)) {
        LOGW("Could not delete %s, error: %s", path, err.message().c_str());
        return false;
    }
    
    return true;
}

bool FileSystem::DeleteFolder(const char* path)
{
    std::error_code err;
    if (!fs::remove_all(path, err)) {
        LOGW("Could not delete %s, error: %s", path, err.message().c_str());
        return false;
    }

    return true;
}

bool FileSystem::CopyTo(const char* source, const char* dst)
{
    std::error_code err;
    fs::copy(source, dst, fs::copy_options::overwrite_existing, err);
    if (err.value() != 0)
        LOGE("Copying from %s to %s failed: %s", source, dst, err.message().c_str());
    return err.value() == 0;
}

bool FileSystem::MoveTo(const char* source, const char* dst)
{
    if (CopyTo(source, dst))
        return fDeleteFile(source);
    return false;
}

bool FileSystem::IsFileInFolder(const char* file, const char* folder, bool recursive)
{
    auto f = GetPtrFolder(folder);
    for (auto i = f->files.begin(); i != f->files.end(); ++i)
        if ((*i).first.compare(file) == 0)
            return true;
    return false;
}

Folder* FileSystem::GetFolders(const char* path)
{
    Folder* parent = new Folder(path, nullptr);
    std::stack<Folder*> stack;
    stack.push(parent);

    while (!stack.empty()) {
        Folder* folder = stack.top();
        stack.pop();

        for (const auto& entry : fs::directory_iterator(folder->full_path)) {
            if (entry.is_directory()) {
                Folder* f = new Folder((entry.path().u8string() + '/').c_str(), folder);
                stack.push(f);
                folder->folders.push_back(f);
            }
            else {
                folder->files[entry.path().filename().string()] = LastTimeWrite((folder->full_path + entry.path().filename().string()).c_str());
            }
        }
    }

    return parent;
}

std::string FileSystem::GetFileExtension(const char* file, bool with_dot)
{
    std::string f(file);
    std::string ext;

    for (auto i = f.rbegin(); i != f.rend(); ++i) {
        if (*i != '.')
            ext.push_back(std::tolower(*i));
        else {
            if (with_dot)
                ext.push_back(std::tolower(*i));
            break;
        }
    }

    std::reverse(ext.begin(), ext.end());
    return ext;
}

std::string FileSystem::GetNameFile(const char* file, bool with_extension)
{
    std::string f(file);
    std::string name;

    for (auto i = f.rbegin(); i != f.rend(); ++i) {
        if (*i != '/' && *i != '//' && *i != '\\')
            name.push_back(*i);
        else
            break;
    }

    if (!with_extension) {
        if (std::find(name.begin(), name.end(), '.') != name.end()) {
            auto i = name.begin();
            int size = 0;
            while (i != name.end()) {
                if (*i == '.') {
                    ++size;
                    break;
                }
                else {
                    ++size;
                    ++i;
                }
            }
            name.erase(name.begin(), name.begin() + size);
        }
        else
            LOGW("File %s has not extension", name.c_str());
    }

    std::reverse(name.begin(), name.end());
    return name;
}

std::string FileSystem::GetFolder(const char* path)
{
    std::string p(path);
    std::string ret;

    bool end_of_file = false;
    for (auto i = p.rbegin(); i != p.rend(); ++i) {
        if (!end_of_file) {
            if (*i == '/' || *i == '//' || *i == '\\') {
                end_of_file = true;
            }
        }
        else {
            ret.push_back(*i);
        }
    }

    std::reverse(ret.begin(), ret.end());
    return ret;
}

std::string FileSystem::GetNameFolder(const char* path, bool with_slash)
{
    std::string ret;
    std::string p(path);

    bool first_slash = false;
    for (auto i = p.rbegin(); i != p.rend(); ++i) {
        if (*i == '/' || *i == '//' || *i == '\\') {
            if (first_slash) {
                first_slash = true;
                if (with_slash)
                    ret.push_back(*i);
            }
            else
                break;
        }
        else {
            ret.push_back(*i);
        }
    }
    std::reverse(ret.begin(), ret.end());
    return ret;
}

std::string FileSystem::GetParentFolder(const char* path)
{
    std::string p(path);
    std::string ret;

    bool end_of_file = false;
    bool first_slash = true;
    for (auto i = p.rbegin(); i != p.rend(); ++i) {
        if (!end_of_file) {
            if (*i == '/' || *i == '//' || *i == '\\') {
                if (first_slash) {
                    first_slash = false;
                }
                else {
                    end_of_file = true;
                }
            }
        }
        else {
            ret.push_back(*i);
        }
    }

    std::reverse(ret.begin(), ret.end());
    return ret;
}

std::string FileSystem::GetFullPath(const char* path)
{
    return fs::absolute(path).string();
}

std::string FileSystem::GetCanonical(const char* path)
{
    return fs::canonical(path).u8string();
}

Folder* FileSystem::GetPtrFolder(const char* folder, bool is_appdata)
{
    std::stack<Folder*> s;
    std::string sfolder;
    if (is_appdata) {
        if (appdata->full_path.compare(fs::canonical(folder).u8string()) == 0)
            return appdata;

        s.push(appdata);
        sfolder = fs::canonical(folder).u8string();
    }
    else {
        if (root->full_path.compare(folder) == 0)
            return root;

        s.push(root);
        sfolder = fs::canonical(std::string("./") + folder).u8string();
    }

    while (s.empty() == false) {
        auto f = s.top();
        s.pop();
        for (auto i = f->folders.begin(); i != f->folders.end(); ++i) {
            if ((*i)->full_path.compare(sfolder) == 0) {
                return *i;
            }
            if (sfolder.find(fs::canonical((*i)->full_path).u8string()) != std::string::npos) {
                s.push(*i);
            }
        }
    }
    LOGW("Folder %s not found", folder);
    return nullptr;
}

Folder* FileSystem::GetRootFolder()
{
    return root;
}

Folder* FileSystem::RegenerateRootFolder()
{
    //TODO: don't delete all folders
    delete root;
    return root = FileSystem::GetFolders("./");
}

void FileSystem::GenerateFolders()
{
    char currDir[MAX_PATH];
    if (GetCurrentDirectoryA(MAX_PATH, currDir) == 0) {
        LOGE("GetCurrentDirectory Error(%d)", GetLastError());
    }
    std::string sCurrDir = currDir;

#ifdef DISTR
    sAppdata = GetCanonical(SDL_GetPrefPath(ORGANIZATION, APP_NAME));
    appdata = GetFolders(sAppdata.c_str());
#else
    sAppdata = fs::canonical(sCurrDir + "/installation_dir").u8string();
    appdata = GetFolders(sAppdata.c_str());
#endif

    std::string newDir(sCurrDir.c_str(), sCurrDir.size() - sizeof("MapTileEditor3D") + 1);
    if (SetCurrentDirectoryA((newDir + "test/").c_str()) == 0) {
        LOGE("SetCurrentDirectoryA Error(%d)", GetLastError());
    }
    if (GetCurrentDirectoryA(MAX_PATH, currDir) == 0) {
        LOGE("GetCurrentDirectory Error(%d)", GetLastError());
    }
    root = GetFolders("./");
}

void FileSystem::DeleteRoot()
{
    delete root;
    delete appdata;
}

Folder::Folder(const char* n, Folder* parent) :full_path(fs::canonical(n).u8string()), parent(parent)
{
    for (auto i = full_path.rbegin(); i != full_path.rend(); ++i) {
        if (i != full_path.rbegin()) {
            if (*i == '/' || *i == '\\' || *i == '//')
                break;
            else
                name.push_back(*i);
        }
    }
    std::reverse(name.begin(), name.end());
}
