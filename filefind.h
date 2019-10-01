#pragma once
#include <vector>
#include <string>
namespace opera {
	class findfile {
	public:
		std::vector<std::string> files;
		findfile() { files.clear(); }
		void getallfiles(const std::string stdir, const bool recursive,const std::string filename) {
			std::string strsearchdir = stdir;
			std::vector<std::string> vdirs;
			if (!strsearchdir.empty()) {
				switch (strsearchdir[strsearchdir.size() - 1]) {
				case'\\':
				case'/':
					break;
				default:
					strsearchdir += "\\";
					break;
				}
			}
			WIN32_FIND_DATAA wfd = {};
			HANDLE hfile = FindFirstFileA((strsearchdir + "*.*").c_str(), &wfd);
			if (INVALID_HANDLE_VALUE == hfile) { return; }
			do {
				std::string strfilename(wfd.cFileName);
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					if ((strfilename == ".") || (strfilename == "..")) {
						continue;
					}
					if (recursive) {
						vdirs.push_back(strsearchdir + strfilename);
					}
				}
				else {
					if (!filename.empty()) {
						if (strfilename!=filename) {
							continue;
						}
						files.push_back(strsearchdir + strfilename);
					}
				}
				
			} while (FindNextFile(hfile, &wfd));
			FindClose(hfile);
			if (recursive) {
				for (std::vector<std::string>::iterator iter = vdirs.begin(), end = vdirs.end(); iter != end; ++iter) {
					getallfiles(*iter, true,filename);
				}
			}
		}

	
		void findfilename(std::string stdir, bool recursive,std::string filename) {
			getallfiles(stdir, recursive,filename);
		};

	};
}